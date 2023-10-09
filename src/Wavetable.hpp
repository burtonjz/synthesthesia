#ifndef __WAVETABLE_HPP_
#define __WAVETABLE_HPP_

#include "config.hpp"
#include "Waveform.hpp"

#include <array>
#include <random>

/**
 * class stores all wavetables available to an Oscillator
*/
class Wavetable {
private:
    static std::array<std::array<double,WAVETABLE_SIZE>,static_cast<int>(Waveform::N)> wavetable_ ;

public:
    /**
     * @brief generates all wavetables
    */
    static void generate();

    /**
     * @brief return a read-only pointer to the waveform's wavetable
    */
    static const std::array<double,WAVETABLE_SIZE>* getWavetable(Waveform waveform);

private:
    static void generateSineWavetable();
    static void generateSquareWavetable();
    static void generateTriangleWavetable();
    static void generateSawWavetable();
    static void generateNoiseWavetable();

    static double polyBlep(double t, double dt);
};

#endif // __WAVETABLE_HPP_