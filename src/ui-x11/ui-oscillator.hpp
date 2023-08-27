#ifndef UI_OSC_WIDGETS_HPP_
#define UI_OSC_WIDGETS_HPP_

#include <array>

#include "ui-config.hpp"

#include "ui-object.hpp"
#include "../../BWidgets/BWidgets/HSwitch.hpp"
#include "../../BWidgets/BWidgets/ComboBox.hpp"
#include "../../BWidgets/BWidgets/ValueDial.hpp"

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
    std::array<BWidgets::Widget*,9> widget;
public:
    /**
     * @brief OscillatorFrame constructor
     * 
    */
    OscillatorFrame();

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
    
    std::array<BWidgets::Widget*,9> getWidgetArray() const;
};

#endif // UI_OSC_WIDGETS_HPP_