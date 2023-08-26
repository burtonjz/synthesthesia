#include "ui-config.hpp"
#include "ui-filter.hpp"
#include "../port-info.hpp"
#include "../filter-type.hpp"

UIFilterObject::UIFilterObject(double x, double y, int index):
    UIObject(x,y,index),
    cb_filter_type(
        x+UI_FLT_BOX_TYPE_X,y+UI_FLT_BOX_TYPE_Y,UI_FLT_BOX_TYPE_WIDTH,UI_FLT_BOX_TYPE_HEIGHT,
        FILTER_TYPE_STRINGS,1
    ),
    slider_cutoff(
        x+UI_FLT_SLIDER_CUTOFF_X,y+UI_FLT_SLIDER_CUTOFF_Y,UI_FLT_SLIDER_CUTOFF_WIDTH,UI_FLT_SLIDER_CUTOFF_HEIGHT,
        0.1, ctrlLimits[CTRL_FILTER1_FREQ].first, ctrlLimits[CTRL_FILTER1_FREQ].second
    ),
    dial_resonance(
        x+UI_FLT_DIAL_RES_X,y+UI_FLT_DIAL_RES_Y,UI_FLT_DIAL_RES_WIDTH,UI_FLT_DIAL_RES_HEIGHT,
        0.5, ctrlLimits[CTRL_FILTER1_RES].first, ctrlLimits[CTRL_FILTER1_RES].second
    )
{
    slider_cutoff.setActivatable(false);
    dial_resonance.setClickable(false);
    dial_resonance.setActivatable(false);

    widget[0] = &cb_filter_type;
    widget[1] = &slider_cutoff;
    widget[2] = &dial_resonance;
}

int UIFilterObject::getNumWidgets() const {
    return 3;
}

double UIFilterObject::getObjectHeight() const {
    return UI_FLT_HEIGHT;
}

double UIFilterObject::getObjectWidth() const {
    return UI_FLT_WIDTH;
}

std::array<BWidgets::Widget*,3> UIFilterObject::getWidgetArray() const {
    return widget;
}