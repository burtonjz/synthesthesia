#ifndef __OSCILLATOR_HPP_
#define __OSCILLATOR_HPP_

#include "config.hpp"
#include "Module.hpp"
#include "IO.hpp"

#include <array>
#include <utility>


/**
 * @brief basic Oscillator Module. Parameterized input, no output
 * 
 * An Oscillator class has no input buffers, audio output buffers
*/
class Oscillator : public Module {
protected:
    IO<float,AUDIO_OUT_N> outputBuffer_ ;
    double phase_ ;
    double increment_ ;

public:
    /**
     * @brief instantiates an empty Oscillator module.
    */
    Oscillator(const double* sampleRate);
    Oscillator();
    
    /**
     * @brief Instantiate Oscillator module. Must be called before real-time processing begins
    */
    void activate() override;

    /**
     * @brief set output buffer for specified channel
    */
    void setOutputBuffer(float* buffer, size_t channel) override;

    /**
     * @brief process samples through the module
     * 
     * @param idx frame to process
    */
    void processSample(uint32_t idx);

    /**
     * @brief increment oscillator for next sample
    */
    void tick() override ;

    /**
     * @brief sets parameter value
    */


private:
    /**
     * @brief get value from lookup table
    */
    double wavetableLookup(float index);

    /**
     * @brief return pan values
    */
    std::pair<double,double> panSample(double sample_value);

    /**
     * @brief validate whether all outputBuffer are set
    */
    bool isOutputBufferSet();
    
};

#endif // __OSCILLATOR_HPP_