#pragma once

#include <base_module.hpp>
#include <esp_32.hpp>

namespace r2d2::button {
    class module_c : public base_module_c {
    protected:
        /**
         * The button pin.
         */
        hwlib::pin_out &led;
        r2d2::communication::esp_32_c &esp;

    public:
        /**
         * @param comm
         * @param button
         */
        module_c(base_comm_c &comm, r2d2::communication::esp_32_c &esp, hwlib::pin_out &led)
            : base_module_c(comm), led(led), esp(esp) {

            // Set up listeners
            comm.listen_for_frames(
                {
                    r2d2::frame_type::BUTTON_STATE
                }
            );
        }

        /**
         * Let the module process data.
         */
        void process() override {
            while (1) {
//auto frame = comm.get_data();

                // Only handle requests
                //if (!frame.request) {
                //    continue;
                //}

                // Get button state, create frame and send
                
                r2d2::frame_external_s ext{};

                while (!esp.receive(ext)) {}
                
                frame_button_state_s button_state = *reinterpret_cast<frame_button_state_s *>(&ext.data);
                hwlib::cout << button_state.pressed << '\n';

                led.write(button_state.pressed);

            }
        }
    };
}
