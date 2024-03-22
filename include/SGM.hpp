#pragma once
#ifndef EVT_STRAIN_GAUGE
#define EVT_STRAIN_GAUGE

#include <EVT/io/ADC.hpp>

namespace IO = EVT::core::IO;

namespace SGM {

/**
 * Measures strain/stress on component using reading from ADC.
 */
class SGM {
public:
    /**
     * Create a new strain gauge which will use the provided ADC and
     * pass those values through the conversion to produce a strain/stress
     * reading.
     *
     * @param[in] adc The ADC to use to convert ADC values into strain/stress
     * @param[in] conversion The conversion function
     */
    SGM(EVT::core::IO::ADC& adc, uint32_t (*conversion)(uint32_t));

    /**
     * Return the processed data of the strain gauge
     *
     * @return The conversion of the ADC value into useful stress data
     */
    uint32_t getProcessedData();

    /**
     *     Returns the raw ADC values
     *
     *     @return The raw values from the ADC
     */
    uint32_t getRawADC();
private:
    /// The ADC interface to read from
    EVT::core::IO::ADC& adc;
    /// Funtion that processes raw ADC values into useful data
    uint32_t (*conversion)(uint32_t);
};

}// namespace SGM

#endif