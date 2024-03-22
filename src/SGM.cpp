#include <SGM.hpp>

namespace IO = EVT::core::IO;

namespace SGM {
    SGM::SGM(EVT::core::IO::ADC& adc, uint32_t (*conversion)(uint32_t)) :   adc(adc),
                                                                            conversion(conversion) {
    }

    uint32_t SGM::getProcessedData() {
        return conversion(adc.readRaw());
    }

    uint32_t SGM::getRawADC() {
        return adc.readRaw();
    }

}
