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

/**
 * @brief define value variable type for each ParameterType
*/
template <ParameterType Type> struct ParameterTypeTraits ;
template <> struct ParameterTypeTraits<ParameterType::STATUS>    {using ValueType = bool;};
template <> struct ParameterTypeTraits<ParameterType::WAVEFORM>  {using ValueType = int;};
template <> struct ParameterTypeTraits<ParameterType::FREQUENCY> {using ValueType = double;};
template <> struct ParameterTypeTraits<ParameterType::AMPLITUDE> {using ValueType = double;};
template <> struct ParameterTypeTraits<ParameterType::PHASE>     {using ValueType = double;};
template <> struct ParameterTypeTraits<ParameterType::PAN>       {using ValueType = float;};
template <> struct ParameterTypeTraits<ParameterType::DETUNE>    {using ValueType = float;};
template <> struct ParameterTypeTraits<ParameterType::ATTACK>    {using ValueType = float;};
template <> struct ParameterTypeTraits<ParameterType::DECAY>     {using ValueType = float;};
template <> struct ParameterTypeTraits<ParameterType::SUSTAIN>   {using ValueType = float;};
template <> struct ParameterTypeTraits<ParameterType::RELEASE>   {using ValueType = float;};

/**
 * @brief a std::pair containing the min and max for each parameter type
*/
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

/**
 * @brief a std::array containing the defaults for each parameter type
*/
constexpr std::array<float, static_cast<int>(ParameterType::PARAMETER_N)> parameterDefaults({
    0.0f,                               // STATUS
    static_cast<float>(Waveform::SINE), // WAVEFORM
    440.0f,                             // FREQUENCY
    1.0f,                               // AMPLITUDE
    0.0f,                               // PHASE
    0.0f,                               // PAN
    0.0f,                               // DETUNE
    0.01f,                              // ATTACK
    0.05f,                              // DECAY
    0.8f,                               // SUSTAIN
    0.2f                                // RELEASE
});

#endif // __PARAMETER_TYPE_HPP_