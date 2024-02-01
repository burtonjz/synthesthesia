#ifndef __POLY_OSCILLATOR_HPP_
#define __POLY_OSCILLATOR_HPP_

#include "config.hpp"
#include "Module.hpp"
#include "Oscillator.hpp"
#include "IO.hpp"
#include "portInfo.hpp"
#include "Note.hpp"

#include <boost/container/flat_map.hpp>
#include <array>
#include <utility>
#include <cstddef>
#include <array>

class PolyOscillator : public Module {
protected:
    boost::container::flat_map<uint8_t,Oscillator> oscillator_ ;
    IO<float,AudioPorts::AUDIO_N> outputBuffer_ ;
    static std::array<ParameterType, 5> control_params_ ;

public:
    PolyOscillator(const double* sampleRate);

    /**
     * @brief returns pointer to vector of Oscillator Control Ports
    */
    static std::pair<const ParameterType*, size_t> getControlPorts();

    /**
     * @brief Activate the static components of the module. Must be called before real-time processing begins
     * 
     * Create all oscillator objects
    */
    static void static_activate();

    /**
     * @brief Activate the module. Must be called before real-time processing begins
    */
    void activate();
    
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
     * @brief create child oscillator
     * 
     * @param
    */
    void createChildOscillator(uint8_t midi_note, const Note note);
    /**
     * @brief set child oscillator output buffer to same as this
    */
    void updateChildOutputBuffers(uint8_t index);

};


#endif // __POLY_OSCILLATOR_HPP_