#include "../../BWidgets/BWidgets/Window.hpp"
#include "ui-config.hpp"
#include "ui-oscillator.hpp"
#include "ui-filter.hpp"
#include "ui-envelope.hpp"
#include "ui-lfo.hpp"


/*
BUILDING TEST:

for x in test ui-envelope ui-filter ui-lfo ui-object ui-oscillator; do g++ -fPIC -DPIC `pkg-config --cflags x11 cairo` -IBWidgets/include src/ui-x11/${x}.cpp -c -o src/ui-x11/${x}.o; done
g++ -LBWidgets/build src/ui-x11/*.o -lbwidgetscore -lpugl -lcairoplus -lfontconfig `pkg-config --libs x11 cairo` -o src/ui-x11/test

*/
int main (){
    BWidgets::Window window(UI_WINDOW_WIDTH, UI_WINDOW_HEIGHT, NULL, 0,"Synthesthesia",true);

    UIOscObject osc1(0,0,0);
    UIOscObject osc2(0,UI_MODULE_HEIGHT,1);
    UIOscObject osc3(0,UI_MODULE_HEIGHT*2.0,2);
    UIEnvObject env1(0,UI_MODULE_HEIGHT*3.0,0);
    UILfoObject lfo1(0,UI_MODULE_HEIGHT*4.0,0);
    UIFilterObject flt1(0,UI_MODULE_HEIGHT*5.0,0);

    window.setBackground(BStyles::Fill("/home/burtonjz/synthesthesia/assets/background.png"));
    window.setFont(BStyles::Font("sans",CAIRO_FONT_SLANT_NORMAL,CAIRO_FONT_WEIGHT_NORMAL,24.0,BStyles::Font::TextAlign::center,BStyles::Font::TextVAlign::middle,1.5));
    
    for(auto& element : osc1.getWidgetArray()) window.add(element);
    for(auto& element : osc2.getWidgetArray()) window.add(element);
    for(auto& element : osc3.getWidgetArray()) window.add(element);
    for(auto& element : env1.getWidgetArray()) window.add(element);
    for(auto& element : lfo1.getWidgetArray()) window.add(element);
    for(auto& element : flt1.getWidgetArray()) window.add(element);
    
    window.run();
}