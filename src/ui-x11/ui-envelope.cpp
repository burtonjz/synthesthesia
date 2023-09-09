#include "ui-envelope.hpp"
#include "../port-info.hpp"
#include "../../BWidgets/BWidgets/Supports/ValueableTyped.hpp"


EnvelopeFrame::EnvelopeFrame():
    EnvelopeFrame(BUTILITIES_URID_UNKNOWN_URID,"")
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

    widget[0]=&slider_attack;
    widget[1]=&slider_decay;
    widget[2]=&slider_sustain;
    widget[3]=&slider_release;

    for(auto& element : widget) add(element);
}

void EnvelopeFrame::configure(int x_index, int y_index){
    // configure frame positioning
    set_position(x_index,y_index);

    slider_attack.moveTo(UI_ENV_SLIDER_ATTACK_X,UI_ENV_SLIDER_ATTACK_Y);
    slider_decay.moveTo(UI_ENV_SLIDER_DECAY_X,UI_ENV_SLIDER_DECAY_Y);
    slider_sustain.moveTo(UI_ENV_SLIDER_SUSTAIN_X,UI_ENV_SLIDER_SUSTAIN_Y);
    slider_release.moveTo(UI_ENV_SLIDER_RELEASE_X,UI_ENV_SLIDER_RELEASE_Y);
}

void EnvelopeFrame::port_event(int port, float value){
    switch(port){
    case CTRL_ENV_CONNECTIONS:
        break; // connections are handled by the ui module they are connected to
    case CTRL_ENV_ATTACK:
        dynamic_cast<BWidgets::ValueableTyped<float>*>(widget[CTRL_ENV_ATTACK])->setValue(value);
        break;
    case CTRL_ENV_DECAY:
        dynamic_cast<BWidgets::ValueableTyped<float>*>(widget[CTRL_ENV_SUSTAIN])->setValue(value);
        break;
    case CTRL_ENV_SUSTAIN:
        dynamic_cast<BWidgets::ValueableTyped<float>*>(widget[CTRL_ENV_DECAY])->setValue(value);
        break;
    case CTRL_ENV_RELEASE:
        dynamic_cast<BWidgets::ValueableTyped<float>*>(widget[CTRL_ENV_RELEASE])->setValue(value);
        break;
    }
}

std::array<BWidgets::Widget*,4> EnvelopeFrame::getWidgetArray() const {
    return widget;
}