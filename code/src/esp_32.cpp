#include <esp_32.hpp>

namespace r2d2::communication {

    esp_32_c::esp_32_c(hwlib::spi_bus &spi_connection, hwlib::pin_out &ss, hwlib::pin_in &hs)
        : spi_connection(spi_connection),
          slave_select(ss),
          hand_shake(hs)
          {}

    void esp_32_c::send(const r2d2::frame_external_s &frame) {
        // set the first bite of the buffer to 0
        // only the first because the fist byte of an external frame is its length
        uint8_t recv[256] = {0};
        // wait till esp is ready
        while (!hand_shake.read()){}
        spi_connection.transaction(slave_select)
            .write_and_read(frame.length,
                            reinterpret_cast<const uint8_t *>(&frame), recv);
        // if the length of the external frame received is 0 assume nothing was send
        // and don't put the frame in the receive que
        if (recv[0] != 0) {
            recv_queue.push(*reinterpret_cast<r2d2::frame_external_s *>(&recv));
        }
        return;
    }

    bool esp_32_c::receive(r2d2::frame_external_s &frame) {
        if (recv_queue.empty()) {
            // set the first bite of the buffer to 0
            // only the first because the fist byte of an external frame is its length
            uint8_t recv[256] = {0};
            // wait till esp is ready
            while (!hand_shake.read()){}
            spi_connection.transaction(slave_select).write_and_read(256, nullptr, recv);
            // if the length of the external frame received is 0 assume nothing was send
            // and don't put the frame in the receive que
            if (recv[0] != 0) {
                recv_queue.push(*reinterpret_cast<r2d2::frame_external_s *>(&recv));
            } else {
                return false;
            }
        }

        frame = recv_queue.copy_and_pop();
        return true;
    }
} // namespace r2d2::communication