#include <esp_32.hpp>

namespace r2d2::communication {

    esp_32_c::esp_32_c(hwlib::spi_bus &spi_connection, hwlib::pin_out &ss, hwlib::pin_in &hs)
        : spi_connection(spi_connection),
          slave_select(ss),
          hand_shake(hs)
          {}

    void esp_32_c::send(const r2d2::frame_external_s &frame) {
        // set the first bite of the buffer to 0
        // only the first because the first byte of an external frame is its length
        uint8_t recv[256] = {0};
        uint8_t send[256] = {frame.length, frame.id.octets[0], frame.id.octets[1], frame.type};
                memcpy(send+4, frame.data, frame.length );
        //for (uint8_t i = 0; i < frame.length + 1 ; i++) {
            //hwlib::cout << send[i];
        //}
        //hwlib::cout << '\n';

// wait till esp is ready
        while (!hand_shake.read()){hwlib::cout << 1;}
        //spi_connection.transaction(slave_select)
        //    .write_and_read(frame.length ,
        //                    reinterpret_cast<const uint8_t *>(&frame), recv);
        spi_connection.transaction(slave_select)
            .write_and_read(frame.length + 4, send, recv);
        hwlib::wait_ms(10);
        // if the length of the external frame received is 0 assume nothing was send
        // and don't put the frame in the receive que
        if (recv[0] != 0) {
            recv_queue.push(*reinterpret_cast<r2d2::frame_external_s *>(&recv));
        }
        return;
    }

    bool esp_32_c::receive(r2d2::frame_external_s &frame) {
        if (recv_queue.empty()) {
            // check if esp is ready
            if (hand_shake.read()){
                // set the first bite of the buffer to 0
                // only the first because the first byte of an external frame is its length
                uint8_t recv[256] = {0};
                spi_connection.transaction(slave_select).write_and_read(256, nullptr, recv);
                // if the length of the external frame received is 0 assume nothing was send
                // and don't put the frame in the receive que
                if (recv[0] != 0) {
                    //for (uint8_t i = 0; i < recv[0] + 4 ; i++) {
                    //    hwlib::cout << recv[i];
                    //}
                    //hwlib::cout << recv[0] << '\n';
                    recv_queue.push(*reinterpret_cast<r2d2::frame_external_s *>(&recv));
                } else {
                    return false;
                }
            } else {
                return false;
            }
        }

        frame = recv_queue.copy_and_pop();
        return true;
    }
} // namespace r2d2::communication
