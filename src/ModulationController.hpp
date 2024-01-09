#ifndef __MODULATION_CONTROLLER_HPP_
#define __MODULATION_CONTROLLER_HPP_

#include "ParameterController.hpp"
#include "ParameterType.hpp"
#include "ModulationSource.hpp"

/**
 * @brief manage modulation sources for modulatable parameters
 * 
 * Each synth module with modulatable parameters will have LV2 control ports dedicated
 * towards assigning their modulation source. This class will make sure to set each 
 * parameter's modulation function appropriately and prepare the correct ModulationParameter
 * map for the given modulation source. TODO: each modulator should have a getModulationMap function
 * so that the class itself is responsible.
*/
class ModulationController {
private:

public:
    /**
     * @brief sets the modulation function and parameters for the specified parameter
     * 
     * @param controller pointer to the module's ParameterController
     * @param param the ParameterType to be modulated
     * @param mod the specified modulation source
    */
    static void setModulation(ParameterController* controller, ParameterType param);
};

#endif // __MODULATION_CONTROLLER_HPP_