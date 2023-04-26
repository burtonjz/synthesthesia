#include <array>
#include <boost/container/flat_map.hpp>

#include <lv2.h>
#include <lv2/atom/atom.h>
#include <lv2/urid/urid.h>

#include "port-info.hpp"
#include "linear-fader.hpp"
#include "panner.hpp"
#include "filter.hpp"
#include "modulator-lfo.hpp"
#include "modulator-env.hpp"
#include "key.hpp"


#define SYNTH_URI "https://github.com/burtonjz/lv2-plugins-dev/synthesthesia"
#define N_OSCILLATORS 3

struct Urids {
    LV2_URID midi_MidiEvent;
};

/*
===========================================================
=============== Synth Class Definition ====================
===========================================================
*/ 
class Synthesthesia {
private:
    std::array<const LV2_Atom_Sequence*,MIDI_N> midi_in ;
    std::array<float*,AUDIO_OUT_N> audio_out;
    std::array<const float*, CTRL_N> control;
    std::array<float, CTRL_N> ctrl_values;
    LV2_URID_Map* map;
    Urids urids;
    double rate;
    double pos;
    boost::container::flat_map<uint8_t, Key> key;

    std::array<LinearFader<float>,N_OSCILLATORS> ctrl_osc_gain;
    std::array<double,N_OSCILLATORS> ctrl_osc_detune;
    std::array<Panner,N_OSCILLATORS> ctrl_osc_pan;

    // Filters
    LowPassFilter lpf1;

    // Modulators
    LFO lfo1;
    ADSREnvelope env1;

public:
    Synthesthesia (const double sample_rate, const LV2_Feature *const *features);
    void connectPort (const uint32_t port, void* data);
    void activate();
    void run (const uint32_t sample_count);
    void deactivate();

private:
    void play (const uint32_t start, const uint32_t end);
    std::array<OscillatorConfig,N_OSCILLATORS> configure_oscillators(); 
    void normalize_osc_gain();
};