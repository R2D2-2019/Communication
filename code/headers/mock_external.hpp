#pragma once

#include <external_connection.hpp>
#include <frame_types.hpp>
#include <vector>
#include <functional>

namespace r2d2::communication {
    class mock_external_c : public external_connection_c {
    private:
        std::vector<r2d2::frame_external_s> receive_buffer;
        std::vector<std::function<void(
                const r2d2::frame_external_s&,
                std::vector<r2d2::frame_external_s>&
        )>> send_reactions;
    public:

        /**
         * @brief constructor, nothing to construct
         */
        mock_external_c() {}

        /**
         * @brief send simulates a external_connection_c::send()
         *
         * @param frame frame to send, does not send but checks in reaction vector for reaction
         */
        void send(const r2d2::frame_external_s &frame) override;

        /**
         * @brief receives first item in buffer
         *
         * @param frame frame to be overwritten when buffer is not empty
         * @return bool , true when buffer is not empty, false when empty
         */
        bool receive(r2d2::frame_external_s &frame) override;

        /**
         * @brief set one frame to be received
         *
         * @param frame that will be pushed into the vector
         */
        void set_receive_frame(const r2d2::frame_external_s &frame);

        /**
         * @brief set a vector of frames to be received
         *
         * @param frames , vector of frames to be set
         */
        void set_receive_frames(const std::vector<r2d2::frame_external_s> frames);

        /**
         * @brief set a reaction lambda
         *
         * @param reaction , a reaction lamda wich can react
         */
        void set_send_reaction(std::function<void(
                const r2d2::frame_external_s&,
                std::vector<r2d2::frame_external_s>& )> reaction);


    };
} // namespace r2d2::communication