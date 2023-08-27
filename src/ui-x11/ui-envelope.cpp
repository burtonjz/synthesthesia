#include "ui-envelope.hpp"
#include "../port-info.hpp"

EnvelopeFrame::EnvelopeFrame():
    slider_attack(0.1, EnvLimits[CTRL_ENV_ATTACK].first, EnvLimits[CTRL_ENV_ATTACK].second),
    slider_decay(0.1, EnvLimits[CTRL_ENV_DECAY].first, EnvLimits[CTRL_ENV_DECAY].second),
    slider_sustain(0.1, EnvLimits[CTRL_ENV_SUSTAIN].first, EnvLimits[CTRL_ENV_SUSTAIN].second),
    slider_release(0.1, EnvLimits[CTRL_ENV_RELEASE].first, EnvLimits[CTRL_ENV_RELEASE].second)
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

void EnvelopeFrame::configure(int x, int y){
    moveTo(x,y);

    slider_attack.moveTo(UI_ENV_SLIDER_ATTACK_X,UI_ENV_SLIDER_ATTACK_Y);
    slider_decay.moveTo(UI_ENV_SLIDER_DECAY_X,UI_ENV_SLIDER_DECAY_Y);
    slider_sustain.moveTo(UI_ENV_SLIDER_SUSTAIN_X,UI_ENV_SLIDER_SUSTAIN_Y);
    slider_release.moveTo(UI_ENV_SLIDER_RELEASE_X,UI_ENV_SLIDER_RELEASE_Y);
}

std::array<BWidgets::Widget*,4> EnvelopeFrame::getWidgetArray() const {
    return widget;
}