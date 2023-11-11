#include "KeyboardController.hpp"
#include "ParameterType.hpp"
#include "ADSREnvelope.hpp"

#include <cmath>
#include <algorithm>

// initialization of static class member variables
boost::container::flat_map<uint8_t,Note> KeyboardController::notes_ ;
std::array<double,16384> KeyboardController::pitchbend_scale_factor_ ;
uint16_t KeyboardController::pitchbend_value_ = 8192 ;
uint8_t KeyboardController::sustain_ = 0 ;

void KeyboardController::generate(){
    computePitchbendScaleFactor();
}

void KeyboardController::computePitchbendScaleFactor(){
    float shiftValue ;
    for(uint16_t i = 0; i < 16384; ++i){
        shiftValue = ( i - 8192.0 ) / 16383.0 * CONFIG_PITCHBEND_MAX_SHIFT * 2.0 ; 
        pitchbend_scale_factor_[i] = std::pow(2.0f, (shiftValue / 12.0f));
    }
}

void KeyboardController::pressNote(uint8_t midi_note, float velocity){
    if(notes_[midi_note].getNote() != midi_note ) notes_[midi_note].setNote(midi_note);
    notes_[midi_note].setIsPressed(true);
    notes_[midi_note].setVelocity(velocity);
}

void KeyboardController::releaseNote(uint8_t midi_note){
    if ( midi_note > 127 ) midi_note = 127 ;
    notes_[midi_note].setIsPressed(false);
}

float KeyboardController::getPitchbend(){
    return pitchbend_scale_factor_[pitchbend_value_] ;
}

void KeyboardController::setPitchbend(uint16_t pitchbend ){
    if ( pitchbend > 16383 ) pitchbend = 16383 ;
    pitchbend_value_ = pitchbend ;
}

uint8_t KeyboardController::getSustain(){
    return sustain_ ;
}

void KeyboardController::setSustain(uint8_t sustain ){
    if ( sustain > 127 ) sustain_ = 127 ;
    else sustain_ = sustain ;
}
    
const boost::container::flat_map<uint8_t,Note>* KeyboardController::get_active_notes(){
    return &notes_ ;
}

void KeyboardController::processMidi(LV2_Midi_Message_Type message_type, const uint8_t* const msg){
    switch(message_type){
    case LV2_MIDI_MSG_NOTE_ON:
        pressNote(msg[1],msg[2]);
        break;
    case LV2_MIDI_MSG_NOTE_OFF:
        releaseNote(msg[1]);
        break;
    case LV2_MIDI_MSG_BENDER:
        setPitchbend(static_cast<uint16_t>((msg[2] << 7) | msg[1]));
        break;
    case LV2_MIDI_MSG_CONTROLLER:
        // TODO: figure out if we want to mess with any other controller messages
        if ( msg[1] == LV2_MIDI_CTL_SUSTAIN ) setSustain(msg[2]);
        break;
    default:
        break;
    }
}

void KeyboardController::tick(double time){
    for (auto it = notes_.begin(); it != notes_.end(); ){
        it->second.tick(time);
        if (
            !it->second.getIsPressed() &&
            // it->second.getTimeSinceReleased() > parameterLimits[static_cast<int>(ParameterType::RELEASE)].second
            it->second.getTimeSinceReleased() > ADSREnvelope::getReleaseTime()
        ){
            it = notes_.erase(it) ;
        } else {
            ++it ;
        }
    }
}