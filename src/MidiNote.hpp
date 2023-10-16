#ifndef __MIDI_NOTE_HPP_
#define __MIDI_NOTE_HPP_

#include <array>
#include <cstdint>

/**
 * @brief Static class to store midi note frequencies
 * 
 */
class MidiNote {
private:
    static std::array<double,128> midi_frequency_ ;

public:
    /**
     * @brief populate midi frequency array
     * 
     */
    static void generate();

    /**
     * @brief get the frequency for the specified midi note.
     * 
     */
    static double getFrequency(uint8_t midi_note);

};


#endif // __MIDI_NOTE_HPP_