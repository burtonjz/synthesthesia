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
    None
};

#endif // __MODULATOR_TYPE_HPP_