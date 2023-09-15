#ifndef UI_HPP_
#define UI_HPP_

#include <string>
#include <array>

#include <lv2/lv2plug.in/ns/lv2core/lv2.h>
#include <lv2/lv2plug.in/ns/extensions/ui/ui.h>

#include "ui-config.hpp"
#include "ui-object.hpp"
#include "ui-oscillator.hpp"
#include "ui-filter.hpp"
#include "ui-envelope.hpp"
#include "ui-lfo.hpp"
#include "ui-port-handler.hpp"

#include "../../BWidgets/BWidgets/Window.hpp"
#include "../../BWidgets/BWidgets/Widget.hpp"

class SynthesthesiaUI : public BWidgets::Window {
private:
    LV2UI_Write_Function write_function;
    LV2UI_Controller controller;

    std::array<OscillatorFrame,N_OSCILLATORS> oscillator;
    std::array<EnvelopeFrame,N_ENVELOPES> envelope;
    std::array<LfoFrame,N_LFOS> lfo;
    std::array<FilterFrame,N_FILTERS> filter;
    
public:
    /**
     * @brief UI Constructor. Is a BWidgets::Window and contains LV2 hooks to communicatie with host
     * 
     * @param write_function Host-provided LV2 function to send data to input ports
     * @param controller 
    */
    SynthesthesiaUI(LV2UI_Write_Function write_function,LV2UI_Controller controller, void* parentXWindow, std::string bundlePath);
    ~SynthesthesiaUI();

    LV2UI_Widget get_top_level_widget();
    void port_event(uint32_t port_index,uint32_t buffer_size,uint32_t format,const void *buffer);

    /**
     * @brief callback function for value changes for control widgets
     * 
     * @param event event object
    */
    void control_widget_value_changed_callback(BEvents::Event* event);

    /**
     * @brief callback function for value changes in modulator connection widgets
     * 
     * Unlike control widgets, which have a 1:1 relationship to control ports, any time
     * a connection widget has a value change, we will re-calculate all connections.
     * 
     * @param event event object
    */
    void connection_widget_value_changed_callback(BEvents::Event* event);

    /**
     * @brief sets callback functions for all widgets (control + connection) within a given ModuleFrame
     * 
     * @param frame ModuleFrame
    */
    void set_frame_callbacks(ModuleFrame* frame);

    /**
     * @brief handles connection port events, updates values of modulation ComboBoxes
     * 
     * @param port_data PortData
     * @param value value
    */
    void connection_port_event(PortData port_data, float value);
};

#endif // UI_HPP_