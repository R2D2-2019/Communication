/* BSD Socket API Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/param.h>

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

#include "lwip/dns.h"
#include "lwip/err.h"
#include "lwip/igmp.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

#include "driver/gpio.h"
#include "driver/spi_slave.h"
#include "esp32/rom/cache.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "esp_spi_flash.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "protocol_examples_common.h"
#include "soc/rtc_periph.h"
#include "tcpip_adapter.h"

#ifdef CONFIG_EXAMPLE_IPV4
#define HOST_IP_ADDR CONFIG_EXAMPLE_IPV4_ADDR
#else
#define HOST_IP_ADDR CONFIG_EXAMPLE_IPV6_ADDR
#endif

#define PORT CONFIG_EXAMPLE_PORT

// Pins in use. The SPI Master can use the GPIO mux, so feel free to change
// these if needed.
#define GPIO_HANDSHAKE 15
#define GPIO_MOSI 23
#define GPIO_MISO 19
#define GPIO_SCLK 18
#define GPIO_CS 2

static const char *TAG = "example";
// static const char *payload = "Message from ESP32 ";

size_t buffer_size = 129;

// Called after a transaction is queued and ready for pickup by master. We use
// this to set the handshake line high.
void my_post_setup_cb(spi_slave_transaction_t *trans) {
    WRITE_PERI_REG(GPIO_OUT_W1TS_REG, (1 << GPIO_HANDSHAKE));
}

// Called after transaction is sent/received. We use this to set the handshake
// line low.
void my_post_trans_cb(spi_slave_transaction_t *trans) {
    WRITE_PERI_REG(GPIO_OUT_W1TC_REG, (1 << GPIO_HANDSHAKE));
}

// buffers for spi
DMA_ATTR char sendbuf[129] = "";
DMA_ATTR char recvbuf[129] = "";

static void tcp_client_task(void *pvParameters) {
    // char rx_buffer[128];
    char addr_str[128];
    int addr_family;
    int ip_protocol;

    while (1) {

        struct sockaddr_in dest_addr;
        dest_addr.sin_addr.s_addr = inet_addr(HOST_IP_ADDR);
        dest_addr.sin_family = AF_INET;
        dest_addr.sin_port = htons(PORT);
        addr_family = AF_INET;
        ip_protocol = IPPROTO_IP;
        inet_ntoa_r(dest_addr.sin_addr, addr_str, sizeof(addr_str) - 1);

        int sock = socket(addr_family, SOCK_STREAM, ip_protocol);
        if (sock < 0) {
            ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
            break;
        }
        ESP_LOGI(TAG, "Socket created, connecting to %s:%d", HOST_IP_ADDR,
                 PORT);

        int err =
            connect(sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
        if (err != 0) {
            ESP_LOGE(TAG, "Socket unable to connect: errno %d", errno);
            break;
        }
        ESP_LOGI(TAG, "Successfully connected");

        int n = 0;
        esp_err_t ret;

        // Configuration for the SPI bus
        spi_bus_config_t buscfg = {.mosi_io_num = GPIO_MOSI,
                                   .miso_io_num = GPIO_MISO,
                                   .sclk_io_num = GPIO_SCLK};

        // Configuration for the SPI slave interface
        spi_slave_interface_config_t slvcfg = {
            .mode = 1,
            .spics_io_num = GPIO_CS,
            .queue_size = 3,
            .flags = 0,
            .post_setup_cb = my_post_setup_cb,
            .post_trans_cb = my_post_trans_cb};

        // Configuration for the handshake line
        gpio_config_t io_conf = {.intr_type = GPIO_INTR_DISABLE,
                                 .mode = GPIO_MODE_OUTPUT,
                                 .pin_bit_mask = (1 << GPIO_HANDSHAKE)};

        // Configure handshake line as output
        gpio_config(&io_conf);
        // Enable pull-ups on SPI lines so we don't detect rogue pulses when no
        // master is connected.
        gpio_set_pull_mode(GPIO_MOSI, GPIO_PULLUP_ONLY);
        gpio_set_pull_mode(GPIO_SCLK, GPIO_PULLUP_ONLY);
        gpio_set_pull_mode(GPIO_CS, GPIO_PULLUP_ONLY);

        // Initialize SPI slave interface
        ret = spi_slave_initialize(HSPI_HOST, &buscfg, &slvcfg, 1);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Failed to initialize spi slave");
            break;
        }

        memset(recvbuf, 0, 129);
        spi_slave_transaction_t t = {0};
        memset(&t, 0, sizeof(t));

        while (1) {
            // Clear receive buffer,
            memset(recvbuf, 0x00, 129);

            // Set up a transaction of 128 bytes to send/receive on spi
            t.length = 128 * 8;
            t.tx_buffer = sendbuf;
            t.rx_buffer = recvbuf;

            ret = spi_slave_transmit(HSPI_HOST, &t, portMAX_DELAY);
            // set send buffer to something sane
            memset(sendbuf, 0x00, 129);
            int err = send(sock, recvbuf, strlen(recvbuf), 0);
            if (err < 0) {
                ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
                break;
            }

            int len = recv(sock, sendbuf, sizeof(sendbuf) - 1, 0);
            // Error occurred during receiving
            if (len < 0) {
                ESP_LOGE(TAG, "recv failed: errno %d", errno);
                break;
            }
            // Data received
            else {
                sendbuf[len] = 0; // Null-terminate whatever we received and
                                  // treat like a string
                ESP_LOGI(TAG, "Received %d bytes from %s:", len, addr_str);
                ESP_LOGI(TAG, "%s", sendbuf);
            }

            // vTaskDelay(2000 / portTICK_PERIOD_MS);
        }

        if (sock != -1) {
            ESP_LOGE(TAG, "Shutting down socket and restarting...");
            shutdown(sock, 0);
            close(sock);
        }
    }
    vTaskDelete(NULL);
}

void app_main() {
    ESP_ERROR_CHECK(nvs_flash_init());
    tcpip_adapter_init();
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    /* This helper function configures Wi-Fi or Ethernet, as selected in
     * menuconfig. Read "Establishing Wi-Fi or Ethernet Connection" section in
     * examples/protocols/README.md for more information about this function.
     */
    ESP_ERROR_CHECK(example_connect());

    xTaskCreate(tcp_client_task, "tcp_client", 4096, NULL, 5, NULL);
}
