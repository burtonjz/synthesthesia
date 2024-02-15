#ifndef __SYNTHESTHESIA_HPP_
#define __SYNTHESTHESIA_HPP_

#include "config.hpp"
#include "urids.hpp"
#include "portInfo.hpp"

#include "KeyboardController.hpp"
#include "ControlPortManager.hpp"

#include "PolyOscillator.hpp"
#include "ADSREnvelope.hpp"
#include "LFO.hpp"
#include "Detuner.hpp"
#include "ModulationChain.hpp"

#include <cstdint>
#include <array>

#include <lv2/lv2plug.in/ns/lv2core/lv2.h>
#include <lv2/lv2plug.in/ns/lv2core/lv2_util.h>
#include <lv2/lv2plug.in/ns/ext/atom/atom.h>
#include <lv2/lv2plug.in/ns/ext/urid/urid.h>
#include <lv2/lv2plug.in/ns/ext/buf-size/buf-size.h>

class Synthesthesia {
private:
    const LV2_Atom_Sequence* midi_in ;
    std::array<float*,AudioPorts::AUDIO_N> audio_out ;

    LV2_URID_Map* urid_map ;
    Urids urids;

    double sampleRate_ ;
    KeyboardController keyboardController_ ;
    ControlPortManager portManager_ ;
    std::array<PolyOscillator,1> oscillator_ ;
    
    ModulationChain osc_freq_mod_ ;

    ADSREnvelope envelope_ ;
    LFO lfo_ ;
    Detuner detuner_ ;

public:
    /**
     * @brief Construct a new Synthesthesia object
     * 
     * @param sample_rate host sample rate
     * @param features LV2 features array
     */
    Synthesthesia(const double sample_rate, const LV2_Feature *const *features);

    // hooks for internal LV2 core functions
    void connectPort(const uint32_t port, void* data);
    void activate();
    void run(const uint32_t sample_count);
    void deactivate();

private:

    /**
     * @brief processes a MidiEvent
     * 
     * @param ev LV2 Atom Event
    */
    void processMidi(LV2_Atom_Event* ev);

    /**
     * @brief plays the synthesizer for the given frames
     * 
     * @param start start frame
     * @param end end frame
    */
    void play(const uint32_t start, const uint32_t end);

    /**
     * @brief tick all components of synthesizer
     * 
     * @param time in seconds
    */
    void tick(double time);

    /**
     * @brief for each module, update parameters with host control port data
    */
    void updateControlPorts();

};

#endif // __SYNTHESTHESIA_HPP_