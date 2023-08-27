// Connectors to LV2 Interface
#include "synthesthesia-ui.hpp"

#define UI_PLUGIN_URI "https://github.com/burtonjz/synthesthesia#ui"

static LV2UI_Handle instantiate(
    const struct LV2UI_Descriptor *descriptor, 
    const char *plugin_uri, 
    const char *bundle_path, 
    LV2UI_Write_Function write_function, 
    LV2UI_Controller controller, 
    LV2UI_Widget *widget, 
    const LV2_Feature *const *features)
{
    // make sure we're called by the right plugin
    if(strcmp(plugin_uri,UI_PLUGIN_URI) != 0) return nullptr;

    void* parentXWindow = nullptr;
    for (int i = 0; features[i]; ++i){
        if (strcmp (features[i]->URI, LV2_UI__parent) == 0) parentXWindow = features[i]->data;
    }

    if (!parentXWindow){
        std::cerr << "Required feature LV2_UI__parent not provided" << std::endl;
        return nullptr;
    }

    SynthesthesiaUI* synth_ui;
    try {synth_ui = new SynthesthesiaUI(write_function,controller,parentXWindow,std::string(bundle_path));}
    catch(std::exception& exc){
        std::cerr << "UI instantiation failed." << std::endl;
        return nullptr;
    }

    *widget = synth_ui->get_top_level_widget();
    return static_cast<LV2UI_Handle> (synth_ui);

}

static void cleanup(LV2UI_Handle ui){
    SynthesthesiaUI* synth_ui = static_cast<SynthesthesiaUI*> (ui);
    if(synth_ui) delete synth_ui;
}

static void port_event(
    LV2UI_Handle ui, 
    uint32_t port_index, 
    uint32_t buffer_size, 
    uint32_t format, 
    const void *buffer)
{
    SynthesthesiaUI* synth_ui = static_cast<SynthesthesiaUI*> (ui);
    if(synth_ui) synth_ui->port_event(port_index,buffer_size,format,buffer);
}

static int ui_idle(LV2UI_Handle ui){
    SynthesthesiaUI* synth_ui = static_cast<SynthesthesiaUI*> (ui);
    synth_ui->handleEvents();
    return 0;
}

static const void *extension_data(const char *uri){
    static const LV2UI_Idle_Interface idle = { ui_idle };
    if (strcmp (uri, LV2_UI__idleInterface) == 0) return &idle;
    return nullptr;
}

static const LV2UI_Descriptor ui_descriptor =
{
    UI_PLUGIN_URI,
    instantiate,
    cleanup,
    port_event,
    extension_data
};

LV2_SYMBOL_EXPORT const LV2UI_Descriptor* lv2ui_descriptor(uint32_t index){
    switch(index){
        case 0: return &ui_descriptor;
        default: return 0;
    }
}