#ifndef __MODULATION_SOURCE_HPP_
#define __MODULATION_SOURCE_HPP_

/**
 * @brief definitions of modulation sources. To be used primarily in conjunction with the ModulationController
 * 
 * In order to dynamically set modulation sources through LV2 ports, we need to be able to pair each modulation
 * source to a key value.
*/
enum class ModulationSource {
    LinearFader,
    ADSREnvelope,
};

#endif // __MODULATION_SOURCE_HPP_