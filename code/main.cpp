#include "hwlib.hpp"
#include <hardware_usart.hpp>

int main(void) {
  // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;
    hwlib::wait_ms(1000);

    auto usart = r2d2::usart::hardware_usart_c<r2d2::usart::usart0>(9600);

    while(true){
        //usart << "heyo!";
        usart.send('a');
    }

}