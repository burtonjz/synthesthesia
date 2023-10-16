#include "PolyOscillator.hpp"
#include "NoteInfo.hpp"
#include "ParameterController.hpp"
#include "ParameterType.hpp"

#include <boost/container/vector.hpp>
#include <iostream>

PolyOscillator::PolyOscillator(const double* sampleRate):
    Module(sampleRate),
    oscillator_(),
    keyboard_controller_ptr_(nullptr)
{}

void PolyOscillator::activate(){
}

void PolyOscillator::activate(KeyboardController* controller){
    keyboard_controller_ptr_ = controller ;
    activate();
}

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
    if(keyboard_controller_ptr_){
        updateOscillators(keyboard_controller_ptr_->get_active_notes());
    }
}

void PolyOscillator::updateOscillators(const boost::container::flat_map<uint8_t,NoteInfo> active_notes){
    // first, update oscillators with new information from active_notes
    for (const auto& pair : active_notes ){
        auto it = oscillator_.find(pair.first);
        if(it == oscillator_.end()){
            oscillator_.insert(std::make_pair(pair.first,Oscillator(sampleRate_)));
            ParameterController* p = oscillator_[pair.first].getParameterController();
            
            p->setParameterValue<bool>(ParameterType::STATUS,true);
            p->setParameterValue<double>(ParameterType::FREQUENCY,pair.second.frequency);
            p->setParameterValue<double>(ParameterType::AMPLITUDE,pair.second.velocity / 127.0);

            updateChildOutputBuffers(pair.first);

        } else {
            ParameterController* p = oscillator_[pair.first].getParameterController();

            p->setParameterValue<double>(ParameterType::FREQUENCY,pair.second.frequency);

        }
    }

    // remove oscillators that are not active_notes
    for (auto it = oscillator_.begin(); it != oscillator_.end(); ){
        if (active_notes.find(it->first) == active_notes.end()){
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



// #include "Wavetable.hpp"
// // gcc -std=c++17 -o test Wavetable.cpp Oscillator.cpp Note.cpp KeyboardController.cpp PolyOscillator.cpp -I/usr/include/lv2 -L/usr/lib/lv2 -lboost_container -lm -lstdc++
// int main() {
//     double sample_rate = 100 ;
//     PolyOscillator p(&sample_rate);
//     KeyboardController k ;

//     uint8_t n_samples = 20 ;
//     uint8_t midi_msg[3] = {0, 12, 127} ;

//     float audio_buffer_L[n_samples] ;
//     float audio_buffer_R[n_samples] ;

//     for(int i = 0; i < n_samples; ++i){
//         audio_buffer_L[i] = 0.0f ;
//     }

//     // activate stuff
//     Wavetable::generate() ; 

//     p.activate(&k);
//     p.setOutputBuffer(audio_buffer_L, 0);
//     p.setOutputBuffer(audio_buffer_R, 1);

//     // give keyboard controller a midi message
    
//     k.processMidi(LV2_MIDI_MSG_NOTE_ON, midi_msg);

//     p.tick();

//     for(uint8_t i = 0; i < n_samples; ++i ){
//         p.processSample(i);
//         p.tick();
//         std::cout << "index=" << std::to_string(i) 
//             << ", value_L=" << std::to_string(audio_buffer_L[i]) 
//             << ", value_R=" << std::to_string(audio_buffer_R[i]) 
//             << std::endl ;
//     }

//     return 0 ;

// }