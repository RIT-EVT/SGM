/**
* This example prints out stress reading from a strain gauge
*/
#include <SGM.hpp>
#include <EVT/io/ADC.hpp>
#include <EVT/io/UART.hpp>
#include <EVT/manager.hpp>
#include <EVT/utils/time.hpp>

namespace IO = EVT::core::IO;
namespace DEV = EVT::core::DEV;
namespace time = EVT::core::time;

/**
* Note, this conversion function is just to show off how a conversion
* function will look like. In actuality, this will be determined through
* testing/reading the thermistor datasheet.
*/
uint32_t convert(uint32_t voltage) {
    return voltage / 2;
}

int main() {
    EVT::core::platform::init();

    // Setup IO
    IO::UART& uart = IO::getUART<IO::Pin::PA_6, IO::Pin::PA_5>(9600);

    time::wait(500);

    IO::ADC& adc = IO::getADC<IO::Pin::PA_0>();

    time::wait(500);

    // Setup the strain gauge device
    SGM::SGM strainGauge(adc, convert);

    uart.printf("Starting thermistor test\r\n");

    while (1) {
        uart.printf("Temperature: %dmC\r\n",
                    static_cast<int>(strainGauge.getProcessedData()));
        uart.printf("Voltage: %dV\r\n",
                    static_cast<int>(strainGauge.getRawADC()));
        time::wait(100);
    }
}