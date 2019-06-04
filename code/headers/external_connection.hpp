#pragma once
#include <cstddef>
#include <cstdint>

#include "frame_s.hpp"

namespace r2d2::communication {
    /**
     * @brief Standard interface for connections with external devices
     */
    class external_connection_c {
    public:
        /**
         * @brief Sends frame to connected target
         * 
         * @param frame The frame to send to the target, contains binary data
         */
        virtual void send(const frame_s& frame) = 0;
        
        /**
         * @brief Receives frame from connected target
         * 
         * @param
         * @return true There is data received
         * @return false No data available
         */
        virtual bool receive(frame_s& frame) = 0;
    };
}