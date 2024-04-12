#ifndef strainGauge_STRAINGAUGE_HPP
#define strainGauge_STRAINGAUGE_HPP

#include <EVT/io/ADC.hpp>

namespace IO = EVT::core::IO;

namespace SGM::DEV {

class strainGauge {
public:
    /**
     * Create a new strain gauge which will use the provided ADC and
     * pass those values through the conversion to produce a strain/stress
     * reading.
     *
     * @param[in] adc The ADC to use to convert ADC values into strain/stress
     * @param[in] conversion The conversion function
     */
    explicit strainGauge(EVT::core::IO::ADC& adc, uint32_t (*conversion)(uint32_t));

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
    /// Function that processes raw ADC values into useful data
    uint32_t (*conversion)(uint32_t);
};

}// namespace SGM::DEV

#endif//strainGauge_STRAINGAUGE_HPP
