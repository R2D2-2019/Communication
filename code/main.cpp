#include "hwlib.hpp"
#include <esp_32.hpp>


int main(void) {
  // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;
    hwlib::wait_ms(1000);

    auto sclk = hwlib::target::pin_out(hwlib::target::pins::d9);
    auto mosi = hwlib::target::pin_out(hwlib::target::pins::mosi);
    auto miso = hwlib::target::pin_in(hwlib::target::pins::miso);
    auto ss = hwlib::target::pin_out(hwlib::target::pins::d10);

    //configure spi bus with cs 0 = pin 10
    //configure spi bus with spi mode 1
    //configure with divider 42 ( 84 / 42 = 2Mhz )
    auto spi_bus = hwlib::target::hwspi(0, hwlib::target::hwspi::SPI_MODE::SPI_MODE1, 42);

    //big size for testing
    size_t size = 100;
    while(1) {
        //chars to send
        char data[] = "Het duurt te lang";
        //chars to receive
        char received[size] = {0};
        //read and write
        spi_bus.transaction(ss).write_and_read(size ,(uint8_t*)data, (uint8_t*)received);
        //show what is received
        hwlib::cout << (char*)received << '\n';
    }
}
