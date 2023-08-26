#include "ui-config.hpp"
#include "ui-envelope.hpp"
#include "../port-info.hpp"
#include "../waveform.hpp"

UIEnvObject::UIEnvObject(double x, double y, int index):
    UIObject(x,y,index),
    slider_attack(
        x+UI_ENV_SLIDER_ATTACK_X,y+UI_ENV_SLIDER_ATTACK_Y,UI_ENV_SLIDER_ATTACK_WIDTH,UI_ENV_SLIDER_ATTACK_HEIGHT,
        0.1, ctrlLimits[CTRL_ENV1_ATTACK].first, ctrlLimits[CTRL_ENV1_ATTACK].second
    ),
    slider_decay(
        x+UI_ENV_SLIDER_DECAY_X,y+UI_ENV_SLIDER_DECAY_Y,UI_ENV_SLIDER_DECAY_WIDTH,UI_ENV_SLIDER_DECAY_HEIGHT,
        0.1, ctrlLimits[CTRL_ENV1_DECAY].first, ctrlLimits[CTRL_ENV1_DECAY].second
    ),
    slider_sustain(
        x+UI_ENV_SLIDER_SUSTAIN_X,y+UI_ENV_SLIDER_SUSTAIN_Y,UI_ENV_SLIDER_SUSTAIN_WIDTH,UI_ENV_SLIDER_SUSTAIN_HEIGHT,
        0.1, ctrlLimits[CTRL_ENV1_SUSTAIN].first, ctrlLimits[CTRL_ENV1_SUSTAIN].second
    ),
    slider_release(
        x+UI_ENV_SLIDER_RELEASE_X,y+UI_ENV_SLIDER_RELEASE_Y,UI_ENV_SLIDER_RELEASE_WIDTH,UI_ENV_SLIDER_RELEASE_HEIGHT,
        0.1, ctrlLimits[CTRL_ENV1_RELEASE].first, ctrlLimits[CTRL_ENV1_RELEASE].second
    )
{
    slider_attack.setActivatable(false);
    slider_decay.setActivatable(false);    
    slider_sustain.setActivatable(false);
    slider_release.setActivatable(false);

    widget[0]=&slider_attack;
    widget[1]=&slider_decay;
    widget[2]=&slider_sustain;
    widget[3]=&slider_release;
}

int UIEnvObject::getNumWidgets() const {
    return 4;
}

double UIEnvObject::getObjectHeight() const {
    return UI_ENV_HEIGHT;
}

double UIEnvObject::getObjectWidth() const {
    return UI_ENV_WIDTH;
}

std::array<BWidgets::Widget*,4> UIEnvObject::getWidgetArray() const {
    return widget;
}