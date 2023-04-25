#include <lv2.h>
#include <lv2/atom/atom.h>
#include <lv2/urid/urid.h>
#include <lv2/midi/midi.h>
#include <lv2/core/lv2_util.h>
#include <lv2/atom/util.h>

#include <array>
#include <boost/container/flat_map.hpp>
#include <stdexcept>
#include <iostream>

#include "config.hpp"
#include "port-info.hpp"
#include "array-math.hpp"
#include "linear-fader.hpp"
#include "limit.hpp"
#include "panner.hpp"
#include "cfg-oscillator.hpp"
#include "modulator-lfo.hpp"
#include "modulator-env.hpp"
#include "filter-type.hpp"
#include "filter.hpp"
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

Synthesthesia::Synthesthesia(const double sample_rate, const LV2_Feature *const *features):
    midi_in{nullptr},
    audio_out{nullptr},
    control{nullptr},
    ctrl_values{0},
    map(nullptr),
    rate(sample_rate),
    pos(0.0),
    key(),
    ctrl_osc_gain{LinearFader<float>(0.0f)},
    ctrl_osc_detune{0.0},
    ctrl_osc_pan{Panner()},
    lpf1(),
    lfo1(),
    env1()
{
    const char* missing = lv2_features_query(
        features,
        LV2_URID__map, &map, true,
        NULL
    );

    if (missing) throw std::invalid_argument("Feature map not provided by the host. Aborting.");

    urids.midi_MidiEvent = map->map(map->handle, LV2_MIDI__MidiEvent);
}

void Synthesthesia::connectPort(const uint32_t port, void* data){
    if (port<MIDI_N){
        std::cout << "[" << SYNTH_URI << "]: Connected Port "  << port << "." << std::endl;
        midi_in[port] = static_cast <const LV2_Atom_Sequence*> (data);
    } else if (port < MIDI_N + AUDIO_OUT_N){
        std::cout << "[" << SYNTH_URI << "]: Connected Port "  << port << "." << std::endl;
        audio_out[port - MIDI_N] = static_cast <float*> (data);
    } else {
        std::cout << "[" << SYNTH_URI << "]: Connected Port "  << port << "." << std::endl;
        control[port - MIDI_N - AUDIO_OUT_N] = static_cast <const float*> (data);
    }
}

void Synthesthesia::play (const uint32_t start, const uint32_t end){
    for (uint32_t i = start; i < end; ++i){
        std::array<float,N_OSCILLATORS> osc_samps = {};
        std::array<float,N_CHANNELS> out = {};

        // loop through active keys and append to output
        for (auto it = key.begin(); it < key.end(); ){
            if (it->second.isOn()){;
                array_add<float,N_OSCILLATORS>(osc_samps,it->second.get_sample());
                it->second.tick();
                ++it;
            } else {
                it = key.erase(it);
            }; 
        }

        // apply panning (for each oscillator, get the pan weights and multiply them by that oscillator's sample value)
        std::array<float,N_CHANNELS> pan_vals = {};
        for(int j = 0; j < N_OSCILLATORS; ++j){
            pan_vals = ctrl_osc_pan[j].get();
            array_multiply<float,2>(pan_vals,osc_samps[j]);
            array_add<float,2>(out,pan_vals);
        }

        // apply filter
        lpf1.tick(out);
        if(lpf1.is_active()){
            out = lpf1.get_sample();
        }

        // assign to audio_out
        for(int j = 0; j < N_CHANNELS; ++j){
            audio_out[j][i] = limit<float>(out[j],-1.0f,1.0f);
        }
        
        // tick components
        for(int i = 0; i < N_OSCILLATORS; ++i){
            ctrl_osc_gain[i].tick();
            ctrl_osc_pan[i].tick();
        }
        
        if(lfo1.get_is_active()) lfo1.tick();
    }
}

std::array<OscillatorConfig,N_OSCILLATORS> Synthesthesia::configure_oscillators(){
    std::array<OscillatorConfig,N_OSCILLATORS> osc_configs;
    for(int i = 0; i < N_OSCILLATORS; ++i){
        osc_configs[i] = {
            ctrl_values[OSC_GET_STATUS(i)] != 0.0,
            static_cast<Waveform> (ctrl_values[OSC_GET_WAVEFORM(i)]),
            &lfo1, // freq modulator
            &env1, // amp modulator
            nullptr,  // phase modulator
            &ctrl_osc_gain[i],
            ctrl_osc_detune[i]
        };
        // TODO: add connection logic for freq/amp/phase
    }
    return osc_configs;
}

void Synthesthesia::normalize_osc_gain(){
    // whenever an oscillator gain control is changed
    // normalize sum of gain to be 1.0
    float gain_sum = 0;
    for(int i = 0; i < N_OSCILLATORS; ++i){
        if(ctrl_values[OSC_GET_STATUS(i)] != 0.0){
            gain_sum += ctrl_values[OSC_GET_GAIN(i)];
        }
    }
    if(gain_sum > 1.0){
        for(int i = 0; i < N_OSCILLATORS; ++i){
            ctrl_osc_gain[i].set(ctrl_values[OSC_GET_GAIN(i)]/gain_sum, 0.01 * rate);
        }
    } else {
        for(int i = 0; i < N_OSCILLATORS; ++i){
            ctrl_osc_gain[i].set(ctrl_values[OSC_GET_GAIN(i)], 0.01 * rate);
        }
    }
}

void Synthesthesia::run(const uint32_t sample_count)
{
    // make sure all ports are connected
    for (int i = 0; i < MIDI_N; ++i){
        if (!midi_in[i]) return;
    }

    for (int i = 0; i < AUDIO_OUT_N; ++i){
        if (!audio_out[i]) return;
    }

    for (int i = 0; i < CTRL_N; ++i){
        if (!control[i]) return;
        if (*control[i] != ctrl_values[i]){
            ctrl_values[i] = limit<float> (*control[i], 
            ctrlLimits[i].first, ctrlLimits[i].second);
            switch(i){
            case CTRL_OSC1_STATUS:
            case CTRL_OSC1_GAIN:
            case CTRL_OSC2_STATUS:
            case CTRL_OSC2_GAIN:
            case CTRL_OSC3_STATUS:
            case CTRL_OSC3_GAIN:
                normalize_osc_gain();
                break;
            case CTRL_OSC1_DETUNE:
                ctrl_osc_detune[0] = ctrl_values[i];
                break;
            case CTRL_OSC2_DETUNE:
                ctrl_osc_detune[1] = ctrl_values[i];
                break;
            case CTRL_OSC3_DETUNE:
                ctrl_osc_detune[2] = ctrl_values[i];
                break;
            case CTRL_OSC1_PAN:
                ctrl_osc_pan[0].set(ctrl_values[i], 0.01 * rate);
                break;
            case CTRL_OSC2_PAN:
                ctrl_osc_pan[1].set(ctrl_values[i], 0.01 * rate);
                break;
            case CTRL_OSC3_PAN:
                ctrl_osc_pan[2].set(ctrl_values[i], 0.01 * rate);
                break;
            case CTRL_ENV1_ATTACK:
            case CTRL_ENV1_DECAY:
            case CTRL_ENV1_SUSTAIN:
            case CTRL_ENV1_RELEASE:
                env1.set_adsr({
                        ctrl_values[CTRL_ENV1_ATTACK],
                        ctrl_values[CTRL_ENV1_DECAY],
                        ctrl_values[CTRL_ENV1_SUSTAIN],
                        ctrl_values[CTRL_ENV1_RELEASE]
                });
                break;
            case CTRL_LFO1_STATUS:
                lfo1.set_is_active(ctrl_values[i] != 0.0);
                break;
            case CTRL_LFO1_FREQ:
                lfo1.set_freq(static_cast<double> (ctrl_values[i]));
                break;
            case CTRL_LFO1_WAVEFORM:
                lfo1.set_waveform(static_cast<Waveform> (ctrl_values[i]));
                break;
            case CTRL_LFO1_DEPTH:
                lfo1.set_depth(ctrl_values[i]);
                break;
            case CTRL_FILTER1_TYPE:
                lpf1.set_type(static_cast<FilterType> (ctrl_values[i]));
                break;
            case CTRL_FILTER1_FREQ:
                lpf1.set_cutoff_freq(ctrl_values[i]);
            }
        }
    }

    /* 
    get incoming midi messages
    within an atom each message is stored sequentially after the header
    */
    uint32_t last_frame = 0;
    LV2_ATOM_SEQUENCE_FOREACH(midi_in[0], ev){
        // play frames until event
        const uint32_t frame = ev->time.frames;
        play(last_frame, frame);
        last_frame = frame;
        if (ev->body.type == urids.midi_MidiEvent){
            const uint8_t* const msg = reinterpret_cast <const uint8_t*> (ev + 1); // within frame, move to data block
            const uint8_t typ = lv2_midi_message_type(msg);
            std::array<OscillatorConfig,N_OSCILLATORS> osc_configs;

            switch (typ){
            case LV2_MIDI_MSG_NOTE_ON:
                osc_configs = configure_oscillators();
                key[msg[1] & 0x7f].press(
                    osc_configs, // oscillator configs
                    msg[1], // note
                    msg[2]
                );
                break;

            case LV2_MIDI_MSG_NOTE_OFF:
                key[msg[1] & 0x7f].release(msg[1]);
                break;

            case LV2_MIDI_MSG_CONTROLLER:
                if (msg[1] == LV2_MIDI_CTL_ALL_NOTES_OFF){
                    for (auto& k : key) k.second.release();
                }
                else if (msg[1] == LV2_MIDI_CTL_ALL_SOUNDS_OFF){
                    for (auto& k : key) k.second.mute();
                } 
                break;
            
            default:
                break;
            }
        }
    }

    // play remaining frames
    play(last_frame, sample_count);
}

void Synthesthesia::activate(){
};

void Synthesthesia::deactivate(){
};

/*
===========================================================
=============== internal core methods =====================
===========================================================
*/ 
static LV2_Handle instantiate(const struct LV2_Descriptor *descriptor, double sample_rate, const char *bundle_path, const LV2_Feature *const *features)
{
    std::cout << "[" << SYNTH_URI << "]: Instantiating plugin." << std::endl;

    Synthesthesia* m = nullptr;
    try {
        m = new Synthesthesia(sample_rate, features);
    } 
    catch(const std::invalid_argument& ia){
        std::cerr << ia.what() << std::endl;
        return nullptr;
    }
    catch(const std::bad_alloc& ba){
        std::cerr << "Failed to allocate memory. Aborting." << std::endl;
        return nullptr;
    }
    
    std::cout << "[" << SYNTH_URI << "]: Plugin Instantiated." << std::endl;
    return m;
}

static void connect_port(LV2_Handle instance, uint32_t port, void *data){
    std::cout << "[" << SYNTH_URI << "]: Attempting to connect port " << port << "." << std::endl;
    Synthesthesia* m = static_cast <Synthesthesia*> (instance);
    if (m) m->connectPort (port, data);
}

static void activate(LV2_Handle instance){
    Synthesthesia* m = static_cast <Synthesthesia*> (instance);
    m->activate();
}

static void run(LV2_Handle instance, uint32_t sample_count){
    Synthesthesia* m = static_cast <Synthesthesia*> (instance);

    if (m) m->run (sample_count);
}

static void deactivate (LV2_Handle instance){
    Synthesthesia* m = static_cast <Synthesthesia*> (instance);
    m->deactivate();
}

static void cleanup (LV2_Handle instance){
    Synthesthesia* m = static_cast <Synthesthesia*> (instance);
    if (m) delete m;
}

static const void* extension_data(const char *uri){
    return NULL;
}

// descriptor
static LV2_Descriptor const descriptor = {
    SYNTH_URI,
    instantiate,
    connect_port,
    activate,
    run,
    deactivate,
    cleanup,
    extension_data
};

// interface
LV2_SYMBOL_EXPORT const LV2_Descriptor* lv2_descriptor(uint32_t index){
    if (index == 0) return &descriptor;
    return NULL;
};