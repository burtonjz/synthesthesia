#include "ui-config.hpp"
#include "ui-lfo.hpp"
#include "../port-info.hpp"
#include "../waveform.hpp"

UILfoObject::UILfoObject(double x, double y, int index):
    UIObject(x,y,index),
    slider_freq(
        x+UI_LFO_SLIDER_FREQ_X,y+UI_LFO_SLIDER_FREQ_Y,UI_LFO_SLIDER_FREQ_WIDTH,UI_LFO_SLIDER_FREQ_HEIGHT,
        0.1, ctrlLimits[CTRL_LFO1_FREQ].first, ctrlLimits[CTRL_LFO1_FREQ].second
    ),
    slider_depth(
        x+UI_LFO_SLIDER_DEPTH_X,y+UI_LFO_SLIDER_DEPTH_Y,UI_LFO_SLIDER_DEPTH_WIDTH,UI_LFO_SLIDER_DEPTH_HEIGHT,
        0.1, ctrlLimits[CTRL_LFO1_DEPTH].first, ctrlLimits[CTRL_LFO1_DEPTH].second
    ),
    cb_waveform(
        x+UI_LFO_BOX_WAVEFORM_X,y+UI_LFO_BOX_WAVEFORM_Y,UI_LFO_BOX_WAVEFORM_WIDTH,UI_LFO_BOX_WAVEFORM_HEIGHT,
        WAVEFORM_STRINGS,1
    )
{
    slider_freq.setActivatable(false);
    slider_depth.setActivatable(false);

    widget[0] = &slider_freq;
    widget[1] = &slider_depth;
    widget[2] = &cb_waveform;
}

int UILfoObject::getNumWidgets() const {
    return 3;
}

double UILfoObject::getObjectHeight() const {
    return UI_LFO_HEIGHT;
}

double UILfoObject::getObjectWidth() const {
    return UI_LFO_WIDTH;
}

std::array<BWidgets::Widget*,3> UILfoObject::getWidgetArray() const {
    return widget;
}