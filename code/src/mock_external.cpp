#include <mock_external.hpp>

namespace r2d2::communication {
    void mock_external_c::send(const r2d2::frame_external_s &frame){
        for (auto &reaction : send_reactions) {
            reaction(frame, receive_buffer);
        }
        return;
    }

    bool mock_external_c::receive(r2d2::frame_external_s &frame) {
        if (receive_buffer.empty()){
            return false;
        }
        frame = receive_buffer.front();
        receive_buffer.erase(receive_buffer.begin());
        return true;
    }

    void mock_external_c::set_receive_frame(const r2d2::frame_external_s &frame){
        receive_buffer.push_back(frame);
        return;
    }

    void mock_external_c::set_receive_frames(const std::vector<r2d2::frame_external_s> frames){
        receive_buffer.clear();
        for (auto &frame : frames){
            receive_buffer.push_back(frame);
        }
        return;
    }

    void mock_external_c::set_send_reaction(std::function<void(
            const r2d2::frame_external_s&,
            std::vector<r2d2::frame_external_s>&
    )> reaction) {
        send_reactions.push_back(reaction);
        return;
    }


}
