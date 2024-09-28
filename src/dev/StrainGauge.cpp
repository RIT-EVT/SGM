#include <dev/StrainGauge.hpp>

namespace IO = EVT::core::IO;

namespace SGM::DEV {

StrainGauge::StrainGauge(EVT::core::IO::ADC& adc, uint32_t (*conversion)(uint32_t)) : adc(adc), conversion(conversion) {}

uint32_t StrainGauge::getForce() {
    return conversion(adc.readRaw());
}

uint32_t StrainGauge::getRawADC() {
    return adc.readRaw();
}
};// namespace SGM::DEV
