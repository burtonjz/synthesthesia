#include "LFO.hpp"

#include "ModulatorType.hpp"
#include "ModuleType.hpp"
#include "ParameterType.hpp"

std::array<ParameterType,4> LFO::control_params_ = {
    ParameterType::STATUS,
    ParameterType::WAVEFORM,
    ParameterType::FREQUENCY,
    ParameterType::AMPLITUDE
};

std::pair<const ParameterType*, size_t> LFO::getControlPorts(){
    return {control_params_.data(), control_params_.size() } ;
}

LFO::LFO():
    Modulator(ModulatorType::LFO),
    Module(ModuleType::LFO),
    oscillator_()
{
    // create LFO parameters
    parameterController_.addParameter<ParameterType::STATUS>(true,false);
    parameterController_.addParameter<ParameterType::WAVEFORM>(parameterDefaults[static_cast<int>(ParameterType::WAVEFORM)],false);
    parameterController_.addParameter<ParameterType::FREQUENCY>(
        CONFIG_LFO_DEFAULT_FREQ,
        true,
        CONFIG_LFO_MIN_FREQ,
        CONFIG_LFO_MAX_FREQ
    );

    // add references to the child oscillator
    oscillator_.getParameterController()->addReferences(parameterController_);

    // treating amplitude as depth, don't want it calculated in oscillator sample
    parameterController_.addParameter<ParameterType::AMPLITUDE>(1.0,true);
}

void LFO::activate(const double* sampleRate){
    oscillator_.activate(sampleRate);
}

void LFO::tick(){
    oscillator_.tick();
    // TODO: make sure to modulate LFO params!
}

double LFO::modulate(double value, ParameterModMap* modp ) const {
    if(!parameterController_.getParameterValue<ParameterType::STATUS>()) return value ;
    
    return value + value * oscillator_.getSample() * parameterController_.getParameterInstantaneousValue<ParameterType::AMPLITUDE>() ;
}
