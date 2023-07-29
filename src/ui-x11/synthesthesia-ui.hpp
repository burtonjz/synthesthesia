#ifndef UI_HPP_
#define UI_HPP_

#include <string>
#include <lv2/lv2plug.in/ns/lv2core/lv2.h>
#include <lv2/lv2plug.in/ns/extensions/ui/ui.h>

#include "../../BWidgets/BWidgets/Window.hpp"
// #include "../../BWidgets/BWidgets/Button.hpp"
// #include "../../BWidgets/BWidgets/ComboBox.hpp"
// #include "../../BWidgets/BWidgets/ValueDial.hpp"
// #include "../../BWidgets/BWidgets/ValueHSlider.hpp"
// #include "../../BWidgets/BWidgets/Dial.hpp"

class SynthesthesiaUI : public BWidgets::Window {
private:
    LV2UI_Write_Function write_function;
    LV2UI_Controller controller;
    BWidgets::ValueDial dial;

public:
    SynthesthesiaUI(LV2UI_Write_Function write_function,LV2UI_Controller controller, void* parentXWindow, std::string bundlePath);
    ~SynthesthesiaUI();
    LV2UI_Widget get_top_level_widget();
    void port_event(uint32_t port_index,uint32_t buffer_size,uint32_t format,const void *buffer);
    static void value_change_callback(BEvents::Event* event);
};

#endif // UI_HPP_