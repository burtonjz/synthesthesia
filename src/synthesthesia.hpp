#include <array>
#include <boost/container/flat_map.hpp>
#include <tuple>

#include <lv2.h>
#include <lv2/atom/atom.h>
#include <lv2/urid/urid.h>

#include "config.hpp"
#include "port-info.hpp"
#include "linear-fader.hpp"
#include "panner.hpp"
#include "filter.hpp"
#include "modulator-lfo.hpp"
#include "modulator-env.hpp"
#include "key.hpp"

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
    std::array<const float*, CTRL_OSC_N * N_OSCILLATORS> ctrl_osc;
    std::array<float, CTRL_OSC_N * N_OSCILLATORS> ctrl_osc_values;
    std::array<const float*, CTRL_ENV_N * N_ENVELOPES> ctrl_env;
    std::array<float, CTRL_ENV_N * N_ENVELOPES> ctrl_env_values;
    std::array<const float*, CTRL_LFO_N * N_LFOS> ctrl_lfo;
    std::array<float, CTRL_LFO_N * N_LFOS> ctrl_lfo_values;
    std::array<const float*, CTRL_FILTER_N * N_FILTERS> ctrl_filter;
    std::array<float, CTRL_FILTER_N * N_FILTERS> ctrl_filter_values;
    LV2_URID_Map* map;
    Urids urids;
    double rate;
    double pos;
    boost::container::flat_map<uint8_t, Key> key;

    std::array<LinearFader<float>,N_OSCILLATORS> ctrl_osc_gain;
    std::array<double,N_OSCILLATORS> ctrl_osc_detune;
    std::array<Panner,N_OSCILLATORS> ctrl_osc_pan;

    // Filters
    std::array<Filter,N_FILTERS> filter;

    // Modulators
    std::array<LFO,N_LFOS> lfo;
    std::array<ADSREnvelope,N_ENVELOPES> env;
    std::array<Modulator*,N_MODULATORS> modulator;

public:
    Synthesthesia (const double sample_rate, const LV2_Feature *const *features);

    // GETTERS/SETTERS
    Key* find_key(uint8_t i);
    std::tuple<KeyStatus,double,float> get_global_key_params() const;

    bool any_key_active() const;

    // hooks for internal LV2 core functions
    void connectPort (const uint32_t port, void* data);
    void activate();
    void run (const uint32_t sample_count);
    void deactivate();

private:
    void play (const uint32_t start, const uint32_t end);
    std::array<OscillatorConfig,N_OSCILLATORS> configure_oscillators(); 
    void normalize_osc_gain();
    void validate_and_update_ports();
    void update_filter_connections();
};