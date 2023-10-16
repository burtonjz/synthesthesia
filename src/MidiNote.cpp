#include "MidiNote.hpp"

#include <cmath>

std::array<double,128> MidiNote::midi_frequency_;

void MidiNote::generate(){
    // TODO: support tunings other than equal temperment
    for(uint8_t i = 0; i < 128; ++i){
        midi_frequency_[i] = pow(2.0, (static_cast<float>(i) - 69.0) / 12.0) * 440.0 ;
    }

}

double MidiNote::getFrequency(uint8_t midi_note){
    if (midi_note > 127) midi_note = 127 ;
    return midi_frequency_[midi_note] ;
}