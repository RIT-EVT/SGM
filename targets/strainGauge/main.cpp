/**
* This example prints out stress reading from a strain gauge
*/

#include <EVT/io/ADC.hpp>
#include <EVT/io/UART.hpp>
#include <EVT/manager.hpp>
#include <EVT/utils/time.hpp>
#include <SGM.hpp>

namespace IO = EVT::core::IO;
namespace DEV = EVT::core::DEV;
namespace time = EVT::core::time;

/**
* Note, this conversion function is just to show off how a conversion
* function will look like. In actuality, this will be determined through
* testing/reading the strain gauge datasheet.
*/
uint32_t convert(uint32_t voltage) {
    return voltage / 2;
}

int main() {
    EVT::core::platform::init();

    // Setup IO
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    // Setup pins for Strain Gauges
    IO::ADC& adcA = IO::getADC<IO::Pin::PC_3>();// Strain Gauge A
    IO::ADC& adcB = IO::getADC<IO::Pin::PC_2>();// Strain Gauge B
    IO::ADC& adcC = IO::getADC<IO::Pin::PC_1>();// Strain Gauge C
    IO::ADC& adcD = IO::getADC<IO::Pin::PC_0>();// Strain Gauge D

    // Create an instance for each strain gauge
    SGM::DEV::StrainGauge gauges[NUM_GAUGES] = {
        SGM::DEV::StrainGauge(adcA, convert),
        SGM::DEV::StrainGauge(adcB, convert),
        SGM::DEV::StrainGauge(adcC, convert),
        SGM::DEV::StrainGauge(adcD, convert),
    };

    uart.printf("Starting strain gauge test\r\n");

    while (1) {
        uart.printf("A Strain: %u unit\r\n",
                    gauges[0].getForce());
        uart.printf("A Voltage: %u V\r\n",
                    gauges[0].getRawADC());
        uart.printf("B Strain: %u unit\r\n",
                    gauges[1].getForce());
        uart.printf("B Voltage: %u V\r\n",
                    gauges[1].getRawADC());
        uart.printf("C Strain: %u unit\r\n",
                    gauges[2].getForce());
        uart.printf("C Voltage: %u V\r\n",
                    gauges[2].getRawADC());
        uart.printf("D Strain: %u unit\r\n",
                    gauges[3].getForce());
        uart.printf("D Voltage: %u V\r\n\n",
                    gauges[3].getRawADC());
        time::wait(500);
    }
}
