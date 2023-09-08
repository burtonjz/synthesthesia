#include "ui-filter.hpp"
#include "../port-info.hpp"
#include "../filter-type.hpp"
#include "../cfg-connection.hpp"
#include "../../BWidgets/BWidgets/Supports/ValueableTyped.hpp"

FilterFrame::FilterFrame():
    FilterFrame(BUTILITIES_URID_UNKNOWN_URID,"")
{}

FilterFrame::FilterFrame(const uint32_t urid, const std::string& title):
    ModuleFrame(urid,title),
    cb_filter_type(FILTER_TYPE_STRINGS,1),
    slider_cutoff(0.1, FilterLimits[CTRL_FILTER_FREQ].first, FilterLimits[CTRL_FILTER_FREQ].second),
    dial_resonance(0.5, FilterLimits[CTRL_FILTER_RES].first, FilterLimits[CTRL_FILTER_RES].second),
    cb_mod_cutoff({"None"},1),
    cb_mod_resonance({"None"},1)
{
    slider_cutoff.setActivatable(false);
    dial_resonance.setClickable(false);
    dial_resonance.setActivatable(false);

    widget[0] = &cb_filter_type;
    widget[1] = &slider_cutoff;
    widget[2] = &dial_resonance;
    widget[3] = &cb_mod_cutoff;
    widget[4] = &cb_mod_resonance;

    for(auto& element : widget) add(element);
}

void FilterFrame::configure(int x, int y){
    moveTo(x,y);

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

void FilterFrame::port_event(int port, float value){
    switch(port){
    case CTRL_FILTER_TYPE:
        dynamic_cast<BWidgets::ValueableTyped<int>*>(widget[CTRL_FILTER_TYPE])->setValue(value);
        break;
    case CTRL_FILTER_FREQ:
        dynamic_cast<BWidgets::ValueableTyped<float>*>(widget[CTRL_FILTER_FREQ])->setValue(value);
        break;
    case CTRL_FILTER_RES:
        dynamic_cast<BWidgets::ValueableTyped<float>*>(widget[CTRL_FILTER_RES])->setValue(value);
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

std::array<BWidgets::Widget*,5> FilterFrame::getWidgetArray() const {
    return widget;
}