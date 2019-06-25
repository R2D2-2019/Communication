# ESP CODE
## What esp is used?
We have used this code with the heltec wifi kit 32. This is because one of the engineers had one lying around. 
## How to compile and flash

#### ESP 32
For deploying the code for the ESP 32 the official esp idf was used. You need to setup some things to get it working.

1. Read the getting started page https://docs.espressif.com/projects/esp-idf/en/latest/get-started/ 
2. Check if you have all required hardware on hand.
3. Install the development environment. 
    - Setup the Toolchain
    - Get the ESP-IDF https://github.com/espressif/esp-idf 
    - Set Eniviroment Variables
    - Installl the required python packages
4. To flash the esp
    - Copy the tcp-example to next to the esp-idf directory 
    - Replace tcp-client.c in the example with the tcp-client.c in this repository 
    - Set target access point with password with `make menuconfig`.
    - Set target host with port with `make menuconfig`.
    - Connect the esp with an usb-cable to your pc.
    - Set target usb-port with `make menuconfig`.
    - Use `make flash` to flash your esp.
    - It should work now, contact Patrick if things are not working.
