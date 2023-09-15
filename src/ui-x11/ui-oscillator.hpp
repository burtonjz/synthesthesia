#ifndef UI_OSC_WIDGETS_HPP_
#define UI_OSC_WIDGETS_HPP_

#include <array>

#include "ui-config.hpp"
#include "ui-object.hpp"
#include "ui-waveform.hpp"
#include "../module-type.hpp"

#include "../../BWidgets/BEvents/Event.hpp"
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
    void port_event(uint32_t port, float value) override;

    /**
     * @brief get port_index and float value for specified widget
     * 
     * @param relative_index relative port index
    */
    std::pair<uint32_t,float> get_callback_data(const uint32_t relative_index) override;
    
    /**
     * @brief returns the frame ModuleType
    */
    const ModuleType get_module_type() override;
};

#endif // UI_OSC_WIDGETS_HPP_