/**
 * This is a basic sample of using the UART module. The program provides a
 * basic echo functionality where the uart will write back whatever the user
 * enters.
 */

#include <EVT/io/UART.hpp>
#include <EVT/io/pin.hpp>
#include <EVT/io/ADC.hpp>
#include <EVT/manager.hpp>
#include <SGM.hpp>

/**
 * Note, this conversion function is just to show off how a conversion
 * function will look like. In actuality, this will be determined through
 * testing/reading the thermistor datasheet.
 */


uint32_t convert(uint32_t voltage) {
    return voltage / 2;
}

namespace IO = EVT::core::IO;
namespace time = EVT::core::time;

int main() {
    // Initialize system
    EVT::core::platform::init();

    // Setup UART
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    // Set pin for Strain Gauge
    IO::ADC& adc_0 = IO::getADC<IO::Pin::PA_0>();

    time::wait(500);

    SGM::SGM strainGauge_0(adc_0, convert);


    // String to store user input
    char buf[100];

    //Testing the Strain Gauge:
    while(1) {
        uart.printf("Processed stress: %dmC\r\n",
                    static_cast<int>(strainGauge_0.getProcessedData()));
        uart.printf("Voltage: %dV\r\n",
                    static_cast<int>(strainGauge_0.getRawADC()));
        time::wait(100);
    }
    /**
     *  while (1) {
     *      // Read user input
     *      uart.printf("Enter message: ");
     *      uart.gets(buf, 100);
     *      uart.printf("\n\recho: %s\n\r", buf);
     *  }
     */
}
