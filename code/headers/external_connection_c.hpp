#pragma once
#include <cstddef>
#include <cstdint>

#include "frame_s.hpp"

namespace r2d2::communication {
    class external_connection_c {
    public:
        virtual void send(const frame_s& frame) = 0;
        virtual bool receive(frame_s& frame) = 0;
    };
}