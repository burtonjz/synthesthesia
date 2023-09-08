#include "ui-config.hpp"
#include "get-project-path.hpp"
#include "ui-oscillator.hpp"
#include "ui-filter.hpp"
#include "ui-envelope.hpp"
#include "ui-lfo.hpp"

#include "../../BWidgets/BWidgets/Window.hpp"

#include <iostream>

/*
BUILDING TEST:

for x in test ui-object ui-envelope ui-filter ui-lfo ui-oscillator; do g++ -fPIC -DPIC `pkg-config --cflags x11 cairo` -IBWidgets/include src/ui-x11/${x}.cpp -c -o src/ui-x11/${x}.o; done
g++ -LBWidgets/build src/ui-x11/*.o -lbwidgetscore -lpugl -lcairoplus -lfontconfig `pkg-config --libs x11 cairo` -o src/ui-x11/test

*/
int main (){
    std::cout << BWIDGETS_DEFAULT_WINDOW_WIDTH << std::endl;
    std::cout << BWIDGETS_DEFAULT_WINDOW_HEIGHT << std::endl;

    BWidgets::Window window(BWIDGETS_DEFAULT_WINDOW_WIDTH,BWIDGETS_DEFAULT_WINDOW_HEIGHT,NULL,0,"Synthesthesia",true);

    OscillatorFrame osc1;
    OscillatorFrame osc2;
    OscillatorFrame osc3;
    EnvelopeFrame env1;
    LfoFrame lfo1;
    FilterFrame flt1;
    
    std::string backgroundFilePath = getProjectPath("assets/background.png");
    window.setBackground(BStyles::Fill(backgroundFilePath));
    window.setFont(BStyles::Font("sans",CAIRO_FONT_SLANT_NORMAL,CAIRO_FONT_WEIGHT_NORMAL,24.0,BStyles::Font::TextAlign::center,BStyles::Font::TextVAlign::middle,1.5));
    
    osc1.configure(0,0);
    osc2.configure(0,UI_MODULE_HEIGHT);
    osc3.configure(0,UI_MODULE_HEIGHT * 2.0);
    env1.configure(0,UI_MODULE_HEIGHT * 3.0);
    lfo1.configure(0,UI_MODULE_HEIGHT * 4.0);
    flt1.configure(0,UI_MODULE_HEIGHT * 5.0);
    window.add(&osc1);
    window.add(&osc2);
    window.add(&osc3);
    window.add(&env1);
    window.add(&lfo1);
    window.add(&flt1);
    
    window.run();
}