#ifndef UI_FLT_WIDGETS_HPP_
#define UI_FLT_WIDGETS_HPP_

#include <array>
#include "ui-config.hpp"

#include "ui-object.hpp"
#include "../../BWidgets/BWidgets/ValueHSlider.hpp"
#include "../../BWidgets/BWidgets/ValueDial.hpp"
#include "../../BWidgets/BWidgets/ComboBox.hpp"

class FilterFrame : public ModuleFrame {
private:
    BWidgets::ComboBox cb_filter_type;
    BWidgets::ValueHSlider slider_cutoff;
    BWidgets::ValueDial dial_resonance;
    BWidgets::ComboBox cb_mod_cutoff;
    BWidgets::ComboBox cb_mod_resonance;
    std::array<BWidgets::Widget*,5> widget;
public:
    /**
     * @brief LfoFrame constructor
     * 
    */
    FilterFrame();

    /**
     * @brief position frame and widgets
     * 
     * @param x integer top-left x coordinate
     * @param y integer top-left y coordinate
    */
    void configure(int x, int y) override;

    /**
     * @brief update widgets based off port value changes
     * 
     * @param port port index relative to frame
     * @param value update value for widget
    */
    void port_event(int port, float value) override;

    std::array<BWidgets::Widget*,5> getWidgetArray() const;
};

#endif // UI_FLT_WIDGETS_HPP_