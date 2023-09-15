#include "ui-envelope.hpp"
#include "ui-port-handler.hpp"
#include "../port-info.hpp"
#include "../../BWidgets/BWidgets/Supports/ValueableTyped.hpp"

// forward declaration (reqd for callback function)
class SynthesthesiaUI;

EnvelopeFrame::EnvelopeFrame():
    EnvelopeFrame(URID("frame"),"")
{}

EnvelopeFrame::EnvelopeFrame(const uint32_t urid, const std::string& title):
    ModuleFrame(urid,title),
    slider_attack(0.1, EnvLimits[CTRL_ENV_ATTACK].first, EnvLimits[CTRL_ENV_ATTACK].second,0.0,URID("/value-slider"),"attack"),
    slider_decay(0.1, EnvLimits[CTRL_ENV_DECAY].first, EnvLimits[CTRL_ENV_DECAY].second,0.0,URID("/value-slider"),"attack"),
    slider_sustain(0.1, EnvLimits[CTRL_ENV_SUSTAIN].first, EnvLimits[CTRL_ENV_SUSTAIN].second,0.0,URID("/value-slider"),"attack"),
    slider_release(0.1, EnvLimits[CTRL_ENV_RELEASE].first, EnvLimits[CTRL_ENV_RELEASE].second,0.0,URID("/value-slider"),"attack")
{
    setDraggable(false);

    slider_attack.setActivatable(false);
    slider_decay.setActivatable(false);    
    slider_sustain.setActivatable(false);
    slider_release.setActivatable(false);

    control_widget_.push_back(&slider_attack);
    control_widget_.push_back(&slider_decay);
    control_widget_.push_back(&slider_sustain);
    control_widget_.push_back(&slider_release);

    for(auto& element : control_widget_) add(element);
}

void EnvelopeFrame::configure(int x_index, int y_index){
    // configure frame positioning
    set_position(x_index,y_index);

    slider_attack.moveTo(UI_ENV_SLIDER_ATTACK_X,UI_ENV_SLIDER_ATTACK_Y);
    slider_decay.moveTo(UI_ENV_SLIDER_DECAY_X,UI_ENV_SLIDER_DECAY_Y);
    slider_sustain.moveTo(UI_ENV_SLIDER_SUSTAIN_X,UI_ENV_SLIDER_SUSTAIN_Y);
    slider_release.moveTo(UI_ENV_SLIDER_RELEASE_X,UI_ENV_SLIDER_RELEASE_Y);
}

void EnvelopeFrame::port_event(uint32_t port, float value){
    switch(port){
    case CTRL_ENV_CONNECTIONS:
        break; // connections are handled by the ui module they are connected to
    case CTRL_ENV_ATTACK:
        dynamic_cast<BWidgets::ValueableTyped<float>*>(control_widget_[CTRL_ENV_ATTACK])->setValue(value);
        break;
    case CTRL_ENV_DECAY:
        dynamic_cast<BWidgets::ValueableTyped<float>*>(control_widget_[CTRL_ENV_SUSTAIN])->setValue(value);
        break;
    case CTRL_ENV_SUSTAIN:
        dynamic_cast<BWidgets::ValueableTyped<float>*>(control_widget_[CTRL_ENV_DECAY])->setValue(value);
        break;
    case CTRL_ENV_RELEASE:
        dynamic_cast<BWidgets::ValueableTyped<float>*>(control_widget_[CTRL_ENV_RELEASE])->setValue(value);
        break;
    }
}

std::pair<uint32_t,float> EnvelopeFrame::get_callback_data(const uint32_t relative_index){
    std::pair<uint32_t,float> data;
    data.first = PortHandler::get_port(get_module_type(),get_instance(),relative_index);
    data.second = dynamic_cast<BWidgets::ValueableTyped<double>*>(control_widget_[relative_index])->getValue();
    
    return data;
}

const ModuleType EnvelopeFrame::get_module_type(){
    return ModuleType::ENVELOPE;
} 