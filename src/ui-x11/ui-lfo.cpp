#include "ui-lfo.hpp"
#include "ui-port-handler.hpp"
#include "../port-info.hpp"
#include "../waveform.hpp"
#include "../../BWidgets/BWidgets/Supports/ValueableTyped.hpp"

// forward declaration (reqd for callback function)
class SynthesthesiaUI;

LfoFrame::LfoFrame():
    LfoFrame(URID("frame"),"")
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

    
    control_widget_.push_back(&slider_freq);
    control_widget_.push_back(&slider_depth);
    control_widget_.push_back(&cb_waveform);
    control_widget_.push_back(&image_waveform);

    for(auto& element : control_widget_) add(element);
}

void LfoFrame::configure(int x_index, int y_index){
    // configure frame positioning
    set_position(x_index,y_index);

    slider_freq.moveTo(UI_LFO_SLIDER_FREQ_X,UI_LFO_SLIDER_FREQ_Y);
    slider_depth.moveTo(UI_LFO_SLIDER_DEPTH_X,UI_LFO_SLIDER_DEPTH_Y);
    cb_waveform.moveTo(UI_LFO_BOX_WAVEFORM_X,UI_LFO_BOX_WAVEFORM_Y);
    image_waveform.moveTo(UI_LFO_WAVEFORM_IMAGE_X,UI_LFO_WAVEFORM_IMAGE_Y);
}

void LfoFrame::port_event(uint32_t port, float value){
    switch(port){
    case CTRL_LFO_CONNECTIONS:
        break; // connections will be handled by what they are connected to
    case CTRL_LFO_WAVEFORM:
        dynamic_cast<BWidgets::ValueableTyped<int>*>(control_widget_[CTRL_LFO_WAVEFORM])->setValue(value);
        break;
    case CTRL_LFO_FREQ:
        dynamic_cast<BWidgets::ValueableTyped<float>*>(control_widget_[CTRL_LFO_FREQ])->setValue(value);
        break;
    case CTRL_LFO_DEPTH:
        dynamic_cast<BWidgets::ValueableTyped<float>*>(control_widget_[CTRL_LFO_DEPTH])->setValue(value);
        break;
    default:
        break;
    }
}

std::pair<uint32_t,float> LfoFrame::get_callback_data(const uint32_t relative_index){
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

const ModuleType LfoFrame::get_module_type(){
    return ModuleType::LFO;
} 
