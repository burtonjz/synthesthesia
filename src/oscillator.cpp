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
    frequency_modulator(nullptr),
    amplitude_modulator(nullptr),
    phase_modulator(nullptr),
    synth_ptr(synth_ptr_)
{};

Oscillator::Oscillator(Waveform wf,uint8_t note,double r,double wave_range_min,double wave_range_max, double a,double p, Synthesthesia* synth_ptr_):
    BaseOscillator(wf,note,r,wave_range_min,wave_range_max,a,p),
    frequency_modulator(nullptr),
    amplitude_modulator(nullptr),
    phase_modulator(nullptr),
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
        amplitude_modulator &&
        amplitude_modulator->get_is_active()
    ){
        if(ADSREnvelope* envamp = dynamic_cast<ADSREnvelope*>(amplitude_modulator)){
            return envamp->get_adsr().release;
        } else return 0.0;
    } else return 0.0;
}

// gets level from envelope if envelope set as amplitude modulator
double Oscillator::get_env_level() const {
    if(
        amplitude_modulator &&
        amplitude_modulator->get_is_active()
    ){
        if(ADSREnvelope* envamp = dynamic_cast<ADSREnvelope*>(amplitude_modulator)){
            Key* k = synth_ptr->find_key(key_index);
            return envamp->get_level(k->get_status(),k->get_time(),k->get_start_level(oscillator_index));
        } else return 1.0f;
    } else return 1.0f;
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
    if (!frequency_modulator || !frequency_modulator->get_is_active()) return;

    if (LFO* lfofreq = dynamic_cast<LFO*>(frequency_modulator)){
        set_inst_freq(get_freq() + lfofreq->modulate_frequency(get_freq()));
        return;
    }

    if (ADSREnvelope* envfreq = dynamic_cast<ADSREnvelope*>(frequency_modulator)){
        Key* k = synth_ptr->find_key(key_index);
        set_inst_freq(get_freq() + envfreq->modulate_frequency(get_freq(),k->get_status(), k->get_time(), k->get_start_level(oscillator_index)));
        return;
    }
}

void Oscillator::modulate_phase(){
    if (!phase_modulator || !phase_modulator->get_is_active()) return;

    if (LFO* lfophase = dynamic_cast<LFO*>(phase_modulator)){
        set_inst_phase(get_phase() + lfophase->modulate_phase(get_phase()));
        return;
    } 

    if (ADSREnvelope* envphase = dynamic_cast<ADSREnvelope*>(phase_modulator)){
        Key* k = synth_ptr->find_key(key_index);
        set_inst_phase(get_phase() + envphase->modulate_phase(get_phase(),k->get_status(),k->get_time(),k->get_start_level(oscillator_index)));
        return;
    }
}

void Oscillator::modulate_amplitude(){
    if(!amplitude_modulator || !amplitude_modulator->get_is_active()) return;
    
    if(LFO* lfoamp = dynamic_cast<LFO*>(amplitude_modulator)){
        set_inst_amp(get_amp() + lfoamp->modulate_amplitude(get_amp()));
        return;
    }

    if(ADSREnvelope* envamp = dynamic_cast<ADSREnvelope*>(amplitude_modulator)){
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
void Oscillator::configure(OscillatorConfig config){
    connect_frequency_modulator(config.freq_mod);
    connect_amplitude_modulator(config.amp_mod);
    connect_phase_modulator(config.phase_mod);
    set_is_active(config.status);
    set_gain_ptr(config.gain);
    set_detune(config.detune);
    set_waveform(config.wf);
}

void Oscillator::connect_frequency_modulator(Modulator* ptr){
    if(!frequency_modulator){
            frequency_modulator = ptr;
    }
}

void Oscillator::disconnect_frequency_modulator(){
    frequency_modulator = nullptr;
}

void Oscillator::connect_amplitude_modulator(Modulator* ptr){
    if(!amplitude_modulator){
        amplitude_modulator = ptr;
    }
}

void Oscillator::disconnect_amplitude_modulator(){
    amplitude_modulator = nullptr;
}

void Oscillator::connect_phase_modulator(Modulator* ptr){
    if(!phase_modulator){
        phase_modulator = ptr;
    }
}

void Oscillator::disconnect_phase_modulator(){
    phase_modulator = nullptr;
}

void Oscillator::disconnect_modulators(){
    disconnect_frequency_modulator();
    disconnect_amplitude_modulator();
    disconnect_phase_modulator();
}