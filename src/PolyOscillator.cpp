#include "PolyOscillator.hpp"
#include "Note.hpp"
#include "ParameterController.hpp"
#include "KeyboardController.hpp"
#include "ParameterType.hpp"

#include <cstdint>

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
    updateOscillators(KeyboardController::get_active_notes());
}

void PolyOscillator::updateOscillators(const NoteInfo note_info){
    // first, update oscillators with new information from active_notes
    for (const auto& pair : *note_info.notes_ptr ){
        auto it = oscillator_.find(pair.first);
        if(it == oscillator_.end()){
            oscillator_.insert(std::make_pair(pair.first,Oscillator(sampleRate_)));
            ParameterController* p = oscillator_[pair.first].getParameterController();
            
            p->setParameterValue<bool>(ParameterType::STATUS,true);
            p->setParameterValue<double>(ParameterType::FREQUENCY,pair.second.getFrequency() * note_info.pitchbend_scale_factor);
            p->setParameterValue<double>(ParameterType::AMPLITUDE,pair.second.getVelocity() / 127.0);

            updateChildOutputBuffers(pair.first);

        } else {
            ParameterController* p = oscillator_[pair.first].getParameterController();

            p->setParameterValue<double>(ParameterType::FREQUENCY,pair.second.getFrequency() * note_info.pitchbend_scale_factor);

        }
    }

    // remove oscillators that are not active_notes
    for (auto it = oscillator_.begin(); it != oscillator_.end(); ){
        if ( (*note_info.notes_ptr).find(it->first) == (*note_info.notes_ptr).end()){
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