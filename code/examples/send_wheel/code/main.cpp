#include <hwlib.hpp>

#include <comm.hpp>

#include <module.hpp>

#include <esp_32.hpp>

int main() {
    using namespace r2d2;

    // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;

    hwlib::wait_ms(10);

    r2d2::comm_c comm;

    auto spi_bus = hwlib::target::hwspi(1, hwlib::target::hwspi::SPI_MODE::SPI_MODE1, 42);

    auto hand_shake = hwlib::target::pin_in(hwlib::target::pins::d5);

    auto pin_in = hwlib::target::pin_in(hwlib::target::pins::d11);

    auto esp = r2d2::communication::esp_32_c(spi_bus, hwlib::pin_out_dummy, hand_shake);

    robos::module_c module(comm, esp, pin_in);

    for (;;) {
        module.process();
    }
}


