#pragma once

#include <base_module.hpp>
#include <esp_32.hpp>

namespace r2d2::robos {
    class module_c : public base_module_c {
    protected:
        /**
         * The button pin.
         */
        hwlib::pin_in &button;
        r2d2::communication::esp_32_c &esp;
        int8_t speed = 0;
        int8_t steering_angle = 0;
        bool brake = false;
        bool flag_for_send = 0;
        bool forward = true;

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
                    r2d2::frame_type::MANUAL_CONTROL_SLIDER, 
                    frame_type::MANUAL_CONTROL_JOYSTICK,
                    frame_type::MANUAL_CONTROL_BUTTON
                }
            );
        }



        void process_movement_control_speed(const frame_s &frame) {
            auto data = frame.as_frame_type<frame_type::MANUAL_CONTROL_SLIDER>();

            // convert to moving platform
            // slider id 0 is left peddel and slider id 1 is right peddel
            if (data.slider_id == 0) {
                brake = true;
                speed = 0;
            } else {
                brake = false;
                if (forward == true) {
                    speed = data.value;
                } else {
                    speed = -data.value;
                }
            }
            flag_for_send = 1;
            // Data recieved, reset timer
        }

        void process_movement_control_steer(const frame_s &frame) {
            auto data = frame.as_frame_type<frame_type::MANUAL_CONTROL_JOYSTICK>();

            // convert to moving platform
            steering_angle = data.value_x;
            flag_for_send = 1;
            // Data recieved, reset timer
        }

        void process_movement_control_direction(const frame_s &frame) {
            auto data = frame.as_frame_type<frame_type::MANUAL_CONTROL_BUTTON>();
            hwlib::cout << " button pressed: " << data.button_id << "\n";
            if (data.button_id == 0 && data.value == true) {
                forward = true;
            } else if (data.button_id == 3 && data.value == true) {
                forward = false;
            }

         }

        /**
         * Let the module process data.
         */
        void process() override {
            while (comm.has_data()) {
                auto frame = comm.get_data();

                if (frame.request) {
                    continue;
                }


                switch (frame.type) {
                    case frame_type::MANUAL_CONTROL_SLIDER:
                        process_movement_control_speed(frame);
                        break;
                    case frame_type::MANUAL_CONTROL_JOYSTICK:
                        process_movement_control_steer(frame);
                        break;
                    case frame_type::MANUAL_CONTROL_BUTTON:
                        process_movement_control_direction(frame);
                        break;
                    default:
                        break;
                }

                if (flag_for_send) {
                    flag_for_send = 0;
                    
                    frame_movement_control_s movement;

                    movement.brake = brake;
                    movement.rotation = steering_angle;
                    movement.speed = speed;
                
                    r2d2::frame_external_s ext;
                    size_t size = sizeof(frame_movement_control_s);
                    
                    for (size_t i = 0; i < size; i++) {
                        ext.data[i] = reinterpret_cast<const uint8_t *>(&movement)[i];
                    }

                    ext.type = r2d2::MOVEMENT_CONTROL;
                    ext.length = size;
                    ext.id = { 0, 0 };

                    esp.send(ext);

                     
                }
            }
        }
    };
}
