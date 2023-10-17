#ifndef __MODULATION_PARAMETERS_HPP_
#define __MODULATION_PARAMETERS_HPP_

/**
 * @brief valid key values for handling additional modulation variables
 * 
 * the Parameter class sets a modulation function of type std::function<T(T,boost::container::flat_map<ModulationParameter,T>).
 * extra variables may be exposed to the modulation function through this map
*/
enum class ModulationParameter {
    MIDI_NOTE
};

#endif // __MODULATION_PARAMETERS_HPP_