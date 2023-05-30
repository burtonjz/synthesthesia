#include "config.hpp"

#include <lv2/midi/midi.h>
#include <lv2/core/lv2_util.h>
#include <lv2/atom/util.h>

#include <array>
#include <iostream>
#include <stdexcept>

#include "synthesthesia.hpp"
#include "array-math.hpp"
#include "linear-fader.hpp"
#include "limit.hpp"
#include "cfg-oscillator.hpp"
#include "filter-type.hpp"


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
    lfo{LFO()},
    env{ADSREnvelope()}
{
    const char* missing = lv2_features_query(
        features,
        LV2_URID__map, &map, true,
        NULL
    );

    if (missing) throw std::invalid_argument("Feature map not provided by the host. Aborting.");

    urids.midi_MidiEvent = map->map(map->handle, LV2_MIDI__MidiEvent);

    // couple filter with this class (required for current implementation of envelope modulation)
    lpf1.set_synth_ptr(this);

    // initialize modulator pointer array
    for(int i = 0; i < N_ENVELOPES; ++i){
        modulator[i] = &env[i];
    }
    for(int i = 0; i < N_LFOS; ++i){
        modulator[N_ENVELOPES+i] = &lfo[i];
    }

}

Key* Synthesthesia::find_key(uint8_t index){
    auto it = key.find(index);
    if (it != key.end()) return &(it->second);
    else return nullptr;
};

/*
For a global filter to be modulated by an envelope, we need to find the key parameters in the following priority
KEY_PRESS -- return first pressed key (for attack/decay/sustain stages)
KEY_RELEASE -- find most recently released key (for release stage)
KEY_OFF
*/
std::tuple<KeyStatus,double,float> Synthesthesia::get_global_key_params() const {
    KeyStatus status = KEY_OFF;
    double time = 0.0;
    float start_level = 0.0f;
    for (auto it = key.begin(); it < key.end(); ){
        switch(it->second.get_status()){
        case KEY_PRESSED:
            return std::make_tuple(it->second.get_status(),it->second.get_time(),it->second.get_start_level());
        case KEY_RELEASED:
            if (status == KEY_OFF){
                status = KEY_RELEASED;
                time = it->second.get_time();
                start_level = it->second.get_start_level();
            } else if (time > it->second.get_time()){
                time = it->second.get_time();
                start_level = it->second.get_start_level();
            }
            break;
        }
        ++it;
    }
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
            array_multiply<float,N_CHANNELS>(pan_vals,osc_samps[j]);
            array_add<float,N_CHANNELS>(out,pan_vals);
        }

        // apply filter if active TODO: add filter connection logic (allow connect to some but not all oscillators?)
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
        
        for(int i = 0; i < N_LFOS; ++i){
            if(lfo[i].get_is_active()) lfo[i].tick();
        }
    }
}

std::array<OscillatorConfig,N_OSCILLATORS> Synthesthesia::configure_oscillators(){
    std::array<OscillatorConfig,N_OSCILLATORS> osc_configs;
    // determine modulator connections
    Modulator* freq_mod = nullptr;
    Modulator* amp_mod = nullptr;
    Modulator* phase_mod = nullptr;

    for(int i = 0; i < N_OSCILLATORS; ++i){
        freq_mod = nullptr;
        amp_mod = nullptr;
        phase_mod = nullptr;
        for(int j = 0; j < N_MODULATORS; ++j){
            if(modulator[j]->get_is_active()){
                if(MOD_IS_CONNECTED(modulator[j]->get_connections(),static_cast<ModConnection>(1 << (i*3)))) amp_mod = modulator[j];
                if(MOD_IS_CONNECTED(modulator[j]->get_connections(),static_cast<ModConnection>(2 << (i*3)))) freq_mod = modulator[j];
                if(MOD_IS_CONNECTED(modulator[j]->get_connections(),static_cast<ModConnection>(4 << (i*3)))) phase_mod = modulator[j];
            }
        }
        osc_configs[i] = {
            ctrl_values[OSC_GET_STATUS(i)] != 0.0,
            static_cast<Waveform> (ctrl_values[OSC_GET_WAVEFORM(i)]),
            freq_mod, // freq modulator
            amp_mod, // amp modulator
            phase_mod,  // phase modulator
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
            ctrl_osc_gain[i].set(ctrl_values[OSC_GET_GAIN(i)]/gain_sum, CTRL_FADER_WEIGHT * rate);
        }
    } else {
        for(int i = 0; i < N_OSCILLATORS; ++i){
            ctrl_osc_gain[i].set(ctrl_values[OSC_GET_GAIN(i)], CTRL_FADER_WEIGHT * rate);
        }
    }
}

void Synthesthesia::validate_and_update_ports(){
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
                ctrl_osc_pan[0].set(ctrl_values[i], CTRL_FADER_WEIGHT * rate);
                break;
            case CTRL_OSC2_PAN:
                ctrl_osc_pan[1].set(ctrl_values[i], CTRL_FADER_WEIGHT * rate);
                break;
            case CTRL_OSC3_PAN:
                ctrl_osc_pan[2].set(ctrl_values[i], CTRL_FADER_WEIGHT * rate);
                break;
            case CTRL_ENV1_CONNECTIONS:
                env[0].set_connections(ctrl_values[i]);
                break;
            case CTRL_ENV1_ATTACK:
            case CTRL_ENV1_DECAY:
            case CTRL_ENV1_SUSTAIN:
            case CTRL_ENV1_RELEASE:
                env[0].set_adsr({
                        ctrl_values[CTRL_ENV1_ATTACK],
                        ctrl_values[CTRL_ENV1_DECAY],
                        ctrl_values[CTRL_ENV1_SUSTAIN],
                        ctrl_values[CTRL_ENV1_RELEASE]
                });
                break;
            case CTRL_LFO1_CONNECTIONS:
                lfo[0].set_connections(ctrl_values[i]);
                break;
            case CTRL_LFO1_FREQ:
                lfo[0].set_freq(static_cast<double> (ctrl_values[i]));
                break;
            case CTRL_LFO1_WAVEFORM:
                lfo[0].set_waveform(static_cast<Waveform> (ctrl_values[i]));
                break;
            case CTRL_LFO1_DEPTH:
                lfo[0].set_depth(ctrl_values[i]);
                break;
            case CTRL_FILTER1_TYPE:
                lpf1.set_type(static_cast<FilterType> (ctrl_values[i]));
                break;
            case CTRL_FILTER1_FREQ:
                lpf1.set_cutoff_freq(ctrl_values[i]);
                break;
            case CTRL_FILTER1_RES:
                lpf1.set_q_factor(ctrl_values[i]);
                break;
            }
        }
    }
}

void Synthesthesia::update_filter_connections(){
    // check filter cutoff
    if (MOD_IS_CONNECTED(ctrl_values[CTRL_ENV1_CONNECTIONS],MOD_CONNECT_FILTER1_CUTOFF)){
        lpf1.connect_modulator_fc(&env[0]);
    } else if (MOD_IS_CONNECTED(ctrl_values[CTRL_LFO1_CONNECTIONS],MOD_CONNECT_FILTER1_CUTOFF)){
        lpf1.connect_modulator_fc(&lfo[0]);
    } else lpf1.disconnect_modulator_fc();

    // check filter resonance (Q Factor)
    if (MOD_IS_CONNECTED(ctrl_values[CTRL_ENV1_CONNECTIONS],MOD_CONNECT_FILTER1_Q)){
        lpf1.connect_modulator_q(&env[0]);
    } else if (MOD_IS_CONNECTED(ctrl_values[CTRL_LFO1_CONNECTIONS],MOD_CONNECT_FILTER1_Q)){
        lpf1.connect_modulator_q(&lfo[0]);
    } else lpf1.disconnect_modulator_q();

}

void Synthesthesia::run(const uint32_t sample_count)
{
    validate_and_update_ports();

    /* 
    get incoming midi messages
    within an atom each message is stored sequentially after the header
    */
    uint32_t last_frame = 0;
    LV2_ATOM_SEQUENCE_FOREACH(midi_in[0], ev){
        update_filter_connections(); // update modulators for filters if they have changes

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
                    msg[2], // velocity
                    this
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

