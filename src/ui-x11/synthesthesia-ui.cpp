#include <iostream>
#include "cairo/cairo.h"

#include "synthesthesia-ui.hpp"
#include "../port-info.hpp"
#include "../config.hpp"
#include <stdexcept>




SynthesthesiaUI::SynthesthesiaUI(LV2UI_Write_Function write_function,LV2UI_Controller controller, void* parentXWindow, std::string bundlePath):
    Window(750, 600, reinterpret_cast<PuglNativeView>(parentXWindow),0, "Synthesthesia",false,PUGL_MODULE,0),
    write_function(write_function),
    controller(controller),
    frames_ptr{nullptr},
    oscillator(),
    envelope(create_env_array()),
    lfo(create_lfo_array()),
    filter(create_filter_array())
{
    setBackground(BStyles::Fill(bundlePath + "/background.png"));

    // configure frames
    int iFrame = 0;
    for(size_t i = 0; i < oscillator.size(); ++i ){
        frames_ptr[iFrame] = &oscillator[i];
        iFrame += 1;
    }
    for(size_t i = 0; i < envelope.size(); ++i ){
        frames_ptr[iFrame] = &envelope[i];
        iFrame += 1;
    }
    for(size_t i = 0; i < lfo.size(); ++i ){
        frames_ptr[iFrame] = &lfo[i];
        iFrame += 1;
    }
    for(size_t i = 0; i < filter.size(); ++i ){
        frames_ptr[iFrame] = &filter[i];
        iFrame += 1;
    }

    for(size_t i = 0; i < frames_ptr.size(); ++i){
        frames_ptr[i]->configure(0,i*BWIDGETS_DEFAULT_FRAME_HEIGHT);
    }

        
    // TODO: ADD INIT WIDGET FUNCTIONS
    // dial.setClickable(false);
    // dial.setCallbackFunction(BEvents::Event::EventType::valueChangedEvent, value_change_callback);
}

SynthesthesiaUI::~SynthesthesiaUI(){

}

std::array<OscillatorFrame,N_OSCILLATORS> SynthesthesiaUI::create_osc_array(){

}

std::array<EnvelopeFrame,N_ENVELOPES> SynthesthesiaUI::create_env_array(){

}
std::array<LfoFrame,N_LFOS> SynthesthesiaUI::create_lfo_array(){

}
std::array<FilterFrame,N_FILTERS> SynthesthesiaUI::create_filter_array(){

}


LV2UI_Widget SynthesthesiaUI::get_top_level_widget(){
    return reinterpret_cast<LV2UI_Widget>(puglGetNativeView(getView()));
}


void SynthesthesiaUI::port_event(uint32_t port_index,uint32_t buffer_size,uint32_t format,const void *buffer){
    if(format == 0){
        const float value = *static_cast<const float*>(buffer);
        const int control_port = port_index - MIDI_N - AUDIO_OUT_N;
        switch(port_index){
            case 2: 
                dial.setValue(value);
                break;
            default: break;
        }
    }
}

void SynthesthesiaUI::value_change_callback(BEvents::Event* event){
    BWidgets::ValueDial* d = dynamic_cast<BWidgets::ValueDial*>(event->getWidget());
    SynthesthesiaUI* ui = dynamic_cast<SynthesthesiaUI*>(d->getMainWindow());
    if (ui) {
        float gain = d->getValue();
        ui->write_function(ui->controller,2,sizeof(gain),0,&gain);
    }
}


void SynthesthesiaUI::init_oscillator_widgets(OscillatorWidgets osc, const double x, const double y){
    osc.button_on();
}

void SynthesthesiaUI::init_envelope_widgets(EnvelopeWidgets env, const double x, const double y){

}

void SynthesthesiaUI::init_lfo_widgets(LFOWidgets lfo, const double x, const double y){

}

void SynthesthesiaUI::init_filter_widgets(FilterWidgets filt, const double x, const double y){

}

