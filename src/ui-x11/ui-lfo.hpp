#ifndef UI_LFO_WIDGETS_HPP_
#define UI_LFO_WIDGETS_HPP_

#include "ui-config.hpp"

#include "ui-object.hpp"
#include "ui-waveform.hpp"
#include "../../BWidgets/BWidgets/ValueHSlider.hpp"
#include "../../BWidgets/BWidgets/ComboBox.hpp"

#include <array>
#include <string>
#include <cstdint>

class LfoFrame : public ModuleFrame {
private:
    BWidgets::ValueHSlider slider_freq;
    BWidgets::ValueHSlider slider_depth;
    BWidgets::ComboBox cb_waveform;
    std::array<BWidgets::Widget*,4> widget;
    WaveformImage image_waveform;
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
     * @param x_index frame x position index. Always 0 for full rack, 1 for half rack
     * @param y_index frame y position index. 0 corresponds to the top left corner
    */
    void configure(int x_index, int y_index) override;

    /**
     * @brief update widgets based off port value changes
     * 
     * @param port port index relative to frame
     * @param value update value for widget
    */
    void port_event(int port, float value) override;

    std::array<BWidgets::Widget*,4> getWidgetArray() const;
};

#endif // UI_LFO_WIDGETS_HPP_