#include <iostream>
#include "cairo/cairo.h"

#include "synthesthesia-ui.hpp"
#include "../config.hpp"
#include "../port-info.hpp"
#include "../module-type.hpp"

#include "../connection-manager.hpp"
#include "../cfg-connection.hpp"
#include "../config.hpp"

#include "../../BWidgets/BStyles/Theme.hpp"

#include <stdexcept>
#include <utility>
#include <vector>
#include <iostream>

SynthesthesiaUI::SynthesthesiaUI(LV2UI_Write_Function write_function,LV2UI_Controller controller, void* parentXWindow, std::string bundlePath):
    Window(BWIDGETS_DEFAULT_WINDOW_WIDTH, BWIDGETS_DEFAULT_WINDOW_HEIGHT, reinterpret_cast<PuglNativeView>(parentXWindow),0, "Synthesthesia",false,PUGL_MODULE,0),
    write_function(write_function),
    controller(controller),
    oscillator(),
    envelope(),
    lfo(),
    filter()
{
    // THEMING

    BStyles::Font defaultFont(
        "Nimbus Sans",
        CAIRO_FONT_SLANT_NORMAL,
        CAIRO_FONT_WEIGHT_NORMAL,
        18.0,
        BStyles::Font::TextAlign::left,
        BStyles::Font::TextVAlign::middle,
        1.5
    );

    BStyles::Theme theme = BStyles::Theme{
		{
			URID("/window"),
			BStyles::Style({
				{BURID(BSTYLES_STYLEPROPERTY_BACKGROUND_URI), BUtilities::makeAny<BStyles::Fill>(BStyles::Fill(bundlePath + "/assets/background.png"))},
				{BURID(BSTYLES_STYLEPROPERTY_BORDER_URI), BUtilities::makeAny<BStyles::Border>(BStyles::noBorder)}
			})
		},
        {
			URID("/frame"),
			BStyles::Style({
				{BURID(BSTYLES_STYLEPROPERTY_BACKGROUND_URI), BUtilities::makeAny<BStyles::Fill>(BStyles::noFill)},
				{BURID(BSTYLES_STYLEPROPERTY_BORDER_URI), BUtilities::makeAny<BStyles::Border>(BStyles::noBorder)}
			})
		},
        {
			URID("/value-dial/label"), 
			BStyles::Style(BURID(BSTYLES_STYLEPROPERTY_FONT_URI), BUtilities::makeAny<BStyles::Font>(defaultFont))
		},
        {
			URID("/value-slider/label"), 
			BStyles::Style(BURID(BSTYLES_STYLEPROPERTY_FONT_URI), BUtilities::makeAny<BStyles::Font>(defaultFont))
		},
        {
			URID("/combo-box"), 
			BStyles::Style({
                {BURID(BSTYLES_STYLEPROPERTY_BACKGROUND_URI), BUtilities::makeAny<BStyles::Fill>(BStyles::darkgreyFill)},
                {BURID(BSTYLES_STYLEPROPERTY_BORDER_URI), BUtilities::makeAny<BStyles::Border>(BStyles::blackBorder1pt)}
            })
		},
        {
            URID("/combo-box/label"),
            BStyles::Style(BURID(BSTYLES_STYLEPROPERTY_FONT_URI), BUtilities::makeAny<BStyles::Font>(defaultFont))
        },
        {
            URID("/combo-box/listbox/label"),
            BStyles::Style(BURID(BSTYLES_STYLEPROPERTY_FONT_URI), BUtilities::makeAny<BStyles::Font>(defaultFont))
        }
    };
     
    setTheme(theme);

    // CONFIGURE FRAMES

    int iFrame = 0;
    for(size_t i = 0; i < oscillator.size(); ++i ){
        oscillator[i].setTitle("oscillator-" + std::to_string(i+1));
        oscillator[i].set_instance(i);
        oscillator[i].configure(0,iFrame);
        set_frame_callbacks(&oscillator[i]);
        add(&oscillator[i]);
        iFrame += 1;
    }
    for(size_t i = 0; i < envelope.size(); ++i ){
        envelope[i].setTitle("envelope-" + std::to_string(i+1));
        envelope[i].set_instance(i);
        envelope[i].configure(0,iFrame);
        add(&envelope[i]);
        set_frame_callbacks(&envelope[i]);
        iFrame += 1;
    }
    for(size_t i = 0; i < lfo.size(); ++i ){
        lfo[i].setTitle("LFO-" + std::to_string(i+1));
        lfo[i].set_instance(i);
        lfo[i].configure(0,iFrame);
        add(&lfo[i]);
        set_frame_callbacks(&lfo[i]);
        iFrame += 1;
    }
    for(size_t i = 0; i < filter.size(); ++i ){
        filter[i].setTitle("filter-" + std::to_string(i+1));
        filter[i].set_instance(i);
        filter[i].configure(0,iFrame);
        add(&filter[i]);
        set_frame_callbacks(&filter[i]);
        iFrame += 1;
    }
}

SynthesthesiaUI::~SynthesthesiaUI(){
}

LV2UI_Widget SynthesthesiaUI::get_top_level_widget(){
    return reinterpret_cast<LV2UI_Widget>(puglGetNativeView(getView()));
}


void SynthesthesiaUI::port_event(uint32_t port_index,uint32_t buffer_size,uint32_t format,const void *buffer){
    return ;
    if(format == 0){
        const float value = *static_cast<const float*>(buffer);
        PortData port_data = PortHandler::get_port_data(port_index);

        switch(port_data.module_type){
        case ModuleType::OSCILLATOR:
            oscillator[port_data.instance].port_event(port_data.index,value);
            break;
        case ModuleType::ENVELOPE:
            if(port_data.index == CTRL_ENV_CONNECTIONS ) connection_port_event(port_data, value);
            else envelope[port_data.instance].port_event(port_data.index,value);
            break;
        case ModuleType::LFO:
            if(port_data.index == CTRL_LFO_CONNECTIONS ) connection_port_event(port_data, value);
            else lfo[port_data.instance].port_event(port_data.index,value);
            break;
        case ModuleType::FILTER:
            envelope[port_data.instance].port_event(port_data.index,value);
            break;
        default:
            break;
        }
    }
}

void SynthesthesiaUI::control_widget_value_changed_callback(BEvents::Event* event){
    int relative_index;
    BWidgets::Widget* widget = event->getWidget();

    SynthesthesiaUI* ui = dynamic_cast<SynthesthesiaUI*>(widget->getMainWindow());
    if (!ui) return ;

    // all widgets with callbacks belong to a ModuleFrame widget, which will be the parent widget
    ModuleFrame* parent = dynamic_cast<ModuleFrame*>(widget->getParent());
    if (!parent) return ;

    int i = 0;
    bool widget_found = false;
    std::vector<Widget*> control_widgets = parent->get_control_widgets();
    
    for(i = 0; i < control_widgets.size(); ++i ){
        if ( widget == control_widgets[i] ){
            widget_found = true;
            break;
        }
    }

    if (widget_found){
        std::pair<uint32_t,float> data = parent->get_callback_data(i);
        ui->write_function(ui->controller, data.first, sizeof(data.second), 0, &data.second );
        return ;
    }

}

void SynthesthesiaUI::connection_widget_value_changed_callback(BEvents::Event* event){
    std::array<ConnectionManager,N_MODULATORS> connection_manager;

    BWidgets::Widget* widget = event->getWidget();
    SynthesthesiaUI* ui = dynamic_cast<SynthesthesiaUI*>(widget->getMainWindow());
    if (!ui) return ;

    // loop through all modulatable widgets and add connections

    ModulatableType module_type = MODULATABLE_OSCILLATOR;
    for (int instance = 0; instance < oscillator.size(); ++instance ){
        std::vector<Widget*> widget = oscillator[instance].get_connection_widgets() ;
        for (int port = 0; port < widget.size(); ++port ){
            // subtract by two to account for null item and "none" item
            int modulator = dynamic_cast<BWidgets::ValueableTyped<double>*>(widget[port])->getValue() - 2; 
            if ( modulator >= 0 ){
                connection_manager[modulator].add_connection(module_type,instance,port);
            }
        }
    }

    module_type = MODULATABLE_FILTER;
    for (int instance = 0; instance < filter.size(); ++instance ){
        std::vector<Widget*> widget = filter[instance].get_connection_widgets() ;
        for (int port = 0; port < widget.size(); ++port ){
            // subtract by two to account for null item and "none" item
            int modulator = dynamic_cast<BWidgets::ValueableTyped<double>*>(widget[port])->getValue() - 2; 
            if ( modulator >= 0 ){
                connection_manager[modulator].add_connection(module_type,instance,port);
            }
        }
    }

    // set connections for each modulator
    for (int i = 0; i < connection_manager.size(); ++i ){
        if ( i < N_ENVELOPES ){
            int port_index = PORT_ENV_RANGE.first + i * CTRL_ENV_N + CTRL_ENV_CONNECTIONS ;
            float value = connection_manager[i].encode_as_float();
            ui->write_function(ui->controller, port_index , sizeof(value), 0, &value );
        }
    }
}


void SynthesthesiaUI::set_frame_callbacks(ModuleFrame* frame){
    for(auto& w : frame->get_control_widgets())
        w->setCallbackFunction(
            BEvents::Event::EventType::valueChangedEvent, 
            [this](BEvents::Event* event) {control_widget_value_changed_callback(event); }
        );
    for(auto& w : frame->get_connection_widgets()) 
        w->setCallbackFunction(
            BEvents::Event::EventType::valueChangedEvent, 
            [this](BEvents::Event* event) {connection_widget_value_changed_callback(event); }
        );    
}

void SynthesthesiaUI::connection_port_event(PortData port_data, float value){
    ConnectionManager connection_manager;
    connection_manager.set_data_from_float(value);

    for( Connection connection : connection_manager ){
        switch(connection.modulatable_type){
        case MODULATABLE_OSCILLATOR:
            dynamic_cast<BWidgets::ValueableTyped<int>*>(&oscillator[connection.instance])->setValue(connection.port + 2);
            break;
        case MODULATABLE_FILTER:
            dynamic_cast<BWidgets::ValueableTyped<int>*>(&filter[connection.instance])->setValue(connection.port + 2);
            break;
        default:
            break;
        }
    }
}