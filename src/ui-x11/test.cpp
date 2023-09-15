#include "ui-config.hpp"
#include "get-project-path.hpp"
#include "ui-oscillator.hpp"
#include "ui-filter.hpp"
#include "ui-envelope.hpp"
#include "ui-lfo.hpp"

#include "../../BWidgets/BWidgets/Window.hpp"
#include "../../BWidgets/BStyles/Types/Font.hpp"

#include <iostream>

/*
BUILDING TEST:

for x in synthesthesia-ui ui-port-handler ui-object ui-envelope ui-filter ui-lfo ui-oscillator ui-waveform; do g++ -fPIC -DPIC `pkg-config --cflags lv2 x11 cairo` -IBWidgets/include src/ui-x11/${x}.cpp -c -o src/ui-x11/${x}.o; done
g++ -LBWidgets/build src/ui-x11/*.o -lbwidgetscore -lpugl -lcairoplus -lfontconfig `pkg-config --libs x11 cairo` -o src/ui-x11/test

*/

void widget_recurse(BWidgets::Widget* w, int depth){
    // indentation
    for (int i = 0; i < depth; ++i) {
        std::cout << "  "; // Add indentation for nested widgets
    }

    std::cout << "{\"URID\": \"" << w->getUrid() << 
        "\", \"URI\": \"" << BUtilities::Urid::uri(w->getUrid()) <<  
        "\", \"Title\": \"" << w->getTitle() <<  
        "\", \"Visible\": \"" << static_cast<int>(w->isVisible()) << 
        "\", \"Layer\": \"" << w->getLayer() << 
        "\", \"Width\": \"" << w->getWidth() << 
        "\", \"Height\": \"" << w->getHeight() << 
        "\", \"Font\": { \"Family\": \"" << w->getFont().family << "\", \"Size\": \"" << w->getFont().size << "\"}" 
        << "} " << std::endl;

    for(auto& child: w->getChildren()){
        BWidgets::Widget* wc = dynamic_cast<BWidgets::Widget*>(child);
        widget_recurse(wc,depth+1);
    }
}

int main (){

    OscillatorFrame osc1(URID("/frame"),"oscillator-1");
    OscillatorFrame osc2(URID("/frame"),"oscillator-2");
    OscillatorFrame osc3(URID("/frame"),"oscillator-3");
    EnvelopeFrame env1(URID("/frame"),"envelope-1");
    LfoFrame lfo1(URID("/frame"),"lfo-1");
    FilterFrame flt1(URID("/frame"),"filter-1");
    
    std::string backgroundFilePath = getProjectPath("assets/background.png");
    
    // configure and add child widgets
    BWidgets::Window window(BWIDGETS_DEFAULT_WINDOW_WIDTH,BWIDGETS_DEFAULT_WINDOW_HEIGHT,NULL,URID("/window"),"Synthesthesia",true);

    osc1.configure(0,0);
    osc2.configure(0,1);
    osc3.configure(0,2);
    env1.configure(0,3);
    lfo1.configure(0,4);
    flt1.configure(0,5);


    window.add(&osc1);
    window.add(&osc2);
    window.add(&osc3);
    window.add(&env1);
    window.add(&lfo1);
    window.add(&flt1);
    
    // style and theming
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
				{BURID(BSTYLES_STYLEPROPERTY_BACKGROUND_URI), BUtilities::makeAny<BStyles::Fill>(BStyles::Fill(backgroundFilePath))},
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
     
    window.setTheme(theme);

    widget_recurse(dynamic_cast<BWidgets::Widget*>(&osc2),0);

    window.run();
}




