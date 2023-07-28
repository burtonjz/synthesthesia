#include "ui-config.hpp"
#include "ui-oscillator.hpp"
#include "../port-info.hpp"
#include "../waveform.hpp"

UIOscObject::UIOscObject(double x, double y, int index):
    UIObject(x,y,index),
    button_on(true, true),
    dial_gain(
        x+UI_OSC_DIAL_GAIN_X,y+UI_OSC_DIAL_GAIN_Y,UI_OSC_DIAL_GAIN_WIDTH,UI_OSC_DIAL_GAIN_HEIGHT,
        0.5, ctrlLimits[CTRL_OSC1_GAIN].first, ctrlLimits[CTRL_OSC1_GAIN].second
    ),
    cb_waveform(
        x+UI_OSC_BOX_WAVEFORM_X,y+UI_OSC_BOX_WAVEFORM_Y,UI_OSC_BOX_WAVEFORM_WIDTH,UI_OSC_BOX_WAVEFORM_HEIGHT,
        WAVEFORM_STRINGS,1
    ),
    dial_detune_semi(
        x+UI_OSC_DIAL_DETUNE_SEMI_X,y+UI_OSC_DIAL_DETUNE_SEMI_Y,UI_OSC_DIAL_DETUNE_SEMI_WIDTH,UI_OSC_DIAL_DETUNE_SEMI_HEIGHT,
        0, -24, 24
    ),
    dial_detune_cents(
        x+UI_OSC_DIAL_DETUNE_CENTS_X,y+UI_OSC_DIAL_DETUNE_CENTS_Y,UI_OSC_DIAL_DETUNE_CENTS_WIDTH,UI_OSC_DIAL_DETUNE_CENTS_HEIGHT,
        0.0, -0.99, 0.99
    ),
    dial_pan(
        x+UI_OSC_DIAL_PAN_X,y+UI_OSC_DIAL_PAN_Y,UI_OSC_DIAL_PAN_WIDTH,UI_OSC_DIAL_PAN_HEIGHT,
        0.5, ctrlLimits[CTRL_OSC1_PAN].first, ctrlLimits[CTRL_OSC1_PAN].second
    ),
    cb_freq_mod(
        x+UI_OSC_BOX_MOD_FREQ_X,y+UI_OSC_BOX_MOD_FREQ_Y,UI_OSC_BOX_MOD_FREQ_WIDTH,UI_OSC_BOX_MOD_FREQ_HEIGHT,
        MOD_SOURCES,0
    ),
    cb_amp_mod(
        x+UI_OSC_BOX_MOD_AMP_X,y+UI_OSC_BOX_MOD_AMP_Y,UI_OSC_BOX_MOD_AMP_WIDTH,UI_OSC_BOX_MOD_AMP_HEIGHT,
        MOD_SOURCES,0
    ),
    cb_phase_mod(
        x+UI_OSC_BOX_MOD_PHASE_X,y+UI_OSC_BOX_MOD_PHASE_Y,UI_OSC_BOX_MOD_PHASE_WIDTH,UI_OSC_BOX_MOD_PHASE_HEIGHT,
        MOD_SOURCES,0
    )
{
    widget[0] = &button_on;
    widget[1] = &dial_gain;
    widget[2] = &cb_waveform;
    widget[3] = &dial_detune_semi;
    widget[4] = &dial_detune_cents;
    widget[5] = &dial_pan;
    widget[6] = &cb_freq_mod;
    widget[7] = &cb_amp_mod;
    widget[8] = &cb_phase_mod;
}

int UIOscObject::getNumWidgets() const {
    return 9;
}

double UIOscObject::getObjectHeight() const {
    return UI_OSC_HEIGHT;
}

double UIOscObject::getObjectWidth() const {
    return UI_OSC_WIDTH;
}

std::array<BWidgets::Widget*,9> UIOscObject::getWidgetArray() const {
    return widget;
}