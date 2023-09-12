#ifndef UI_OSC_WIDGETS_HPP_
#define UI_OSC_WIDGETS_HPP_

#include <array>

#include "ui-config.hpp"

#include "ui-object.hpp"
#include "ui-waveform.hpp"
#include "../../BWidgets/BWidgets/HSwitch.hpp"
#include "../../BWidgets/BWidgets/ComboBox.hpp"
#include "../../BWidgets/BWidgets/ValueDial.hpp"

#include <string>
#include <cstdint>

class OscillatorFrame : public ModuleFrame {
private:
    BWidgets::HSwitch switch_on;
    BWidgets::ValueDial dial_gain;
    BWidgets::ComboBox cb_waveform;
    BWidgets::ValueDial dial_detune_semi;
    BWidgets::ValueDial dial_detune_cents;
    BWidgets::ValueDial dial_pan;
    BWidgets::ComboBox cb_freq_mod;
    BWidgets::ComboBox cb_amp_mod;
    BWidgets::ComboBox cb_phase_mod;
    WaveformImage image_waveform;
    std::array<BWidgets::Widget*,10> widget;
    
public:
    /**
     * @brief OscillatorFrame constructor
     * 
    */
    OscillatorFrame();

    /**
     * @brief Virtual Class for UI Module Frames
     * 
     * @param URID object URID
     * @param title widget title
    */
    OscillatorFrame(const uint32_t urid, const std::string& title);

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
    
    std::array<BWidgets::Widget*,10> getWidgetArray() const;
};

#endif // UI_OSC_WIDGETS_HPP_