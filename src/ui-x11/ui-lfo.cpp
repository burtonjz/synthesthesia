#include "ui-lfo.hpp"
#include "../port-info.hpp"
#include "../waveform.hpp"
#include "../../BWidgets/BWidgets/Supports/ValueableTyped.hpp"


LfoFrame::LfoFrame():
    LfoFrame(BUTILITIES_URID_UNKNOWN_URID,"")
{}

LfoFrame::LfoFrame(const uint32_t urid, const std::string& title):
    ModuleFrame(urid,title),
    slider_freq(0.1, LfoLimits[CTRL_LFO_FREQ].first, LfoLimits[CTRL_LFO_FREQ].second,0.0,URID("/value-slider"),"frequency"),
    slider_depth(0.1, LfoLimits[CTRL_LFO_DEPTH].first, LfoLimits[CTRL_LFO_DEPTH].second,0.0,URID("/value-slider"),"depth"),
    cb_waveform(WAVEFORM_STRINGS,1,URID("/combo-box"),"waveform"),
    image_waveform(URID("/image"),"waveform-image")
{
    setDraggable(false);

    slider_freq.setActivatable(false);
    slider_depth.setActivatable(false);

    image_waveform.createImage(BStyles::Status::normal);
    image_waveform.draw(static_cast<Waveform>(cb_waveform.getValue()-1));

    
    widget[0] = &slider_freq;
    widget[1] = &slider_depth;
    widget[2] = &cb_waveform;
    widget[3] = &image_waveform;

    for(auto& element : widget) add(element);
}

void LfoFrame::configure(int x_index, int y_index){
    // configure frame positioning
    set_position(x_index,y_index);

    slider_freq.moveTo(UI_LFO_SLIDER_FREQ_X,UI_LFO_SLIDER_FREQ_Y);
    slider_depth.moveTo(UI_LFO_SLIDER_DEPTH_X,UI_LFO_SLIDER_DEPTH_Y);
    cb_waveform.moveTo(UI_LFO_BOX_WAVEFORM_X,UI_LFO_BOX_WAVEFORM_Y);
    image_waveform.moveTo(UI_LFO_WAVEFORM_IMAGE_X,UI_LFO_WAVEFORM_IMAGE_Y);
}

void LfoFrame::port_event(int port, float value){
    switch(port){
    case CTRL_LFO_CONNECTIONS:
        break; // connections will be handled by what they are connected to
    case CTRL_LFO_WAVEFORM:
        dynamic_cast<BWidgets::ValueableTyped<int>*>(widget[CTRL_LFO_WAVEFORM])->setValue(value);
        break;
    case CTRL_LFO_FREQ:
        dynamic_cast<BWidgets::ValueableTyped<float>*>(widget[CTRL_LFO_FREQ])->setValue(value);
        break;
    case CTRL_LFO_DEPTH:
        dynamic_cast<BWidgets::ValueableTyped<float>*>(widget[CTRL_LFO_DEPTH])->setValue(value);
        break;
    default:
        break;
    }
}

std::array<BWidgets::Widget*,4> LfoFrame::getWidgetArray() const {
    return widget;
}