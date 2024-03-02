#ifndef __FILTER_HPP_
#define __FILTER_HPP_


#include "config.hpp"
#include "Module.hpp"
#include "IO.hpp"
#include "portInfo.hpp"
#include "FilterType.hpp"

#include <cstdint>
#include <array>
#include <utility>


/**
 * @brief 2nd Order IIR Filter
*/
class Filter : public Module {
private:
    IO<float,AudioPorts::AUDIO_N> buffer_ ; 
    FilterType type_ ;

    double a1, a2 ; // feedback coefficients
    double b0, b1, b2 ; // feedforward coefficients

public:
    /**
     * @brief construct an empty Filter module. Must be activated
    */
    Filter();

    /**
     * @brief construct a Filter module
    */
    Filter(const double* sampleRate);

    /**
     * @brief returns a pointer to a vector of all Parameter controlPorts
    */
    std::pair<const ParameterType*, size_t> getControlPorts();

    /**
     * @brief activate the oscillator module, sets the sample rate
    */
    void activate(const double* sampleRate);

    /**
     * @brief set output buffer for specified channel
    */
    void setOutputBuffer(float* buffer, size_t channel);

    /**
     * @brief get filter type
    */
    const FilterType getFilterType() ;

    /**
     * @brief set filter type
     * 
     * @param typ FilterType
    */
    void setFilterType(FilterType typ);

private:
    /**
     * @brief calculates 2nd order coefficients for filter based on 
    */
    void calculate_coefficients();

    /**
     * @brief passes all values in buffer through the filter
    */
    void process();

};

#endif // __FILTER_HPP_