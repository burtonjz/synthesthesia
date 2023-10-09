#include "KeyboardController.hpp"
#include "ParameterType.hpp"

#include <cmath>

KeyboardController::KeyboardController():
    notes_(),
    active_note_ids_(),
    pitchbend_frequency_scale_factor_(0),
    sustain_(0)
{}

void KeyboardController::pressNote(uint8_t midi_note, float velocity){

    if(notes_[midi_note].getNote() != midi_note ) notes_[midi_note].setNote(midi_note);

    notes_[midi_note].setIsPressed(true);
    notes_[midi_note].setVelocity(velocity);
    active_note_ids_.insert(midi_note);
}

void KeyboardController::releaseNote(uint8_t midi_note){
    if ( midi_note > 127 ) midi_note = 127 ;

    notes_[midi_note].setIsPressed(false);
}

float KeyboardController::getPitchbend() const {
    return pitchbend_frequency_scale_factor_ ;
}

void KeyboardController::setPitchbend(uint16_t pitchbend ){
    if ( pitchbend > 16383 ) pitchbend = 16383 ;

    // linear transformation to make value in  [-1.0f, 1.0f] * MAX PITCHBEND SHIFT
    float shiftValue = ( (pitchbend - 8192.0f) / 16383 ) * CONFIG_PITCHBEND_MAX_SHIFT ;

    pitchbend_frequency_scale_factor_ = std::pow(2.0f, (shiftValue / 12.0f));
}

void KeyboardController::setSustain(uint8_t sustain ){
    if ( sustain > 127 ) sustain_ = 127 ;
    else sustain_ = sustain ;
}
    
boost::container::vector<NoteInfo> KeyboardController::get_active_notes(){
    boost::container::vector<NoteInfo> note_info ;
    for (auto it = active_note_ids_.begin(); it != active_note_ids_.end(); ++it){
        NoteInfo n ;
        n.frequency = notes_[*it].getFrequency() * getPitchbend();
        n.sustain = sustain_ ;
        n.time_since_event = notes_[*it].getTimeSinceEvent();
        n.velocity = notes_[*it].getVelocity() ;
        note_info.push_back(n);
    }
    return note_info ;
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
    auto it = active_note_ids_.begin();
    while (it != active_note_ids_.end()) {
        notes_[*it].tick(time);
        if ( 
            !notes_[*it].getIsPressed() &&
            ( notes_[*it].getTimeSinceEvent() > parameterLimits[static_cast<int>(ParameterType::RELEASE)].second )
        ){
            it = active_note_ids_.erase(it);
        } else ++it ;
    }
}