#pragma once

#include <cstddef>
#include <cstdint>

namespace r2d2::communication {
    /**
     * @brief  frame type that contains an byte array/pointer with length.
     */
    struct frame_s {
        /**
         * @brief Size of the data in bytes (1 byte, 8 bits equals 1)
         */
        size_t size;
        /**
         * @brief Frame data including headers
         */
        uint8_t data[];
    };
}
