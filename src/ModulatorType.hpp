#ifndef __MODULATOR_TYPE_HPP_
#define __MODULATOR_TYPE_HPP_

/**
 * @brief enumeration of modulation source classes
 * 
*/
enum class ModulatorType {
    LinearFader,
    ADSREnvelope,
    Detuner,
    LFO,
    ModulationChain,
    N_MODULATORS
};

constexpr int N_MODULATOR_TYPES = static_cast<int>(ModulatorType::N_MODULATORS) ;

#endif // __MODULATOR_TYPE_HPP_