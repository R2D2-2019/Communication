#include <esp_32.hpp>

namespace r2d2::communication {

    esp_32_c::esp_32_c(hwlib::spi_bus &spi_connection, hwlib::pin_out &ss):
        spi_connection(spi_connection),
        slave_select(ss)
    {}

    void esp_32_c::send(const r2d2::frame_external_s &frame) {
        uint8_t recv[256] = {0};
        spi_connection.transaction(slave_select).write_and_read(
                frame.length,
                reinterpret_cast<const uint8_t*>(&frame),
                recv
                );

        if (recv[0] != 0) {
            recv_queue.push(*reinterpret_cast<r2d2::frame_external_s*>(&recv));
        }
        return;
    }

    bool esp_32_c::receive(r2d2::frame_external_s &frame) {
        if (recv_queue.empty()) {
            return false;
        } else {
            frame = recv_queue.copy_and_pop();
            return true;
        }
    }
}