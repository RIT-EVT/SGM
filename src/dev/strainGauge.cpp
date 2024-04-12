#include <dev/strainGauge.hpp>

namespace IO = EVT::core::IO;

namespace SGM::DEV {
strainGauge::strainGauge(EVT::core::IO::ADC& adc, uint32_t (*conversion)(uint32_t)) : adc(adc), conversion(conversion) {}

uint32_t strainGauge::getProcessedData() {
    return conversion(adc.readRaw());
}

uint32_t strainGauge::getRawADC() {
    return adc.readRaw();
}
};// namespace SGM::DEV