#ifndef UI_LFO_WIDGETS_HPP_
#define UI_LFO_WIDGETS_HPP_

#include <array>
#include "ui-config.hpp"

#include "ui-object.hpp"
#include "../../BWidgets/BWidgets/ValueHSlider.hpp"
#include "../../BWidgets/BWidgets/ComboBox.hpp"

#include <string>
#include <cstdint>

class LfoFrame : public ModuleFrame {
private:
    BWidgets::ValueHSlider slider_freq;
    BWidgets::ValueHSlider slider_depth;
    BWidgets::ComboBox cb_waveform;
    std::array<BWidgets::Widget*,3> widget;
public:
    /**
     * @brief LfoFrame constructor
     * 
    */
    LfoFrame();

    /**
     * @brief Virtual Class for UI Module Frames
     * 
     * @param URID object URID
     * @param title widget title
    */
    LfoFrame(const uint32_t urid, const std::string& title);

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

    std::array<BWidgets::Widget*,3> getWidgetArray() const;
};

#endif // UI_LFO_WIDGETS_HPP_