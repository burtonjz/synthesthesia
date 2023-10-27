#include "Synthesthesia.hpp"
#include "Wavetable.hpp"
#include "MidiNote.hpp"
#include "KeyboardController.hpp"
#include "ADSREnvelope.hpp"
#include "portInfo.hpp"

// #include <lv2/lv2plug.in/ns/ext/options/options.h>
#include <lv2/atom/util.h>

#include <stdexcept>
#include <iostream>

Synthesthesia::Synthesthesia(const double sample_rate, const LV2_Feature *const *features):
    midi_in(nullptr),
    urid_map(nullptr),
    audio_out{nullptr},
    sampleRate_(sample_rate),
    oscillator_{PolyOscillator(&sampleRate_)}
{
    const char* missing = lv2_features_query(
        features,
        LV2_URID__map, &urid_map, true,
        NULL
    );

    if(missing) throw std::invalid_argument("Feature map not provided by the host. Cannot instantiate plugin.");

    urids.initialize(urid_map);

}

void Synthesthesia::connectPort(const uint32_t port, void* data){
    switch(port){
    case MidiPorts::MIDI_IN:
        midi_in = static_cast<const LV2_Atom_Sequence*>(data);
        break ;
    case AudioPorts::AUDIO_L + MidiPorts::MIDI_N:
    case AudioPorts::AUDIO_R + MidiPorts::MIDI_N:
        audio_out[port - MidiPorts::MIDI_N] = static_cast<float*>(data);
        break ;
    default:
        // connectControlPort(port - MidiPorts::N - AudioPorts::N);
        break ;
    }
}

void Synthesthesia::connectControlPort(const uint32_t port){ // TODO:
    if (port < PolyOscillator::getNumControlPorts()){
        auto oscillator_ports = PolyOscillator::getControlPorts();
        oscillator_[0].getParameterController();
    }
}

void Synthesthesia::activate(){
    std::cout << "Activating plugin..." << std::endl;

    // generate static data
    Wavetable::generate();
    MidiNote::generate();
    KeyboardController::generate();

    // activate modules and set buffers
    ADSREnvelope::activate(&sampleRate_);

    oscillator_[0].setOutputBuffer(audio_out[0],0);
    oscillator_[0].setOutputBuffer(audio_out[1],1);
    oscillator_[0].activate();

    std::cout << "Activated!" << std::endl; 
}

void Synthesthesia::run(const uint32_t sample_count){
    uint32_t last_frame = 0;

    LV2_ATOM_SEQUENCE_FOREACH(midi_in, ev){
        const uint32_t frame = ev->time.frames;
        play(last_frame, frame);
        last_frame = frame;

        if (ev->body.type == urids.midiEvent) processMidi(ev);
    }

    // play any remaining frames in buffer
    play(last_frame, sample_count);

}

void Synthesthesia::deactivate(){

}

void Synthesthesia::processMidi(LV2_Atom_Event* ev){
    const uint8_t* const msg = reinterpret_cast<const uint8_t*>(ev + 1);
    const LV2_Midi_Message_Type typ = lv2_midi_message_type(msg);

    KeyboardController::processMidi(typ, msg);
}


void Synthesthesia::play(const uint32_t start, const uint32_t end){
    for (uint32_t i = start; i < end; ++i){
        oscillator_[0].processSample(i);
        tick( 1.0 / sampleRate_ ) ;
    }
}

void Synthesthesia::tick(double time){
    oscillator_[0].tick();
    ADSREnvelope::tick();
    KeyboardController::tick(time);
}