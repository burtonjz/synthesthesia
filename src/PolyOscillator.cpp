#include "PolyOscillator.hpp"
#include "Note.hpp"
#include "ParameterController.hpp"
#include "KeyboardController.hpp"
#include "ParameterType.hpp"
#include "LinearFader.hpp"
#include "ADSREnvelope.hpp"
#include "ModulationParameter.hpp"

#include <cstdint>

#ifdef DEBUG
#include <iostream>
#endif

PolyOscillator::PolyOscillator(const double* sampleRate):
    Module(sampleRate),
    oscillator_()
{}

void PolyOscillator::activate(){}

void PolyOscillator::setOutputBuffer(float* buffer, size_t channel){
    outputBuffer_.set(buffer,channel);
    for (auto& pair : oscillator_){
        pair.second.setOutputBuffer(buffer,channel);
    }
}

void PolyOscillator::processSample(uint32_t idx){
    for (auto& pair : oscillator_ ){
        pair.second.processSample(idx);
    }
}

void PolyOscillator::tick(){
    updateOscillators();
}

void PolyOscillator::updateOscillators(){
    const boost::container::flat_map<uint8_t,Note>* notes_ptr = KeyboardController::get_active_notes();
    // first, update oscillators with new information from active_notes
    for (const auto& pair : *notes_ptr ){
        auto it = oscillator_.find(pair.first);
        if(it == oscillator_.end()){
            oscillator_.insert(std::make_pair(pair.first,Oscillator(sampleRate_)));
            ParameterController* p = oscillator_[pair.first].getParameterController();
            
            p->setParameterValue<bool>(ParameterType::STATUS,true);
            p->setParameterValue<double>(ParameterType::FREQUENCY,pair.second.getFrequency() * KeyboardController::getPitchbend());
            p->setParameterValue<double>(ParameterType::AMPLITUDE,pair.second.getVelocity() / 127.0);

            boost::container::flat_map<ModulationParameter,double> amp_map ;
            amp_map[ModulationParameter::MIDI_NOTE] = pair.first ;
            amp_map[ModulationParameter::LAST_SAMPLE] = 0.0 ;
            
            p->setParameterModulation<double>(
                ParameterType::AMPLITUDE, 
                ADSREnvelope::modulate, 
                amp_map
            );

            updateChildOutputBuffers(pair.first);

        } else {
            ParameterController* p = oscillator_[pair.first].getParameterController();

            p->setParameterValue<double>(ParameterType::FREQUENCY,pair.second.getFrequency() * KeyboardController::getPitchbend());

        }
    }

    // remove oscillators that are not active_notes
    for (auto it = oscillator_.begin(); it != oscillator_.end(); ){
        if ( (*notes_ptr).find(it->first) == (*notes_ptr).end()){
            it = oscillator_.erase(it);
        } else {
            oscillator_[it->first].tick();
            ++it ;
        }
    }

}

void PolyOscillator::updateChildOutputBuffers(uint8_t index){
    for(int i = 0; i < AUDIO_OUT_N; ++i ){
        oscillator_[index].setOutputBuffer(outputBuffer_.get(i),i);
    }
}


#ifdef DEBUG
#include "Wavetable.hpp"
#include "MidiNote.hpp"
#include <iostream>
// gcc -DDEBUG -std=c++17 -o test ADSREnvelope.cpp MidiNote.cpp Wavetable.cpp Oscillator.cpp Note.cpp KeyboardController.cpp PolyOscillator.cpp -I/usr/include/lv2 -L/usr/lib/lv2 -lboost_container -lm -lstdc++
int main() {
    KeyboardController::generate();
    MidiNote::generate();
    ADSREnvelope::activate();

    double sample_rate = 10 ;
    PolyOscillator p(&sample_rate);
    
    int n_samples = 100 ;
    uint8_t midi_msg[3] = {0, 69, 80} ;

    float audio_buffer_L[n_samples] ;
    float audio_buffer_R[n_samples] ;

    for(int i = 0; i < n_samples; ++i){
        audio_buffer_L[i] = 0.0f ;
        audio_buffer_R[i] = 0.0f ;
    }

    // activate stuff
    Wavetable::generate() ; 

    p.activate();
    p.setOutputBuffer(audio_buffer_L, 0);
    p.setOutputBuffer(audio_buffer_R, 1);

    // give keyboard controller a midi message
    KeyboardController::processMidi(LV2_MIDI_MSG_NOTE_ON, midi_msg);
    // p.tick();

    for(int i = 0; i < 60; ++i ){
        p.processSample(i);
        p.tick();
        KeyboardController::tick(1.0/sample_rate);
        // std::cout << std::to_string(i) 
        //     << ", " << std::to_string(audio_buffer_L[i]) 
        //     << ", " << std::to_string(audio_buffer_R[i]) 
        //     << std::endl ;
    }

    // release note and refresh buffer
    KeyboardController::processMidi(LV2_MIDI_MSG_NOTE_OFF, midi_msg);
    for(int i = 0; i < n_samples; ++i){
        audio_buffer_L[i] = 0.0f ;
        audio_buffer_R[i] = 0.0f ;
    }

    for(int i = 0; i < 15; ++i ){
        p.processSample(i);
        p.tick();
        KeyboardController::tick(1.0/sample_rate);
        // std::cout << std::to_string(i) 
        //     << ", " << std::to_string(audio_buffer_L[i]) 
        //     << ", " << std::to_string(audio_buffer_R[i]) 
        //     << std::endl ;
    }

    // press note again & refresh buffer
    KeyboardController::processMidi(LV2_MIDI_MSG_NOTE_ON, midi_msg);
    for(int i = 0; i < n_samples; ++i){
        audio_buffer_L[i] = 0.0f ;
        audio_buffer_R[i] = 0.0f ;
    }

    for(int i = 0; i < 15; ++i ){
        p.processSample(i);
        p.tick();
        KeyboardController::tick(1.0/sample_rate);
        // std::cout << std::to_string(i) 
        //     << ", " << std::to_string(audio_buffer_L[i]) 
        //     << ", " << std::to_string(audio_buffer_R[i]) 
        //     << std::endl ;
    }



    return 0 ;

}
#endif