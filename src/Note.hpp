#ifndef __NOTE_HPP_
#define __NOTE_HPP_

#include <cstdint>

/**
 * @brief class to control a single note within the keyboard controller
 * 
 * Note handles midi conversion, stores velocity, keeps track of time since 
 * the note was pressed or released, and conversions from midi number to frequency
*/
class Note {
private:
    uint8_t midi_note_ ;
    uint8_t midi_velocity_ ;
    float frequency_ ;
    bool is_note_pressed_ ;
   double time_since_event_ ;

    void setFrequency(uint8_t midi_note);

public:
    /**
     * @brief Constructor for Note class
     * 
     * @param midi_note the midi note number (0-127)
     * @param midi_velocity the midi velocity (0-127)
     * @param is_note_pressed the key press status
     * @param time_since_event time passed since the note was released. (-1 if note on)
    */
    Note(uint8_t midi_note, uint8_t midi_velocity, bool is_note_pressed,double time_since_event);

    /**
     * @brief Constructor for Note class
     * 
     * This constructor will initialize the note as off and the release time as -1 
     * 
     * @param midi_note the midi note number (0-127)
     * @param midi_velocity the midi velocity (0-127)
    */
    Note(uint8_t midi_note, uint8_t midi_velocity);

    /**
     * @brief Default Constructor for Note class
     * 
     * Initializes with midi_note 0, no velocity and release time as -1
    */
    Note();

    /**
     * @brief returns the midi note (0-127)
    */
    uint8_t getNote() const ;

    /**
     * @brief sets the midi note of this data (will also calculate frequency)
    */
    void setNote(uint8_t note );

    /**
     * @brief returns the velocity (0-127) of this Note
    */
    uint8_t getVelocity() const ;

    /**
     * @brief sets the velocity of this note. Must be between 0-127.
    */
    void setVelocity(uint8_t midi_velocity);

    /**
     * @brief returns the midi note status
    */
    bool getIsPressed() const ;

    /**
     * @brief sets the midi note status
     * 
    */
    void setIsPressed(bool is_note_pressed);

    /**
     * @brief returns the frequency of this Note
    */
    float getFrequency() const ;

    /**
     * @brief gets the time since a MIDI note on/off event
    */
    float getTimeSinceEvent() const ;

    /**
     * @brief resets the time since a MIDI note on/off event to 0
    */
    void resetTimeSinceEvent();

    /**
     * @brief sets the time since the note was last turned off. This
    */
    void tick(double time);
};

#endif // __NOTE_HPP_