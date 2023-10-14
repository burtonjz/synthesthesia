#ifndef __POLY_OSCILLATOR_HPP_
#define __POLY_OSCILLATOR_HPP_

#include "config.hpp"
#include "Module.hpp"
#include "Oscillator.hpp"
#include "KeyboardController.hpp"
#include "IO.hpp"

#include <boost/container/flat_map.hpp>
#include <cstddef>

class PolyOscillator : public Module {
protected:
    boost::container::flat_map<uint8_t,Oscillator> oscillator_ ;
    KeyboardController* keyboard_controller_ptr_ ;
    IO<float,AUDIO_OUT_N> outputBuffer_ ;

public:
    PolyOscillator(const double* sampleRate);

    /**
     * @brief Activate the module. Must be called before real-time processing begins
     * 
     * Create all oscillator objects
    */
    void activate() override ;

    /**
     * @brief Activate the module. Must be called before real-time processing begins
     * 
     * Create all oscillator objects and connect keyboard controller
    */
    void activate(KeyboardController* controller);

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
     * @param active_notes active notes from the KeyboardController
    */
    void updateOscillators(const boost::container::flat_map<uint8_t,NoteInfo> active_notes);

    /**
     * @brief set child oscillator output buffer to same as this
    */
    void updateChildOutputBuffers(uint8_t index);

};


#endif // __POLY_OSCILLATOR_HPP_