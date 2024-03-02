#ifndef __MODULATION_CONTROLLER_HPP_
#define __MODULATION_CONTROLLER_HPP_

#include "config.hpp"
#include "commonTypes.hpp"

#include "ModuleType.hpp"
#include "ParameterType.hpp"
#include "Modulator.hpp"
#include "ModulatorType.hpp"
#include "KeyboardController.hpp"
#include "LinearFader.hpp"
#include "ADSREnvelope.hpp"
#include "Detuner.hpp"
#include "LFO.hpp"
#include "ModulationChain.hpp"
#include "ControlPortManager.hpp"

#include "ParameterController.hpp"
#include "ParameterType.hpp"

#include <map>
#include <array>
#include <cstdint>
#include <tuple>



/**
 * @brief object to provide key indexing of ModulationChains within map
*/
struct ChainIndex {
    ModuleType mod_ ;
    int inst_ ;
    ParameterType p_ ;
    uint8_t midi_note_ ;

    ChainIndex(ModuleType mod, int instance, ParameterType p, uint8_t midi_note);

    ChainIndex(ModuleType mod, int instance, ParameterType p);

    ChainIndex();
};

bool operator<(const ChainIndex& x, const ChainIndex& y);

class ModulationController {
private:
    // modulators
    KeyboardController* keyboardController_ ; // keyboard controller is modulator as provides MIDI_CONTROL modulation
    std::array<LinearFader,N_FADERS> faders_ ;
    std::array<ADSREnvelope,N_ENVELOPES> envelopes_ ;
    std::array<Detuner,N_DETUNERS> detuners_ ;
    std::array<LFO,N_LFOS> lfos_ ;

    std::map<ChainIndex,ModulationChain> chains_ ; // map is filled during activation, so is real-time safe
    Modulator* oscillator_amplitude_mod_ ;

public:
    /**
     * @brief default constructor
     * 
     * The modulation controller is responsible for 
    */
    ModulationController();

    /**
     * @brief activate module by generating all necessary data
     * 
     * @param keyboardController pointer to keybaord controller. Only modulator this class doesn't own.
     * @param sampleRate pointer to synth sample rate
    */
    void activate(KeyboardController* keyboardController, const double* sampleRate);

    /**
     * @brief return the parameter controller for a modulator
     * 
     * Not all modulators have a parameter controller TODO: how do I want to handle this?
     * 
     * @param typ modulator type
     * @param instance modulator instance
    */
    ParameterController* getModulatorParameterController(ModulatorType typ, uint8_t instance);

    /**
     * @brief set modulation for a parameter
     * 
     * @param moduleType the module type to be modulated
     * @param moduleInstance the module instance to be modulated
     * @param params pointer to parameter controller
     * @param p parameter type to modulate
     * @param connections encoded modulation connection integer
     * @param midi_note midi note (required in several modulation parameter maps)
    */
    void setModulation(
        ModuleType moduleType,
        int moduleInstance,
        ParameterController* params, 
        ParameterType p, 
        int connections,
        uint8_t midi_note
    );

    /**
     * @brief return a pointer to the requested modulator
     * 
     * @param typ modulator type
     * @param instance modulator instance
    */
    Modulator* getModulator(ModulatorType typ, uint8_t instance);

    /**
     * @brief tick all components this controller managers
    */
    void tick();

    /**
     * @brief get the release time for the keyboardController
     * 
     * Not elegant, but this class will currently keep track of what modulator is connected to an amplitude control port and will report back
     * for the keyboardController so that notes can turn off efficiently.
    */
    double getAmplitudeModulatorRelease();

    /**
     * @brief use supplied portManager to update all control ports for modulators
     * 
     * @param portManager pointer to portManager
    */
    void updateControlPorts(ControlPortManager* portManager);

    /**
     * @brief register a module during Activation stage. 
     * 
     * Must not be called during real-time processing. This function tells this 
     * controller details about the modulatable parameters of the Module and 
     * prepares relevant data structures
     * 
     * @param mod pointer to modulator
    */
    void registerModule(Module* mod);

    /**
     * @brief registers all Modules that are also Modulators & owned by this class
     * 
     * Modulators are owned by this controller and some (e.g., envelope, lfo) have
     * their own control parameters that can be modulated
    */
    void selfRegisterModules();

private:

    /**
     * @brief update the modulation map for a particular modulation config
     * 
     * @param modp pointer to parameter modulation map
     * @param params pointer to parameter controller
     * @param modt modulator type
     * @param midi_note midi note
    */
    void updateModulationMap(
        ParameterModMap* modp,
        ParameterController* params,
        ModulatorType modt,
        uint8_t midi_note
    );

    /**
     * @brief decode the control port connection index and fill the modulationChain.
     * 
     * A single modulator within the chain is 6 bits:
     * 
     * bits 1-4: modulatorType (0 = no modulator, and ends decoding). 2^16-1 = 15 modulator types max
     * bits 5-6: modulatorInstance 2^2 = 4 max instances for any given modulator type
     * 
     * 32 bit integer means 5 modulators max in a chain
     * 
     * @param ctrl_index control port connection value
     * @param chain pointer to modulation chain. This will be cleared and refilled with appropriate modulators.
     * @param modp modulation parameter map
    */
    std::array<Modulator*, CONFIG_MAX_MODULATION_CHAIN> decodeModulationIndex(int ctrl_index);

    /**
     * @brief encodes a 6 bit modulator within a control port connection index
     * 
     * @param typ modulator type
     * @param instance modulator instance
    */
    uint8_t encodeModulator(ModulatorType typ, uint8_t instance);
};


#endif // __MODULATION_CONTROLLER_HPP_