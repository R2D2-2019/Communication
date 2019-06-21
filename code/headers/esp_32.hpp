#pragma once

#include <hwlib.hpp>
#include <external_connection.hpp>
#include <queue.hpp>

namespace r2d2::communication {

    /**
     * @brief Interface for an esp connected with wifi
     * @details extends from external_connection_c
     */


    class esp_32_c : public external_connection_c{
    private:
        hwlib::spi_bus &spi_connection;
        hwlib::pin_out &slave_select;
        r2d2::queue_c<r2d2::frame_external_s, 32> recv_queue;

    public:

        /**
         * @brief the constructor for the esp32 interface
         *
         * @param spi_connection The spi bus connected to the esp
         * @param ss The chip select or slave select connected to the esp
         */
        esp_32_c(hwlib::spi_bus &spi_connection, hwlib::pin_out &ss);

        /**
         *  @brief Sends external frame to connected target
         *  @details Sends external frame to connected target,
         *  but also receives a frame if available. It stores this in a queue
         *
         * @param rfame The frame to send to the target, contains binary data
         */
        void send(const r2d2::frame_external_s &frame) override ;

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