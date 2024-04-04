#ifndef __MODULE_TYPE_HPP_
#define __MODULE_TYPE_HPP_

enum class ModuleType {
    Oscillator,
    PolyOscillator,
    ADSREnvelope,
    LFO,
    // Filter,
    N_MODULES
};

constexpr int N_MODULE_TYPES = static_cast<int>(ModuleType::N_MODULES) ;


#endif // __MODULE_TYPE_HPP_