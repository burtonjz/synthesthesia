#include "ui-filter.hpp"
#include "../port-info.hpp"
#include "../filter-type.hpp"

FilterFrame::FilterFrame():
    cb_filter_type(FILTER_TYPE_STRINGS,1),
    slider_cutoff(0.1, FilterLimits[CTRL_FILTER_FREQ].first, FilterLimits[CTRL_FILTER_FREQ].second),
    dial_resonance(0.5, FilterLimits[CTRL_FILTER_RES].first, FilterLimits[CTRL_FILTER_RES].second
    )
{
    slider_cutoff.setActivatable(false);
    dial_resonance.setClickable(false);
    dial_resonance.setActivatable(false);

    widget[0] = &cb_filter_type;
    widget[1] = &slider_cutoff;
    widget[2] = &dial_resonance;

    for(auto& element : widget) add(element);
}

void FilterFrame::configure(int x, int y){
    moveTo(x,y);

    cb_filter_type.moveTo(UI_FLT_BOX_TYPE_X,UI_FLT_BOX_TYPE_Y);
    slider_cutoff.moveTo(UI_FLT_SLIDER_CUTOFF_X,UI_FLT_SLIDER_CUTOFF_Y);
    dial_resonance.moveTo(UI_FLT_DIAL_RES_X,UI_FLT_DIAL_RES_Y);
}

std::array<BWidgets::Widget*,3> FilterFrame::getWidgetArray() const {
    return widget;
}