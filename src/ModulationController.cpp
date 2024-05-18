#include "ModulationController.hpp"
#include "ModuleType.hpp"
#include "MidiModule.hpp"

#include <iostream>
#include <set>



ChainIndex::ChainIndex(ModuleType mod, int instance, ParameterType p, uint8_t midi_note):
    mod_(mod),
    inst_(instance),
    p_(p),
    midi_note_(midi_note)
{}

ChainIndex::ChainIndex(ModuleType mod, int instance, ParameterType p):
    ChainIndex(mod,instance,p,0)
{}

ChainIndex::ChainIndex():
    midi_note_(0)
{}


bool operator<(const ChainIndex& x, const ChainIndex& y){
    return std::tie(x.mod_, x.inst_, x.p_, x.midi_note_ ) < std::tie(y.mod_, y.inst_, y.p_, y.midi_note_);
}

ModulationController::ModulationController():
    keyboardController_(nullptr),
    faders_(),
    envelopes_(),
    detuners_(),
    lfos_()
{}

void ModulationController::activate(KeyboardController* keyboardController, const double* sampleRate){
    keyboardController_ = keyboardController ;

    // generate static data
    Detuner::generate();

    // activate modulators
    for (size_t i = 0; i < faders_.size(); ++i) faders_[i].activate(keyboardController);
    for (size_t i = 0; i < envelopes_.size(); ++i) {
        envelopes_[i].activate(sampleRate,keyboardController);
        envelopes_[i].setInstance(i);
    }
    for (size_t i = 0; i < lfos_.size(); ++i ){
        lfos_[i].activate(sampleRate);
        lfos_[i].setInstance(i);
    } 
}


void ModulationController::setModulation(
        ModuleType moduleType,
        int moduleInstance,
        ParameterController* params, 
        ParameterType p, 
        int connections,
        uint8_t midi_note
){
    if (!params) return ;
    std::cout << "[ModulationController] Setting Modulation for parameterType " << static_cast<int>(p) << " with a connection value of " << connections << std::endl ;
    
    ChainIndex chainIdx(moduleType,moduleInstance,p);
    
    // PolyOscillator needs to specify the chain for its sub-oscillator based on the midi note. 
    if (moduleType == ModuleType::PolyOscillator) chainIdx.midi_note_ = midi_note ;

    if( chains_.find(chainIdx) == chains_.end() ){
        std::cerr << "[ModulationController] ERROR: Could not find modulationChain for moduleType=" << static_cast<int>(moduleType) 
            << " Instance=" << moduleInstance 
            << " Parameter=" << static_cast<int>(p)
            << " midiNote=" << static_cast<int>(midi_note) << std::endl ;
        return ;
    }

    ParameterModMap modp ;

    chains_[chainIdx].reset();

    std::array<Modulator*, CONFIG_MAX_MODULATION_CHAIN> mods = decodeModulationIndex(connections);
    for(size_t i = 0 ; i < mods.size() ; ++i){
        if(mods[i]){
            chains_[chainIdx].addModulator(mods[i]);
            updateModulationMap(&modp,params,mods[i]->getType(),midi_note);
        } else break ;
    }
    params->setParameterModulation(p,&chains_[chainIdx],modp);
}

void ModulationController::updateModulationMap(ParameterModMap* modp, ParameterController* params, ModulatorType modt, uint8_t midi_note){
    if (!modp || !params) return ;

    switch(modt){
    case ModulatorType::ADSREnvelope:
        (*modp)[ModulationParameter::MIDI_NOTE] = midi_note ;
        (*modp)[ModulationParameter::INITIAL_VALUE] = 0.0 ;
        (*modp)[ModulationParameter::LAST_VALUE] = 0.0 ;
        break ;
    case ModulatorType::Detuner:
        (*modp)[ModulationParameter::DETUNE_CENTS] = params->getParameterInstantaneousValue<ParameterType::DETUNE>() ;
        break ;
    case ModulatorType::LinearFader:
        (*modp)[ModulationParameter::MIDI_NOTE] = midi_note ;
        break ;
    // no map params needed for the following:
    case ModulatorType::LFO: 
    case ModulatorType::MidiControl:
    case ModulatorType::ModulationChain: 
    default:
        break ;
    }
}

Modulator* ModulationController::getModulator(ModulatorType typ, uint8_t instance){
    switch(typ){
    case ModulatorType::LinearFader:
        if (instance < N_FADERS) return &faders_[instance] ;
        std::cout << "[ModulationController] WARNING requested fader instance out of range." << std::endl ;
        return nullptr ;
    case ModulatorType::ADSREnvelope:
        if (instance < N_ENVELOPES) return &envelopes_[instance] ;
        std::cout << "[ModulationController] WARNING requested envelope instance out of range." << std::endl ;
        return nullptr ;
    case ModulatorType::Detuner:
        if (instance < N_DETUNERS) return &detuners_[instance] ;
        std::cout << "[ModulationController] WARNING requested detuner instance out of range." << std::endl ;
        return nullptr ;
    case ModulatorType::LFO:
        if (instance < N_LFOS) return &lfos_[instance] ;
        std::cout << "[ModulationController] WARNING requested LFO instance out of range." << std::endl ;
        return nullptr ;
    case ModulatorType::MidiControl:
        if (instance > 0){
            std::cout << "[ModulationController] WARNING KeyboardController requested with instance greater than 1. That's weird. I'll give it to you anyway..." << std::endl ;
        } 
        return keyboardController_ ;
    case ModulatorType::ModulationChain:
        std::cout << "[ModulationController] WARNING ModulationChains should not be requested through getModulator. Here's a nullptr." << std::endl ;
        return nullptr ;
    default:
        std::cout << "[ModulationController] WARNING Unknown modulator type requested. Take a null pointer!" << std::endl ;
        return nullptr ;
    }
}

void ModulationController::tick(){
    for (size_t i = 0; i < envelopes_.size(); ++i ) envelopes_[i].tick() ;
    for (size_t i = 0; i < lfos_.size(); ++i ) lfos_[i].tick() ;
}

double ModulationController::getAmplitudeModulatorRelease(){
    /*
    TODO: this probably shouldn't be a function. A desired behavior to separate concerns would be:

    1. ParameterModMap includes metadata for what is being modulated -- ParameterType, what module, what instance
    2. KeyboardController stores 2D bitmap of size (N_POLY_OSCILLATORS, N_MIDI_NOTES) that is set to false
    3. If ADSREnvelope/LinearFader reach end state (e.g., end of release or end of fade), toggle release for respective midi note and oscillator instance, set flag to true
    4. In KeyboardController tick, check map, if all set to true for a given midi note, kill the note
    5. If ADSREnvelope/LinearFader reach start state, toggle flag to false
    6. If Modulation is reset for a poly oscillator instance, the ModulationController sets that instance's midi bit array to false.
    */ 

    return 4.0 ;
    
}

void ModulationController::updateControlPorts(ControlPortManager* portManager){
    if(!portManager) return ;

    for (int i = 0; i < envelopes_.size(); ++i) portManager->updateModuleParameters(envelopes_[i].getParameterController(),ModuleType::ADSREnvelope,i);
    for (int i = 0; i < lfos_.size(); ++i) portManager->updateModuleParameters(lfos_[i].getParameterController(),ModuleType::LFO,i);
}

void ModulationController::registerModule(Module* mod){
    if (!mod) return ;
    ChainIndex idx ;
    idx.mod_ = mod->getType();
    idx.inst_ = mod->getInstance();
    
    std::cout << "[ModulationController] Registering Module " << static_cast<int>(idx.mod_) << " Instance " << idx.inst_ << std::endl ;
    
    // Verify if module is a midi module TODO: use type_traits to make this less hard coded
    ParameterController* params = mod->getParameterController();
    std::set<ParameterType> mp = params->getModulatableParameters();

    // first, any parent modulatable parameters should have their chains created
    

    if (auto* midiModulePtr = dynamic_cast<MidiModule<ModuleType::Oscillator>*>(mod)){
        // get child modulatable parameters, loop through them, and create chains under the parent
        auto [ p, np ] = ControlPortManager::getModulatableParameters(midiModulePtr->getChildType());
        for(int i = 0 ; i < np; ++i ){
            idx.p_ = p[i];
            for (int j = 0; j < 128; ++j){
                idx.midi_note_ = j ;
                std::cout << "[ModulationController]        Creating ModulationChain for Parameter=" << static_cast<int>(idx.p_) << " midiNote=" << static_cast<int>(idx.midi_note_) << std::endl ;
                chains_[idx] = ModulationChain();
            }
        }
        // now loop through parent parameters that cascade down to children
        for (auto it = mp.begin(); it != mp.end(); ++it ){
            idx.p_ = *it ;
            for (int j = 0; j < 128; ++j){
                idx.midi_note_ = j ;
                std::cout << "[ModulationController]        Creating ModulationChain for Parameter=" << static_cast<int>(idx.p_) << " midiNote=" << static_cast<int>(idx.midi_note_) << std::endl ;
                chains_[idx] = ModulationChain();
            }
        }
    } else {
        // otherwise, we'll just create the chains for the parameters (no midi_note)
        for (auto it = mp.begin(); it != mp.end(); ++it ){
            idx.p_ = *it ;
            std::cout << "[ModulationController]        Creating ModulationChain for Parameter=" << static_cast<int>(idx.p_) << std::endl ;
            chains_[idx] = ModulationChain();
        }
    }
}

void ModulationController::selfRegisterModules(){
    for (size_t i = 0; i < envelopes_.size(); ++i ) registerModule(&envelopes_[i]);
    for (size_t i = 0; i < lfos_.size(); ++i ) registerModule(&lfos_[i]);
}

std::array<Modulator*, CONFIG_MAX_MODULATION_CHAIN> ModulationController::decodeModulationIndex(int ctrl_index){

    std::array<Modulator*, CONFIG_MAX_MODULATION_CHAIN> mods{} ;
    const int MASK = (1 << CONFIG_MODULATOR_N_BITS) - 1 ; 
    
    // loop through integer to extract encoded modulators
    int shift = 0 ;
    while (shift < CONFIG_MAX_MODULATION_CHAIN) {
        int chunk = (ctrl_index >> shift * CONFIG_MODULATOR_N_BITS) & MASK ;

        int mod_type = (chunk >> (CONFIG_MODULATOR_N_BITS - CONFIG_MODULATOR_TYPE_N_BITS)) - 1; // 0 is no modulator, so "- 1" switches to enum index
        uint8_t mod_inst = chunk & ((1 << (CONFIG_MODULATOR_N_BITS - CONFIG_MODULATOR_TYPE_N_BITS)) - 1) ;

        // if we don't get a valid modulator type, or the type is specified as none, end iteration
        if (mod_type == -1 || mod_type > N_MODULATOR_TYPES ) break ;

        mods[shift] = getModulator(static_cast<ModulatorType>(mod_type),mod_inst);

        // if we don't get a valid modulator, then it was an invalid instance and we should end the loop
        if (!mods[shift]) {
            break ;
        }

        // otherwise, move to next chunk
        shift += 1 ;
    }

    return mods ;
}


uint8_t ModulationController::encodeModulator(ModulatorType typ, uint8_t instance){
    if (getModulator(typ,instance) == nullptr){
        std::cout << "[ModulationController] WARNING requested encoding does not result in valid modulator." << std::endl ;
        return 0 ;
    }

    int mod_type = (static_cast<int>(typ) + 1) << (CONFIG_MODULATOR_N_BITS - CONFIG_MODULATOR_TYPE_N_BITS);
    return mod_type | (instance & CONFIG_MODULATOR_INST_N_BITS) ;

}