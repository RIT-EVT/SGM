#pragma once
#include <EVT/io/ADC.hpp>
#include <EVT/io/CANDevice.hpp>
#include <EVT/io/CANOpenMacros.hpp>
#include <co_core.h>
#include <dev/StrainGauge.hpp>

#define NUM_GAUGES 4

namespace IO = EVT::core::IO;

namespace SGM {

/**
 * Measures strain/stress on component using reading from ADC.
 */
class SGM : public CANDevice {
public:
    /**
     * Constructor takes an array of 4 strain gauges
     *
     * @param[in] gauges the strain gauges
     */
    explicit SGM(DEV::StrainGauge* gauges);

    /**
     * Gets the object dictionary
     *
     * @return an object dictionary
     */
    CO_OBJ_T* getObjectDictionary() override;

    /**
     * The node ID used to identify the device on the CAN network.
     */
    static constexpr uint8_t NODE_ID = 8;

    /**
     * Updates the voltage values in the force array
     */
    void process();

    uint8_t getNodeID() override;
    uint8_t getNumElements() override;

private:
    /** Stores the 4 strain gauges */
    DEV::StrainGauge* gauges;

    /** Stores the 4 16-bit force values. */
    uint16_t force[NUM_GAUGES] = {};

    /**
     * Object Dictionary Size
     */
    static constexpr uint16_t OBJECT_DICTIONARY_SIZE = 43;

    /**
     * CAN Open object dictionary
     */
    CO_OBJ_T objectDictionary[OBJECT_DICTIONARY_SIZE + 1] = {
        MANDATORY_IDENTIFICATION_ENTRIES_1000_1014,
        HEARTBEAT_PRODUCER_1017(2000),
        IDENTITY_OBJECT_1018,
        SDO_CONFIGURATION_1200,

        // TPDO 0 CONFIGURATION
        TRANSMIT_PDO_SETTINGS_OBJECT_18XX(0x00, TRANSMIT_PDO_TRIGGER_TIMER, TRANSMIT_PDO_INHIBIT_TIME_DISABLE, 2000),

        // TPDO 0 MAPPING
        TRANSMIT_PDO_MAPPING_START_KEY_1AXX(0, 4),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x00, 1, PDO_MAPPING_UNSIGNED16),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x00, 2, PDO_MAPPING_UNSIGNED16),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x00, 3, PDO_MAPPING_UNSIGNED16),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x00, 4, PDO_MAPPING_UNSIGNED16),

        // TPDO 0 DATA LINKS
        DATA_LINK_START_KEY_21XX(0, 4),
        DATA_LINK_21XX(0x00, 1, CO_TUNSIGNED16, &force[0]),
        DATA_LINK_21XX(0x00, 2, CO_TUNSIGNED16, &force[1]),
        DATA_LINK_21XX(0x00, 3, CO_TUNSIGNED16, &force[2]),
        DATA_LINK_21XX(0x00, 4, CO_TUNSIGNED16, &force[3]),

        CO_OBJ_DICT_ENDMARK,
    };
};

}// namespace SGM
