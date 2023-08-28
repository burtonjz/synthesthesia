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
#include "cfg-connection.hpp"
#include "filter-type.hpp"


Synthesthesia::Synthesthesia(const double sample_rate, const LV2_Feature *const *features):
    midi_in{nullptr},
    audio_out{nullptr},
    ctrl_osc{nullptr},
    ctrl_osc_values{0},
    ctrl_env{nullptr},
    ctrl_env_values{0},
    ctrl_lfo{nullptr},
    ctrl_lfo_values{0},
    ctrl_filter{nullptr},
    ctrl_filter_values{0},
    map(nullptr),
    rate(sample_rate),
    pos(0.0),
    key(),
    ctrl_osc_gain{LinearFader<float>(0.0f)},
    ctrl_osc_detune{0.0},
    ctrl_osc_pan{Panner()},
    filter{Filter()},
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
    for(int i = 0; i < N_FILTERS; ++i){
        filter[i].set_synth_ptr(this);
    }

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

bool Synthesthesia::any_key_active() const {
    for (const auto& pair : key){
        if ( pair.second.get_status() != KEY_OFF) return true;
    }
    return false;
}
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
    return std::make_tuple(status,time,start_level);
}

void Synthesthesia::connectPort(const uint32_t port, void* data){

    
    if (port >= PORT_MIDI_RANGE.first && port <= PORT_MIDI_RANGE.second){
        midi_in[port] = static_cast<const LV2_Atom_Sequence*>(data);
    } else if (port >= PORT_AUDIO_OUT_RANGE.first && port <= PORT_AUDIO_OUT_RANGE.second){
        audio_out[port - PORT_AUDIO_OUT_RANGE.first] = static_cast<float*>(data);
    } else if (port >= PORT_OSC_RANGE.first && port <= PORT_OSC_RANGE.second){
        ctrl_osc[port - PORT_OSC_RANGE.first] = static_cast<const float*>(data);
    } else if (port >= PORT_ENV_RANGE.first && port <= PORT_ENV_RANGE.second){
        ctrl_env[port - PORT_ENV_RANGE.first] = static_cast<const float*>(data);
    } else if (port >= PORT_LFO_RANGE.first && port <= PORT_LFO_RANGE.second){
        ctrl_lfo[port - PORT_LFO_RANGE.first] = static_cast<const float*>(data);
    } else if (port >= PORT_FILTER_RANGE.first && port <= PORT_FILTER_RANGE.second){
        ctrl_filter[port - PORT_FILTER_RANGE.first] = static_cast<const float*>(data);
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
        filter[0].tick(out);
        if(filter[0].is_active()){
            out = filter[0].get_sample();
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
            if(lfo[i].get_is_active()){
                if (key.empty()) lfo[i].reset();
                lfo[i].tick();
            } 
        }
    }
}

std::array<OscillatorConfig,N_OSCILLATORS> Synthesthesia::configure_oscillators(){
    std::array<OscillatorConfig,N_OSCILLATORS> osc_configs;
    std::array<Modulator*,OSC_CONNECT_N> osc_mods;

    // determine modulator connections
    for(int i = 0; i < N_OSCILLATORS; ++i){
        for(int j = 0; j < N_MODULATORS; ++j){
            if(modulator[j]->get_is_active()){
                for(size_t k = 0; k < osc_mods.size(); ++k){
                    if(modulator[j]->find_connection(MODULATABLE_OSCILLATOR,i,k)) osc_mods[k] = modulator[j];
                }
            }
        }

        osc_configs[i] = {
            ctrl_osc_values[i * CTRL_OSC_N + CTRL_OSC_STATUS] != 0.0,
            static_cast<Waveform> (ctrl_osc_values[i * CTRL_OSC_N + CTRL_OSC_WAVEFORM]),
            &ctrl_osc_gain[i],
            ctrl_osc_detune[i],
            osc_mods
        };
    }
    return osc_configs;
}

void Synthesthesia::normalize_osc_gain(){
    // whenever an oscillator gain control is changed
    // normalize sum of gain to be 1.0
    float gain_sum = 0;
    for(int i = 0; i < N_OSCILLATORS; ++i){
        if(ctrl_osc_values[i * CTRL_OSC_N + CTRL_OSC_STATUS] != 0.0){
            gain_sum += ctrl_osc_values[i * CTRL_OSC_N + CTRL_OSC_GAIN];
        }
    }
    if(gain_sum > 1.0){
        for(int i = 0; i < N_OSCILLATORS; ++i){
            ctrl_osc_gain[i].set(ctrl_osc_values[i * CTRL_OSC_N + CTRL_OSC_GAIN]/gain_sum, CTRL_FADER_WEIGHT * rate);
        }
    } else {
        for(int i = 0; i < N_OSCILLATORS; ++i){
            ctrl_osc_gain[i].set(ctrl_osc_values[i * CTRL_OSC_N + CTRL_OSC_GAIN], CTRL_FADER_WEIGHT * rate);
        }
    }
}

void Synthesthesia::validate_and_update_ports(){
    bool need_osc_norm = false; 
    int i_ctrl;
    int i_module;

    // only need to verify midi/audio out are connected
    for (size_t i = 0; i < midi_in.size(); ++i){
        if (!midi_in[i]) return;
    }

    for (size_t i = 0; i < audio_out.size(); ++i){
        if (!audio_out[i]) return;
    }

    // oscillator validation/port logic
    for (size_t i = 0; i < ctrl_osc.size(); ++i){
        if (!ctrl_osc[i]) return;
        if (*ctrl_osc[i] != ctrl_osc_values[i]){
            i_ctrl = i % CTRL_OSC_N;
            i_module = i / CTRL_OSC_N;

            ctrl_osc_values[i] = limit<float>(*ctrl_osc[i],
                OscLimits[i_ctrl].first,OscLimits[i_ctrl].second);

            if( !need_osc_norm && (i_ctrl == CTRL_OSC_STATUS || i_ctrl == CTRL_OSC_GAIN )){
                need_osc_norm = true;
            }

            if ( i_ctrl == CTRL_OSC_DETUNE ) ctrl_osc_detune[i_module] = ctrl_osc_values[i];
            else if ( i_ctrl == CTRL_OSC_PAN ) ctrl_osc_pan[i_module].set(ctrl_osc_values[i], CTRL_FADER_WEIGHT * rate); 
        }
    }
    if(need_osc_norm) normalize_osc_gain();

    // envelope validation/port logic
    for (size_t i = 0; i < ctrl_env.size(); ++i){
        if (!ctrl_env[i]) return;
        if (*ctrl_env[i] != ctrl_env_values[i]){
            i_ctrl = i % CTRL_ENV_N;
            i_module = i / CTRL_ENV_N;

            ctrl_env_values[i] = limit<float>(*ctrl_env[i],
                EnvLimits[i_ctrl].first,EnvLimits[i_ctrl].second);

            switch(i_ctrl){
            case CTRL_ENV_CONNECTIONS:
                env[i_module].set_connections(ctrl_env_values[i]);
                break;
            case CTRL_ENV_ATTACK:
            case CTRL_ENV_DECAY:
            case CTRL_ENV_SUSTAIN:
            case CTRL_ENV_RELEASE:
                env[i_module].set_adsr({
                        ctrl_env_values[CTRL_ENV_ATTACK + i_module * CTRL_ENV_N],
                        ctrl_env_values[CTRL_ENV_DECAY + i_module * CTRL_ENV_N],
                        ctrl_env_values[CTRL_ENV_SUSTAIN + i_module * CTRL_ENV_N],
                        ctrl_env_values[CTRL_ENV_RELEASE + i_module * CTRL_ENV_N]
                });
                break;
            default: 
                break;
            }
        }
    }

    // LFO validation/port logic
    for (size_t i = 0; i < ctrl_lfo.size(); ++i){
        if (!ctrl_lfo[i]) return;
        if (*ctrl_lfo[i] != ctrl_lfo_values[i]){
            i_ctrl = i % CTRL_LFO_N;
            i_module = i / CTRL_LFO_N;

            ctrl_lfo_values[i] = limit<float>(*ctrl_lfo[i],
                LfoLimits[i_ctrl].first,LfoLimits[i_ctrl].second);

            switch(i_ctrl){
            case CTRL_LFO_CONNECTIONS:
                lfo[i_module].set_connections(ctrl_lfo_values[i]);
                break;
            case CTRL_LFO_FREQ:
                lfo[i_module].set_freq(static_cast<double> (ctrl_lfo_values[i]));
                break;
            case CTRL_LFO_WAVEFORM:
                lfo[i_module].set_waveform(static_cast<Waveform> (ctrl_lfo_values[i]));
                break;
            case CTRL_LFO_DEPTH:
                lfo[i_module].set_depth(ctrl_lfo_values[i]);
                break;
            default: 
                break;
            }
        }
    }

    // Filter validation/port logic
    for (size_t i = 0; i < ctrl_filter.size(); ++i){
        if (!ctrl_filter[i]) return;

        if (*ctrl_filter[i] != ctrl_filter_values[i]){
            i_ctrl = i % CTRL_FILTER_N;
            i_module = i / CTRL_FILTER_N;

            ctrl_filter_values[i] = limit<float>(*ctrl_filter[i],
                FilterLimits[i_ctrl].first,FilterLimits[i_ctrl].second);

            switch(i_ctrl){
            case CTRL_FILTER_TYPE:
                filter[i_module].set_type(static_cast<FilterType> (ctrl_filter_values[i]));
                break;
            case CTRL_FILTER_FREQ:
                filter[i_module].set_cutoff_freq(ctrl_filter_values[i]);
                break;
            case CTRL_FILTER_RES:
                filter[i_module].set_q_factor(ctrl_filter_values[i]);
                break;
            default: 
                break;
            }
        }
    }
}

// TODO: Modulator Logic needs to be improved if we want to support dynamic number of components
void Synthesthesia::update_filter_connections(){ 
    for(size_t i = 0; i < filter.size(); ++i){
        for(size_t j = 0; j < modulator.size(); ++j){
            if(modulator[j]->get_is_active()){
                for(int k = 0; k < FILTER_CONNECT_N; ++k){
                    if(modulator[j]->find_connection(MODULATABLE_FILTER,i,k)){
                        filter[i].connect_modulator(modulator[j],static_cast<FilterConnectionPorts>(k));
                    } 
                }
            }
        }
    }
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

