#include "config.hpp"
#include "oscillator.hpp"
#include "modulator-env.hpp"
#include "modulator-lfo.hpp"
#include "adsr.hpp"
#include "key.hpp"
#include "synthesthesia.hpp"

// CONSTRUCTORS

/*
The Oscillator builds off the BaseOscillator class, to include logic for modulation.

This class is highly coupled with the Key class due to the nature of envelope modulators.
Unlike other modulation sources, envelopes used to modulate amplitude need to be known to
the Key so that the Key class knows when the release stage is completed.
*/
Oscillator::Oscillator(Waveform wf,double f,double r,double wave_range_min,double wave_range_max, double a,double p, Synthesthesia* synth_ptr_):
    BaseOscillator(wf,f,r,wave_range_min,wave_range_max,a,p),
    osc_mod{nullptr}, 
    synth_ptr(synth_ptr_)
{};

Oscillator::Oscillator(Waveform wf,uint8_t note,double r,double wave_range_min,double wave_range_max, double a,double p, Synthesthesia* synth_ptr_):
    BaseOscillator(wf,note,r,wave_range_min,wave_range_max,a,p),
    osc_mod{nullptr},
    synth_ptr(synth_ptr_)
{};

Oscillator::Oscillator(Waveform wf,double f,double r, double wave_range_min, double wave_range_max):
    Oscillator::Oscillator(wf,f,r,wave_range_min,wave_range_max,DEFAULT_AMPLITUDE,DEFAULT_PHASE,nullptr)
{};

Oscillator::Oscillator(Waveform wf,uint8_t note,double r):
    Oscillator::Oscillator(wf,note,r,DEFAULT_WAVE_RANGE_MIN,DEFAULT_WAVE_RANGE_MAX,DEFAULT_AMPLITUDE,DEFAULT_PHASE,nullptr)
{};

Oscillator::Oscillator(Waveform wf, double f, double r):
    Oscillator::Oscillator(wf,f,r,DEFAULT_WAVE_RANGE_MIN,DEFAULT_WAVE_RANGE_MAX,DEFAULT_AMPLITUDE,DEFAULT_PHASE,nullptr)
{};

Oscillator::Oscillator():
    Oscillator::Oscillator(DEFAULT_WAVEFORM,static_cast <uint8_t> (0),DEFAULT_SAMPLING_RATE)
{};

// GETTERS/SETTERS

// gets release time from envelope if envelope set as amplitude modulator
double Oscillator::get_release(){
    if(
        osc_mod[OSC_CONNECT_AMP] &&
        osc_mod[OSC_CONNECT_AMP]->get_is_active()
    ){
        if(ADSREnvelope* envamp = dynamic_cast<ADSREnvelope*>(osc_mod[OSC_CONNECT_AMP])){
            return envamp->get_adsr().release;
        } else return 100.0; // arbitrarily high value so that KeyFader can control when key goes off
    } else return 100.0;
}

// gets level from envelope if envelope set as amplitude modulator
double Oscillator::get_env_level() const {
    if(
        osc_mod[OSC_CONNECT_AMP] &&
        osc_mod[OSC_CONNECT_AMP]->get_is_active()
    ){
        if(ADSREnvelope* envamp = dynamic_cast<ADSREnvelope*>(osc_mod[OSC_CONNECT_AMP])){
            Key* k = synth_ptr->find_key(key_index);
            return envamp->get_level(k->get_status(),k->get_time(),k->get_start_level(oscillator_index));
        } else return -1.0f;
    } else return -1.0f;
}

// set pointer to parent Synth object. Necessary to synchronize envelope
// or other modulation sources with corresponding Key Presses
void Oscillator::set_synth_ptr(Synthesthesia* synth_ptr_){
    synth_ptr = synth_ptr_;
}

// also need to know where key is within the flat map
void Oscillator::set_key_index(uint8_t i){
    key_index = i;
}

// Give the Oscillator knowledge of where it is in the Key class so that
// we can get the proper start_level during envelope modulation.
void Oscillator::set_oscillator_index(int i){
    oscillator_index = i;
}

// OTHER FUNCTION IMPLEMENTATIONS

void Oscillator::modulate(){ // TODO: Once Envelope is refactored as a Modulator derived class, implement here!
    modulate_frequency();
    modulate_phase();
    modulate_amplitude();
}


void Oscillator::modulate_frequency(){
    if (!osc_mod[OSC_CONNECT_FREQ] || !osc_mod[OSC_CONNECT_FREQ]->get_is_active()) return;

    if (LFO* lfofreq = dynamic_cast<LFO*>(osc_mod[OSC_CONNECT_FREQ])){
        set_inst_freq(get_freq() + lfofreq->modulate_frequency(get_freq()));
        return;
    }

    if (ADSREnvelope* envfreq = dynamic_cast<ADSREnvelope*>(osc_mod[OSC_CONNECT_FREQ])){
        Key* k = synth_ptr->find_key(key_index);
        set_inst_freq(get_freq() + envfreq->modulate_frequency(get_freq(),k->get_status(), k->get_time(), k->get_start_level(oscillator_index)));
        return;
    }
}

void Oscillator::modulate_phase(){
    if (!osc_mod[OSC_CONNECT_PHASE] || !osc_mod[OSC_CONNECT_PHASE]->get_is_active()) return;

    if (LFO* lfophase = dynamic_cast<LFO*>(osc_mod[OSC_CONNECT_PHASE])){
        set_inst_phase(get_phase() + lfophase->modulate_phase(get_phase()));
        return;
    } 

    if (ADSREnvelope* envphase = dynamic_cast<ADSREnvelope*>(osc_mod[OSC_CONNECT_PHASE])){
        Key* k = synth_ptr->find_key(key_index);
        set_inst_phase(get_phase() + envphase->modulate_phase(get_phase(),k->get_status(),k->get_time(),k->get_start_level(oscillator_index)));
        return;
    }
}

void Oscillator::modulate_amplitude(){
    if(!osc_mod[OSC_CONNECT_AMP] || !osc_mod[OSC_CONNECT_AMP]->get_is_active()) return;
    
    if(LFO* lfoamp = dynamic_cast<LFO*>(osc_mod[OSC_CONNECT_AMP])){
        set_inst_amp(get_amp() + lfoamp->modulate_amplitude(get_amp()));
        return;
    }

    if(ADSREnvelope* envamp = dynamic_cast<ADSREnvelope*>(osc_mod[OSC_CONNECT_AMP])){
        Key* k = synth_ptr->find_key(key_index);
        set_inst_amp(envamp->modulate_amplitude(get_amp(),k->get_status(),k->get_time(),k->get_start_level(oscillator_index)));
        return;
    }
}

// override the tick member function so that it first modulates the value
void Oscillator::tick(){
    modulate();
    BaseOscillator::tick();
}

// Handle ingestion of oscillator config parameter struct
void Oscillator::configure(OscillatorConfig config, uint8_t note, Synthesthesia* synth_ptr, int osc_index){
    set_is_active(config.status);
    set_synth_ptr(synth_ptr);
    set_gain_ptr(config.gain);
    set_key_index(note);
    set_oscillator_index(osc_index);
    set_detune(config.detune);
    set_freq(note);
    set_waveform(config.wf);

    for(size_t i = 0; i < osc_mod.size(); ++i){
        osc_mod[i] = config.modulators[i];
    }

    modulate(); // run first modulation so first sample is modulated
}

void Oscillator::connect_modulator(Modulator* ptr,OscillatorConnectionPorts port){
    if(!osc_mod[port]){
            osc_mod[port] = ptr;
    }
}

void Oscillator::disconnect_modulator(OscillatorConnectionPorts port){
    osc_mod[port] = nullptr;
}


void Oscillator::disconnect_all_modulators(){
    for(size_t i = 0; i < OSC_CONNECT_N; ++i){
        osc_mod[i] = nullptr;
    }
}