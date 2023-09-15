#include "ui-oscillator.hpp"
#include "ui-port-handler.hpp"
#include "../port-info.hpp"
#include "../waveform.hpp"
#include "../cfg-connection.hpp"

#include "../../BWidgets/BWidgets/Supports/ValueableTyped.hpp"

#include <cmath>
#include <string>

// forward declaration (reqd for callback function)
class SynthesthesiaUI;

OscillatorFrame::OscillatorFrame():
    OscillatorFrame(URID("frame"),"")
{}

OscillatorFrame::OscillatorFrame(const uint32_t urid, const std::string& title):
    ModuleFrame(urid,title),
    switch_on(true, true,BUTILITIES_URID_UNKNOWN_URID,"oscillator-status"),
    dial_gain(0.5, OscLimits[CTRL_OSC_GAIN].first, OscLimits[CTRL_OSC_GAIN].second,0.0,URID("/value-dial"),"gain"),
    cb_waveform(WAVEFORM_STRINGS,1,URID("/combo-box"),"waveform"),
    dial_detune_semi(0, -24, 24, 0.0, URID("/value-dial"),"detune-semitone"),
    dial_detune_cents(0.0, -0.99, 0.99, 0.0, URID("/value-dial"),"detune-cents"),
    dial_pan(0.0, OscLimits[CTRL_OSC_PAN].first, OscLimits[CTRL_OSC_PAN].second,0.0,URID("/value-dial"),"pan"),
    cb_amp_mod({"None"},1,URID("/combo-box"),"amplitude-modulator"),
    cb_freq_mod({"None"},1,URID("/combo-box"),"frequency-modulator"),
    cb_phase_mod({"None"},1,URID("/combo-box"),"phase-modulator"),
    image_waveform(URID("/image"),"waveform-image")
{
    setDraggable(false);

    dial_gain.setClickable(false);
    dial_gain.setActivatable(false);
    dial_pan.setClickable(false);
    dial_pan.setActivatable(false);
    dial_detune_semi.setClickable(false);
    dial_detune_semi.setActivatable(false);
    dial_detune_cents.setClickable(false);
    dial_detune_cents.setActivatable(false);

    image_waveform.createImage(BStyles::Status::normal);
    image_waveform.draw(static_cast<Waveform>(cb_waveform.getValue()-1));

    control_widget_.push_back(&switch_on);
    control_widget_.push_back(&cb_waveform);
    control_widget_.push_back(&dial_gain);
    control_widget_.push_back(&dial_detune_semi);
    control_widget_.push_back(&dial_detune_cents);
    control_widget_.push_back(&dial_pan);

    connection_widget_.push_back(&cb_amp_mod);
    connection_widget_.push_back(&cb_freq_mod);
    connection_widget_.push_back(&cb_phase_mod);
    
    add(&image_waveform);

    for (auto& element : control_widget_) add(element);
    for (auto& element : connection_widget_) add(element);

}

void OscillatorFrame::configure(int x_index, int y_index){
    // configure frame positioning
    set_position(x_index,y_index);

    // Move widgets within frame
    switch_on.moveTo(UI_OSC_SWITCH_ON_X, UI_OSC_SWITCH_ON_Y);
    dial_gain.moveTo(UI_OSC_DIAL_GAIN_X, UI_OSC_DIAL_GAIN_Y);
    cb_waveform.moveTo(UI_OSC_BOX_WAVEFORM_X, UI_OSC_BOX_WAVEFORM_Y);
    dial_detune_semi.moveTo(UI_OSC_DIAL_DETUNE_SEMI_X, UI_OSC_DIAL_DETUNE_SEMI_Y);
    dial_detune_cents.moveTo(UI_OSC_DIAL_DETUNE_CENTS_X, UI_OSC_DIAL_DETUNE_CENTS_Y);
    dial_pan.moveTo(UI_OSC_DIAL_PAN_X, UI_OSC_DIAL_PAN_Y);
    cb_freq_mod.moveTo(UI_OSC_BOX_MOD_FREQ_X, UI_OSC_BOX_MOD_FREQ_Y);
    cb_amp_mod.moveTo(UI_OSC_BOX_MOD_AMP_X, UI_OSC_BOX_MOD_AMP_Y);
    cb_phase_mod.moveTo(UI_OSC_BOX_MOD_PHASE_X, UI_OSC_BOX_MOD_PHASE_Y);
    image_waveform.moveTo(UI_OSC_WAVEFORM_IMAGE_X, UI_OSC_WAVEFORM_IMAGE_Y);

    // fill in modulator lists
    for(int i = 1; i <= N_ENVELOPES; ++i){
        std::string s = "Env " + std::to_string(i);
        cb_freq_mod.addItem(s);
        cb_amp_mod.addItem(s);
        cb_phase_mod.addItem(s);
    }
    for(int i = 1; i <= N_LFOS; ++i){
        std::string s = "LFO " + std::to_string(i);
        cb_freq_mod.addItem(s);
        cb_amp_mod.addItem(s);
        cb_phase_mod.addItem(s);
    }

}

void OscillatorFrame::port_event(uint32_t port, float value){
    switch(port){
    case CTRL_OSC_STATUS:
        dynamic_cast<BWidgets::ValueableTyped<bool>*>(control_widget_[CTRL_OSC_STATUS])->setValue(value);
        break;
    case CTRL_OSC_WAVEFORM:
        dynamic_cast<BWidgets::ValueableTyped<int>*>(control_widget_[CTRL_OSC_WAVEFORM])->setValue(value+1);
        break;
    case CTRL_OSC_GAIN:
        dynamic_cast<BWidgets::ValueableTyped<float>*>(control_widget_[CTRL_OSC_GAIN])->setValue(value);
        break;
    case CTRL_OSC_DETUNE:
        dynamic_cast<BWidgets::ValueableTyped<float>*>(control_widget_[CTRL_OSC_DETUNE])->setValue(std::floor(value));
        dynamic_cast<BWidgets::ValueableTyped<float>*>(control_widget_[CTRL_OSC_DETUNE + 1])->setValue(value - std::floor(value));
        break;
    case CTRL_OSC_PAN:
        dynamic_cast<BWidgets::ValueableTyped<float>*>(control_widget_[CTRL_OSC_PAN + 1])->setValue(std::floor(value));
        break;
    // modulators
    case CTRL_OSC_N + OSC_CONNECT_AMP: 
        dynamic_cast<BWidgets::ValueableTyped<int>*>(&cb_amp_mod)->setValue(value);
        break;
    case CTRL_OSC_N + OSC_CONNECT_FREQ:
        dynamic_cast<BWidgets::ValueableTyped<int>*>(&cb_freq_mod)->setValue(value);
        break;
    case CTRL_OSC_N + OSC_CONNECT_PHASE:
        dynamic_cast<BWidgets::ValueableTyped<int>*>(&cb_phase_mod)->setValue(value);
        break;
    default:
        break;
    }
}

std::pair<uint32_t,float> OscillatorFrame::get_callback_data(const uint32_t relative_index){
    std::pair<uint32_t,float> data;
    data.first = PortHandler::get_port(get_module_type(),get_instance(),relative_index);
    
    if ( control_widget_[relative_index]->getUrid() == URID("/combo-box") ){
        // combo box values need to be shifted by one as null takes 0
        data.second = dynamic_cast<BWidgets::ValueableTyped<double>*>(control_widget_[relative_index])->getValue() - 1;
    } else {
        data.second = dynamic_cast<BWidgets::ValueableTyped<double>*>(control_widget_[relative_index])->getValue();
    }

    return data;
}

const ModuleType OscillatorFrame::get_module_type(){
    return ModuleType::OSCILLATOR;
} 
