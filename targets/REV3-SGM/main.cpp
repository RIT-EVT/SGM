#include <EVT/io/UART.hpp>
#include <EVT/io/pin.hpp>
#include <EVT/io/ADC.hpp>
#include <EVT/manager.hpp>
#include <EVT/dev/MCUTimer.hpp>
#include <EVT/utils/time.hpp>
#include <EVT/io/CANOpenMacros.hpp>
#include <EVT/io/CANopen.hpp>
#include <EVT/utils/log.hpp>
#include <SGM.hpp>

namespace IO = EVT::core::IO;
namespace log = EVT::core::log;
namespace time = EVT::core::time;
namespace DEV = EVT::core::DEV;

/**
 * Note, this conversion function is just to show off how a conversion
 * function will look like. In actuality, this will be determined through
 * testing/reading the thermistor datasheet.
 */


uint32_t convert(uint32_t voltage) {
    return voltage / 2;
}

///////////////////////////////////////////////////////////////////////////////
// EVT-core CAN callback and CAN setup. This will include logic to set
// aside CANopen messages into a specific queue
///////////////////////////////////////////////////////////////////////////////

/**
 * Interrupt handler to get CAN messages. A function pointer to this function
 * will be passed to the EVT-core CAN interface which will in turn call this
 * function each time a new CAN message comes in.
 *
 * NOTE: For this sample, every non-extended (so 11 bit CAN IDs) will be
 * assumed to be intended to be passed as a CANopen message.
 *
 * @param message[in] The passed in CAN message that was read.
 */

// create a can interrupt handler
void canInterrupt(IO::CANMessage& message, void* priv) {
    EVT::core::types::FixedQueue<CANOPEN_QUEUE_SIZE, IO::CANMessage>* queue =
        (EVT::core::types::FixedQueue<CANOPEN_QUEUE_SIZE, IO::CANMessage>*) priv;

    // Log raw received data
    log::LOGGER.log(log::Logger::LogLevel::DEBUG, "Got RAW message from %X of length %d with data: ", message.getId(), message.getDataLength());

    uint8_t* data = message.getPayload();
    for (int i = 0; i < message.getDataLength(); i++) {
        log::LOGGER.log(log::Logger::LogLevel::DEBUG, "%X ", *data);
        data++;
    }

    if (queue != nullptr)
        queue->append(message);
}

int main() {
    // Initialize system
    EVT::core::platform::init();

    // Setup UART
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);
    log::LOGGER.setUART(&uart);

    // Setup pins for Strain Gauges
    IO::ADC& adc0 = IO::getADC<IO::Pin::PA_0>();
    IO::ADC& adc1 = IO::getADC<IO::Pin::PA_0>();
    IO::ADC& adc2 = IO::getADC<IO::Pin::PA_0>();
    IO::ADC& adc3 = IO::getADC<IO::Pin::PA_0>();

    time::wait(500);

    // Create an instance for each strain gauge
    SGM::DEV::strainGauge strainGauge0 = SGM::DEV::strainGauge(adc0, convert);
    SGM::DEV::strainGauge strainGauge1 = SGM::DEV::strainGauge(adc1, convert);
    SGM::DEV::strainGauge strainGauge2 = SGM::DEV::strainGauge(adc2, convert);
    SGM::DEV::strainGauge strainGauge3 = SGM::DEV::strainGauge(adc3, convert);

    //Testing the Strain Gauge:
    while(1) {
        uart.printf("Processed stress: %dmC\r\n",
                    static_cast<int>(strainGauge0.getProcessedData()));
        uart.printf("Voltage: %dV\r\n",
                    static_cast<int>(strainGauge0.getRawADC()));
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

    /* Update with appropriate timer */

    DEV::Timer& timer = DEV::getTimer<DEV::MCUTimer::Timer2>(100);

    ///////////////////////////////////////////////////////////////////////////
    // Setup CAN configuration, this handles making drivers, applying settings.
    // And generally creating the CANopen stack node which is the interface
    // between the application (the code we write) and the physical CAN network
    ///////////////////////////////////////////////////////////////////////////

    // Will store CANopen messages that will be populated by the EVT-core CAN
    // interrupt
    EVT::core::types::FixedQueue<CANOPEN_QUEUE_SIZE, IO::CANMessage> canOpenQueue;


    /* Update with appropriate CAN pins */

    // Initialize CAN, add an IRQ which will add messages to the queue above
    IO::CAN& can = IO::getCAN<IO::Pin::PA_12, IO::Pin::PA_11>();
    can.addIRQHandler(canInterrupt, reinterpret_cast<void*>(&canOpenQueue));

    // Attempt to join the CAN network
    IO::CAN::CANStatus result = can.connect();

    // Check to see if the device is connected to the CAN network
    if (result != IO::CAN::CANStatus::OK) {
        log::LOGGER.log(log::Logger::LogLevel::ERROR, "Failed to connect to CAN network\r\n");
        return 1;
    } else {
        log::LOGGER.log(log::Logger::LogLevel::INFO, "Connected to CAN network\r\n");
    }

    // Reserved memory for CANopen stack usage
    uint8_t sdoBuffer[CO_SSDO_N * CO_SDO_BUF_BYTE];
    CO_TMR_MEM appTmrMem[16];

    // Reserve driver variables
    CO_IF_DRV canStackDriver;

    CO_IF_CAN_DRV canDriver;
    CO_IF_TIMER_DRV timerDriver;
    CO_IF_NVM_DRV nvmDriver;

    CO_NODE canNode;

    // Initialize all the CANOpen drivers.
    IO::initializeCANopenDriver(&canOpenQueue, &can, &timer, &canStackDriver, &nvmDriver, &timerDriver, &canDriver);

    // Initialize the CANOpen node we are using.
    IO::initializeCANopenNode(&canNode, &tmu, &canStackDriver, sdoBuffer, appTmrMem);

    // Set the node to operational mode
    CONmtSetMode(&canNode.Nmt, CO_OPERATIONAL);

    time::wait(500);

}
