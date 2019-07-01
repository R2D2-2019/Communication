#pragma once

#include <external_connection.hpp>
#include <hwlib.hpp>
#include <queue.hpp>

namespace r2d2::communication {

    /**
     * @brief Interface for an esp connected with wifi
     * @details extends from external_connection_c
     */

    class esp_32_c : public external_connection_c {
    private:
        hwlib::spi_bus &spi_connection;
        hwlib::pin_out &slave_select;
        hwlib::pin_in &hand_shake;
        r2d2::queue_c<r2d2::frame_external_s, 32> recv_queue;

    public:
        /**
         * @brief the constructor for the esp32 interface
         *
         * @details slave select should be a pin_out dummy when using the hwspi implementation
         *
         * @param spi_connection The spi bus connected to the esp
         * @param ss slave select of chip select connected to the esp
         * @param hs The hand shake pin connected to the esp
         */
        esp_32_c(hwlib::spi_bus &spi_connection, hwlib::pin_out &ss, hwlib::pin_in &hs);

        /**
         *  @brief Sends external frame to connected target
         *  @details Sends external frame to connected target,
         *  but also receives a frame if available. It stores this in a queue
         *
         * @param rfame The frame to send to the target, contains binary data
         */
        void send(const r2d2::frame_external_s &frame) override;

        /**
         * @brief Receives frame from connected target
         *
         * @param
         * @return true There is data in queue
         * @return false No data in queue
         */
        bool receive(r2d2::frame_external_s &frame) override;
    };

} // namespace r2d2::communication