#ifndef __PARAMETER_TYPE_HPP_
#define __PARAMETER_TYPE_HPP_

#include "config.hpp"
#include "Waveform.hpp"

#include <array>
#include <utility>
#include <limits>
#include <cmath>


/**
 * @brief types of parameters that might exist within any given module
*/
enum class ParameterType {
    STATUS,
    WAVEFORM,
    FREQUENCY,
    AMPLITUDE,
    PHASE,
    PAN,
    DETUNE,
    ATTACK,
    DECAY,
    SUSTAIN,
    RELEASE,
    PARAMETER_N
};

constexpr std::array<std::pair<float, float>, static_cast<int>(ParameterType::PARAMETER_N)> parameterLimits({
    std::make_pair(0.0f, 1.0f),                               // STATUS
    std::make_pair(0.0f,static_cast<float>(Waveform::N)),     // WAVEFORM
    std::make_pair(0.0f, std::numeric_limits<float>::max()),  // FREQUENCY
    std::make_pair(0.0f, 1.0f),                               // AMPLITUDE
    std::make_pair(0.0f, 1.0f),                               // PHASE
    std::make_pair(-1.0f, 1.0f),                              // PAN
    std::make_pair(-24.5f, 24.5f),                            // DETUNE
    std::make_pair(0.001f, 4.0f ),                            // ATTACK
    std::make_pair(0.001f, 4.0f ),                            // DECAY
    std::make_pair(0.0f, 1.0f ),                              // SUSTAIN
    std::make_pair(0.001f, 4.0f )                             // RELEASE
});

#endif // __PARAMETER_TYPE_HPP_