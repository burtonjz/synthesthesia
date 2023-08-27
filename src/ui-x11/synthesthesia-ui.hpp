#ifndef UI_HPP_
#define UI_HPP_

#include <string>
#include <array>
#include <lv2/lv2plug.in/ns/lv2core/lv2.h>
#include <lv2/lv2plug.in/ns/extensions/ui/ui.h>

#include "ui-config.hpp"

#include "../../BWidgets/BWidgets/Window.hpp"

#include "ui-object.hpp"
#include "ui-oscillator.hpp"
#include "ui-filter.hpp"
#include "ui-envelope.hpp"
#include "ui-lfo.hpp"

class SynthesthesiaUI : public BWidgets::Window {
private:
    LV2UI_Write_Function write_function;
    LV2UI_Controller controller;

    std::array<ModuleFrame*,N_MODULES> frames_ptr;
    std::array<OscillatorFrame,N_OSCILLATORS> oscillator;
    std::array<EnvelopeFrame,N_ENVELOPES> envelope;
    std::array<LfoFrame,N_LFOS> lfo;
    std::array<FilterFrame,N_FILTERS> filter;
    
public:
    SynthesthesiaUI(LV2UI_Write_Function write_function,LV2UI_Controller controller, void* parentXWindow, std::string bundlePath);
    ~SynthesthesiaUI();

    std::array<OscillatorFrame,N_OSCILLATORS> create_osc_array();
    std::array<EnvelopeFrame,N_ENVELOPES> create_env_array();
    std::array<LfoFrame,N_LFOS> create_lfo_array();
    std::array<FilterFrame,N_FILTERS> create_filter_array();

    LV2UI_Widget get_top_level_widget();
    void port_event(uint32_t port_index,uint32_t buffer_size,uint32_t format,const void *buffer);
    static void value_change_callback(BEvents::Event* event);
};

#endif // UI_HPP_