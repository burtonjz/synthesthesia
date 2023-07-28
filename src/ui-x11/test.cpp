#include "../../BWidgets/BWidgets/Window.hpp"
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
    BWidgets::Window window(750, 600, 0);
    UIOscObject osc1(0,0,0);
    UIOscObject osc2(0,50,1);
    UIOscObject osc3(0,100,2);
    UIEnvObject env1(0,150,0);
    UILfoObject lfo1(0,200,0);
    UIFilterObject flt1(0,250,0);

    for(auto& element : osc1.getWidgetArray()) window.add(element);
    for(auto& element : osc2.getWidgetArray()) window.add(element);
    for(auto& element : osc3.getWidgetArray()) window.add(element);
    for(auto& element : env1.getWidgetArray()) window.add(element);
    for(auto& element : lfo1.getWidgetArray()) window.add(element);
    for(auto& element : flt1.getWidgetArray()) window.add(element);
    window.setBackground(BStyles::Fill("/home/burtonjz/synthesthesia/assets/background.png"));
    window.run();
}