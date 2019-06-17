#include <hwlib.hpp>

namespace r2d2::spi {

struct spi0 {
    constexpr static uint32_t sclk = PIO_PA27A_SPI0_SPCK;
    constexpr static uint32_t miso = PIO_PA25A_SPI0_MISO;
    constexpr static uint32_t mosi = PIO_PA26A_SPI0_MOSI;
};



class hardware_spi_c : public hwilb::spi_bus {
private:
    Spi *const port = SPI0;

    void enable() {
        port->SPI_CR = SPI_CR_SPIEN;
    }

    void disable () {
        port->SPI_CR = SPI_CR_SPIDIS;
    }

    void send_byte(uint8_t byte){
        while (!port->SPI_SR & SPI_SR_TDRE) {}
        port->SPI_TDR = SPI_TDR_TD(byte);
    }

    uint8_t receive_byte(){
        while (!port->SPI_SR & SPI_SR_RDRF) {}
        return port->RDR;
    }

    void write_and_read(const size_t n, 
            const uint8_t data_out[], 
            uint8_t data_in[]
    ) override { 
        for (uint_fast8_t i = 0; i < n; ++i){
            send(data_out[n]);
        }

    }

public:
    hardware_spi_c(){
        //enable clock for spi peripheral
        PMC->PMC_PCER0 = (0x01 << ID_SPI0);
        port->SPI_MR |= SPI_MR_MSTR | 0x01 | WDRBT;
        //port->SPI_CSR[]

        enable();
    
    }    

};


}
