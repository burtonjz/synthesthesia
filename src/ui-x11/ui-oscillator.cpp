#include "ui-oscillator.hpp"
#include "../port-info.hpp"
#include "../waveform.hpp"
#include "../../BWidgets/BWidgets/Supports/ValueableTyped.hpp"

#include <cmath>

OscillatorFrame::OscillatorFrame():
    switch_on(true, true,BUTILITIES_URID_UNKNOWN_URID,"Osc On"),
    dial_gain(0.5, OscLimits[CTRL_OSC_GAIN].first, OscLimits[CTRL_OSC_GAIN].second),
    cb_waveform(WAVEFORM_STRINGS,1),
    dial_detune_semi(0, -24, 24),
    dial_detune_cents(0.0, -0.99, 0.99),
    dial_pan(0.5, OscLimits[CTRL_OSC_PAN].first, OscLimits[CTRL_OSC_PAN].second),
    cb_freq_mod(MOD_SOURCES,1),
    cb_amp_mod(MOD_SOURCES,1),
    cb_phase_mod(MOD_SOURCES,1)
{
    setDraggable(false);
    cb_waveform.setFont(BStyles::Font("sans",CAIRO_FONT_SLANT_NORMAL,CAIRO_FONT_WEIGHT_NORMAL,24.0,BStyles::Font::TextAlign::center,BStyles::Font::TextVAlign::middle,1.5));
    dial_gain.setFont(BStyles::Font("sans",CAIRO_FONT_SLANT_NORMAL,CAIRO_FONT_WEIGHT_NORMAL,24.0,BStyles::Font::TextAlign::center,BStyles::Font::TextVAlign::middle,1.5));

    dial_gain.setClickable(false);
    dial_gain.setActivatable(false);
    dial_pan.setClickable(false);
    dial_pan.setActivatable(false);
    dial_detune_semi.setClickable(false);
    dial_detune_semi.setActivatable(false);
    dial_detune_cents.setClickable(false);
    dial_detune_cents.setActivatable(false);

    widget[0] = &switch_on;
    widget[1] = &cb_waveform;
    widget[2] = &dial_gain;
    widget[3] = &dial_detune_semi;
    widget[4] = &dial_detune_cents;
    widget[5] = &dial_pan;
    widget[6] = &cb_freq_mod;
    widget[7] = &cb_amp_mod;
    widget[8] = &cb_phase_mod;

    for(auto& element : widget) add(element);

}

void OscillatorFrame::configure(int x, int y){
    // Move frame
    moveTo(x,y);
    // Move widgets within frame
    switch_on.moveTo(UI_OSC_SWITCH_ON_X,UI_OSC_SWITCH_ON_Y);
    dial_gain.moveTo(UI_OSC_DIAL_GAIN_X,UI_OSC_DIAL_GAIN_Y);
    cb_waveform.moveTo(UI_OSC_BOX_WAVEFORM_X,UI_OSC_BOX_WAVEFORM_Y);
    dial_detune_semi.moveTo(UI_OSC_DIAL_DETUNE_SEMI_X,UI_OSC_DIAL_DETUNE_SEMI_Y);
    dial_detune_cents.moveTo(UI_OSC_DIAL_DETUNE_CENTS_X,UI_OSC_DIAL_DETUNE_CENTS_Y);
    dial_pan.moveTo(UI_OSC_DIAL_PAN_X,UI_OSC_DIAL_PAN_Y);
    cb_freq_mod.moveTo(UI_OSC_BOX_MOD_FREQ_X,UI_OSC_BOX_MOD_FREQ_Y);
    cb_amp_mod.moveTo(UI_OSC_BOX_MOD_AMP_X,UI_OSC_BOX_MOD_AMP_Y);
    cb_phase_mod.moveTo(UI_OSC_BOX_MOD_PHASE_X,UI_OSC_BOX_MOD_PHASE_Y);
}

void OscillatorFrame::port_event(int port, float value){
    switch(port){
    case CTRL_OSC_STATUS:
        BWidgets::ValueableTyped<bool>* w = dynamic_cast<BWidgets::ValueableTyped<bool>*>(widget[CTRL_OSC_STATUS]);
        w->setValue(value);
        break;
    case CTRL_OSC_WAVEFORM:
        BWidgets::ValueableTyped<int>* w = dynamic_cast<BWidgets::ValueableTyped<int>*>(widget[CTRL_OSC_WAVEFORM]);
        w->setValue(value+1);
        break;
    case CTRL_OSC_GAIN:
        BWidgets::ValueableTyped<float>* w = dynamic_cast<BWidgets::ValueableTyped<float>*>(widget[CTRL_OSC_GAIN]);
        w->setValue(value);
        break;
    case CTRL_OSC_DETUNE:
        BWidgets::ValueableTyped<float>* w = dynamic_cast<BWidgets::ValueableTyped<float>*>(widget[CTRL_OSC_DETUNE]);
        w->setValue(std::floor(value));
        BWidgets::ValueableTyped<float>* w = dynamic_cast<BWidgets::ValueableTyped<float>*>(widget[CTRL_OSC_DETUNE + 1]);
        w->setValue(value - std::floor(value));
        break;
    case CTRL_OSC_PAN:
        BWidgets::ValueableTyped<float>* w = dynamic_cast<BWidgets::ValueableTyped<float>*>(widget[CTRL_OSC_PAN + 1]);
        w->setValue(std::floor(value));
        break;
    // modulators
    case CTRL_OSC_N: // amp
        
    case CTRL_OSC_N + 1: // freq

    case CTRL_OSC_N + 2: // phase
        


    }
}

std::array<BWidgets::Widget*,9> OscillatorFrame::getWidgetArray() const {
    return widget;
}
