#ifndef __OSCILLATOR_HPP_
#define __OSCILLATOR_HPP_

#include "config.hpp"
#include "Module.hpp"
#include "IO.hpp"
#include "portInfo.hpp"

#include <array>
#include <utility>
#include <cstdint>

/**
 * @brief basic Oscillator Module. Parameterized input, no output
 * 
 * An Oscillator class has no input buffers, audio output buffers
*/
class Oscillator : public Module {
protected:
    static std::array<ParameterType, 0> control_params_ ;
    static std::array<ParameterType, 5> modulatable_params_ ;

    IO<float,AudioPorts::AUDIO_N> outputBuffer_ ;
    double phase_ ;
    double increment_ ;

public:
    /**
     * @brief construct an Oscillator module.
    */
    Oscillator(const double* sampleRate);

    /**
     * @brief construct an empty Oscillator module. Must be activated.
    */
    Oscillator();

    /**
     * @brief returns pointer to array of Oscillator Control Ports
    */
    static std::pair<const ParameterType*, size_t> getControlPorts();

    /**
     * @brief returns modulatable parameters
    */
    static std::pair<const ParameterType*, size_t> getModulatableParameters();

    /**
     * @brief construct a child Oscillator module
     * 
     * @param sampleRate synth sample rate
     * @param params reference to parent's parameterController
     * 
     * Child oscillators hold reference's to their parent's parameterController
    */
    Oscillator(const double* sampleRate, ParameterController& params);
    
    /**
     * @brief Instantiate Oscillator module. Must be called if oscillator constructed without sample rate
    */
    void activate(const double* sampleRate);

    /**
     * @brief set output buffer for specified channel
    */
    void setOutputBuffer(float* buffer, size_t channel);

    /**
     * @brief process samples through the module.
     * 
     * This saves the sample from getSample to the output buffer (if defined) and pans it appropriately
     * 
     * @param idx frame to process
    */
    void processSample(uint32_t idx);


    /**
     * @brief gets the sample value at oscillator's current position.
     * 
     * Does not save output to outputBuffer. This is primarily used external to the module in the case of LFOs.
    */
    double getSample() const ;

    /**
     * @brief increment oscillator for next sample
    */
    void tick() override ;

private:
    /**
     * @brief pans the sample and adds to respective output buffers
     * 
     * @param sample_value calculated sample value
     * @param idx output buffer index
    */
    void panSample(double sample_value, uint32_t idx);

    /**
     * @brief validate whether all outputBuffer are set
    */
    bool isOutputBufferSet();
};

#endif // __OSCILLATOR_HPP_