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

    // Setup pins for Strain Gauges
    IO::ADC& adc0 = IO::getADC<IO::Pin::PA_0>();
    IO::ADC& adc1 = IO::getADC<IO::Pin::PA_0>();
    IO::ADC& adc2 = IO::getADC<IO::Pin::PA_0>();
    IO::ADC& adc3 = IO::getADC<IO::Pin::PA_0>();

    time::wait(500);

    // Create an instance for each strain gauge
    SGM::DEV::StrainGauge gauges[NUM_GAUGES] = {
        SGM::DEV::StrainGauge(adc0, convert),
        SGM::DEV::StrainGauge(adc1, convert),
        SGM::DEV::StrainGauge(adc2, convert),
        SGM::DEV::StrainGauge(adc3, convert),
    };

    SGM::SGM sgm = SGM::SGM(gauges);

    uart.printf("Starting thermistor test\r\n");

    while (1) {
        uart.printf("Strain: %dunit\r\n",
                    static_cast<int>(gauges[0].getProcessedData()));
        uart.printf("Voltage: %dV\r\n",
                    static_cast<int>(gauges[0].getRawADC()));
        time::wait(100);
    }
}
