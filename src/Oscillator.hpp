#ifndef __OSCILLATOR_HPP_
#define __OSCILLATOR_HPP_

#include "config.hpp"
#include "Module.hpp"
#include "IO.hpp"

#include <array>

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

    /**
     * @brief Instantiate Oscillator module. Must be called before real-time processing begins
    */
    void activate() override;

    /**
     * @brief set input buffer for specified channel
    */
    void setOutputBuffer(float* buffer, size_t channel) override;

    /**
     * @brief validate whether all outputBuffer are set
    */
    bool isOutputBufferSet();

    /**
     * @brief process samples through the module
     * 
     * @param idx frame to process
    */
    void processSample(uint32_t idx);

    /**
     * @brief increment oscillator for next sample
    */
    void tick();

private:
    /**
     * @brief get value from lookup table
    */
    double wavetableLookup(float index);


};

#endif // __OSCILLATOR_HPP_