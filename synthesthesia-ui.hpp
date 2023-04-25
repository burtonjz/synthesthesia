#ifndef UI_HPP_
#define UI_HPP_

#include <lv2/lv2plug.in/ns/lv2core/lv2.h>
#include <lv2/lv2plug.in/ns/extensions/ui/ui.h>
#include <gtk/gtk.h>

class SynthesthesiaUI{
private:
    LV2UI_Write_Function write_function;
    LV2UI_Controller controller;
public:
    SynthesthesiaUI(LV2UI_Write_Function write_function,LV2UI_Controller controller);
    LV2UI_Widget get_main();
    static void port_event(uint32_t port_index,uint32_t buffer_size,uint32_t format,const void *buffer);
};

#endif // UI_HPP_