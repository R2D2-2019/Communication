#pragma once

#include <cstddef>
#include <cstdint>

namespace r2d2::communication {
    struct frame_s {
        size_t size;
        uint8_t data[];
    };
}