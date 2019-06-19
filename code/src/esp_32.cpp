#include <esp_32.hpp>

namespace r2d2::communication {

    esp_32_c::esp_32_c(hwlib::spi_bus &spi_connection, hwlib::pin_out &ss):
        spi_connection(spi_connection),
        slave_select(ss)
    {}

    void esp_32_c::send(const r2d2::frame_s &frame) {
        uint8_t * recv = {0};
        uint8_t data[frame.length + 2] = {frame.type, (uint8_t)frame.request};
        memcpy(data+2, frame.data, frame.length +1);
        spi_connection.transaction(slave_select).write_and_read(frame.length + 2, data, recv);
        if (recv[0] != 0) {
            r2d2::frame_s fr;
            fr.length = recv[0];
            fr.request = (bool)recv[1];
            fr.data = recv + 2;
            recv_queue.push(fr);

        }
    }

    bool esp_32_c::receive(r2d2::frame_s &frame) {
        if (recv_queue.empty()) {
            return false;
        } else {
            r2d2::frame_s tmp = recv_queue.copy_and_pop();
            frame.data = tmp.data;
            frame.type = tmp.type;
            frame.request = tmp.request;
            return true;
        }
    }
}