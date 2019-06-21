# ESP CODE
## What esp is used?
We have used this code with the heltec wifi kit 32. This is because one of the engineers had on lying around. 
## How to compile and flash
This code is written with the use of the official esp_idf from espressif. 
Here's how to get started: https://docs.espressif.com/projects/esp-idf/en/latest/get-started/ 
Here's the github: https://github.com/espressif/esp-idf 
If all this is done place the tcp_client directory next to the esp_ide directory. Next set your port with `man menuconfig`. You should now be able to flash your esp with `make flash`. Monitor it with `make monitor` close your monitor with `Crtl+]`. 
