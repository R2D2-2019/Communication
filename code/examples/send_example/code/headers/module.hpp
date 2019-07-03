#pragma once

#include <base_module.hpp>
#include <esp_32.hpp>

namespace r2d2::button {
    class module_c : public base_module_c {
    protected:
        /**
         * The button pin.
         */
        hwlib::pin_in &button;
        r2d2::communication::esp_32_c &esp;

    public:
        /**
         * @param comm
         * @param button
         */
        module_c(base_comm_c &comm, r2d2::communication::esp_32_c &esp, hwlib::pin_in &button)
            : base_module_c(comm), button(button), esp(esp) {

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
                frame_button_state_s button_state;

                button_state.pressed = button.read();

                //const uint8_t *data = reinterpret_cast<const uint8_t *>(&button_state);

                r2d2::frame_external_s ext;
                
                size_t size = sizeof(frame_button_state_s);

                for (size_t i = 0; i < size; i++) {
                    ext.data[i] = reinterpret_cast<const uint8_t *>(&button_state)[i];
                }

                ext.type = r2d2::frame_type::BUTTON_STATE;
                ext.length = size;
                ext.id = { 0, 0 };
//hwlib::cout << * reinterpret_cast<const uint8_t *>(&ext) << '\n';

                esp.send(ext);
            }
        }
    };
}
