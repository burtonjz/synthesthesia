#ifndef UI_ENV_WIDGETS_HPP_
#define UI_ENV_WIDGETS_HPP_

#include <array>

#include "ui-config.hpp"
#include "ui-object.hpp"
#include "../../BWidgets/BWidgets/ValueHSlider.hpp"

#include <string>
#include <cstdint>

class EnvelopeFrame : public ModuleFrame {
private:
    BWidgets::ValueHSlider slider_attack;
    BWidgets::ValueHSlider slider_decay;
    BWidgets::ValueHSlider slider_sustain;
    BWidgets::ValueHSlider slider_release;
    std::array<BWidgets::Widget*,4> widget;
public:
    /**
     * @brief EnvelopeFrame constructor
    */
    EnvelopeFrame();
    
    /**
     * @brief Virtual Class for UI Module Frames
     * 
     * @param URID object URID
     * @param title widget title
    */
    EnvelopeFrame(const uint32_t urid, const std::string& title);

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

    std::array<BWidgets::Widget*,4> getWidgetArray() const;
    
};

#endif // UI_ENV_WIDGETS_HPP_