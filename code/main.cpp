#include "hwlib.hpp"
#include <hardware_usart.hpp>


int main(void) {
  // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;
    hwlib::wait_ms(1000);

    auto sclk = hwlib::target::pin_out(hwlib::target::pins::d9);
    auto mosi = hwlib::target::pin_out(hwlib::target::pins::mosi);
    auto miso = hwlib::target::pin_in(hwlib::target::pins::miso);
    auto ss = hwlib::target::pin_out(hwlib::target::pins::d8);

    auto spi_bus = hwlib::spi_bus_bit_banged_sclk_mosi_miso(sclk, mosi, miso);

    size_t size = 4;
//    uint8_t * received = 0;
    while(1) {
        char data[] = "mate";
        spi_bus.transaction(ss).write_and_read(size ,(uint8_t*)data, nullptr);
        hwlib::wait_ms(1000);
    }

/*    auto usart = r2d2::usart::hardware_usart_c<r2d2::usart::usart0>(9600);

    while(true){
        //usart << "heyo!";
        usart.send('a');
    }
*/
}
