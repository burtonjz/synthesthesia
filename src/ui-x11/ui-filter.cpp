#include "ui-filter.hpp"
#include "ui-port-handler.hpp"
#include "../port-info.hpp"
#include "../filter-type.hpp"
#include "../cfg-connection.hpp"
#include "../../BWidgets/BWidgets/Supports/ValueableTyped.hpp"

// forward declaration (reqd for callback function)
class SynthesthesiaUI;

FilterFrame::FilterFrame():
    FilterFrame(URID("frame"),"")
{}

FilterFrame::FilterFrame(const uint32_t urid, const std::string& title):
    ModuleFrame(urid,title),
    cb_filter_type(FILTER_TYPE_STRINGS,1,URID("/combo-box"),"filter-type"),
    slider_cutoff(0.1, FilterLimits[CTRL_FILTER_FREQ].first, FilterLimits[CTRL_FILTER_FREQ].second,0.0,URID("/value-slider"),"cutoff-frequency"),
    dial_resonance(0.5, FilterLimits[CTRL_FILTER_RES].first, FilterLimits[CTRL_FILTER_RES].second,0.0,URID("/value-dial"),"cutoff-frequency"),
    cb_mod_cutoff({"None"},1,URID("/combo-box"),"cutoff-modulator"),
    cb_mod_resonance({"None"},1,URID("/combo-box"),"cutoff-modulator")
{
    slider_cutoff.setActivatable(false);
    dial_resonance.setClickable(false);
    dial_resonance.setActivatable(false);

    control_widget_.push_back(&cb_filter_type);
    control_widget_.push_back(&slider_cutoff);
    control_widget_.push_back(&dial_resonance);

    connection_widget_.push_back(&cb_mod_cutoff);
    connection_widget_.push_back(&cb_mod_resonance);

    for (auto& element : control_widget_) add(element);
    for (auto& element : connection_widget_) add(element);
}

void FilterFrame::configure(int x_index, int y_index){
    // configure frame positioning
    set_position(x_index,y_index);

    cb_filter_type.moveTo(UI_FLT_BOX_TYPE_X,UI_FLT_BOX_TYPE_Y);
    slider_cutoff.moveTo(UI_FLT_SLIDER_CUTOFF_X,UI_FLT_SLIDER_CUTOFF_Y);
    dial_resonance.moveTo(UI_FLT_DIAL_RES_X,UI_FLT_DIAL_RES_Y);
    cb_mod_cutoff.moveTo(UI_FLT_BOX_MOD_CUTOFF_X,UI_FLT_BOX_MOD_CUTOFF_Y);
    cb_mod_resonance.moveTo(UI_FLT_BOX_MOD_RES_X,UI_FLT_BOX_MOD_RES_Y);

    // fill in modulator lists
    for(int i = 0; i < N_ENVELOPES; ++i){
        std::string s = "Env " + std::to_string(i);
        cb_mod_cutoff.addItem(s);
        cb_mod_resonance.addItem(s);
    }
    for(int i = 0; i < N_LFOS; ++i){
        std::string s = "LFO " + std::to_string(i);
        cb_mod_cutoff.addItem(s);
        cb_mod_resonance.addItem(s);
    }
}

void FilterFrame::port_event(uint32_t port, float value){
    switch(port){
    case CTRL_FILTER_TYPE:
        dynamic_cast<BWidgets::ValueableTyped<int>*>(control_widget_[CTRL_FILTER_TYPE])->setValue(value);
        break;
    case CTRL_FILTER_FREQ:
        dynamic_cast<BWidgets::ValueableTyped<float>*>(control_widget_[CTRL_FILTER_FREQ])->setValue(value);
        break;
    case CTRL_FILTER_RES:
        dynamic_cast<BWidgets::ValueableTyped<float>*>(control_widget_[CTRL_FILTER_RES])->setValue(value);
        break;
    // mod connections
    case CTRL_FILTER_N + FILTER_CONNECT_FREQ:
        dynamic_cast<BWidgets::ValueableTyped<int>*>(&cb_mod_cutoff)->setValue(value);
        break; 
    case CTRL_FILTER_N + FILTER_CONNECT_Q:
        dynamic_cast<BWidgets::ValueableTyped<int>*>(&cb_mod_resonance)->setValue(value);
        break;
    default:
        break;
    }
}

std::pair<uint32_t,float> FilterFrame::get_callback_data(const uint32_t relative_index){
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

const ModuleType FilterFrame::get_module_type(){
    return ModuleType::FILTER;
} 