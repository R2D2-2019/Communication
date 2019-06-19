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
        r2d2::queue_c<r2d2::frame_s, 32> recv_queue;

    public:
        esp_32_c(hwlib::spi_bus &spi_connection, hwlib::pin_out &ss);

        /**
         *  @brief Sends frame to connected target
         *  @details Sends frame to connected target,
         *  but also receives a frame if available. It stores this in a queue
         *
         * @param rame The frame to send to the target, contains binary data
         */
        void send(const r2d2::frame_s &frame) override ;

        /**
         * @brief Receives frame from connected target
         *
         * @param
         * @return true There is data in queue
         * @return false No data in queue
         */
        bool receive(r2d2::frame_s &frame) override;
    };

}