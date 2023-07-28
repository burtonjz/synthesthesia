#ifndef UI_LFO_WIDGETS_HPP_
#define UI_LFO_WIDGETS_HPP_

#include <array>
#include "ui-object.hpp"
#include "../../BWidgets/BWidgets/ValueHSlider.hpp"
#include "../../BWidgets/BWidgets/ComboBox.hpp"


class UILfoObject : public UIObject {
private:
    BWidgets::ValueHSlider slider_freq;
    BWidgets::ValueHSlider slider_depth;
    BWidgets::ComboBox cb_waveform;
    std::array<BWidgets::Widget*,3> widget;
public:
    /**
     * @brief UILfoObject constructor
     * 
     * @param x The x-coordinate corresponding with the top-left corner of the object
     * @param y The y-coordinate corresponding with the top-left corner of the object
     * @param index The URID to be used for the first BWidget within the object
    */
    UILfoObject(double x, double y, int index);

    int getNumWidgets() const override;
    double getObjectHeight() const override;
    double getObjectWidth() const override;
    std::array<BWidgets::Widget*,3> getWidgetArray() const;
};

#endif // UI_LFO_WIDGETS_HPP_