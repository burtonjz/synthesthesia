#ifndef __POLY_OSCILLATOR_HPP_
#define __POLY_OSCILLATOR_HPP_

#include "config.hpp"
#include "Module.hpp"
#include "Oscillator.hpp"
#include "IO.hpp"

#include <boost/container/flat_map.hpp>
#include <cstddef>
#include <array>

class PolyOscillator : public Module {
protected:
    boost::container::flat_map<uint8_t,Oscillator> oscillator_ ;
    IO<float,AUDIO_OUT_N> outputBuffer_ ;

public:
    PolyOscillator(const double* sampleRate);

    /**
     * @brief gets Oscillator Control Port count
    */
    static uint32_t getNumControlPorts();

    /**
     * @brief returns array of Oscillator Control Ports
    */
    static std::array<ParameterType,6> getControlPorts();

    /**
     * @brief Activate the module. Must be called before real-time processing begins
     * 
     * Create all oscillator objects
    */
    void activate() override ;

    /**
     * @brief set output buffer for specified channel
    */
    void setOutputBuffer(float* buffer, size_t channel) override ;

    /**
     * @brief process samples through the module
     * 
     * @param idx frame to process
    */
    void processSample(uint32_t idx);

    /**
     * @brief increment state of polyphonic oscillator
    */
    void tick() override ;

private:
    /**
     * @brief update oscillators with data from KeyboardController
     * 
     * @param note_info NoteInfo struct from the KeyboardController
    */
    void updateOscillators();

    /**
     * @brief set child oscillator output buffer to same as this
    */
    void updateChildOutputBuffers(uint8_t index);

};


#endif // __POLY_OSCILLATOR_HPP_