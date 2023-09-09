#include "ui-object.hpp"

ModuleFrame::ModuleFrame():
    BWidgets::Frame(URID("/frame"),"Module")
{}

ModuleFrame::ModuleFrame(const uint32_t urid, const std::string& title):
    BWidgets::Frame(urid,title)
{}


void ModuleFrame::set_position(int x_index, int y_index){
    x_index_ = x_index;
    y_index_ = y_index;
    moveTo(x_index*(BWIDGETS_DEFAULT_WINDOW_WIDTH/2),y_index*UI_MODULE_HEIGHT);
    
    setLayerRecursive(this, y_index);
}

void ModuleFrame::setLayerRecursive(BWidgets::Widget* w, int layer){
    // Recurse through children
    w->setLayer(layer);

    for (auto& child : w->getChildren()){
        BWidgets::Widget* wc = dynamic_cast<BWidgets::Widget*>(child);
        setLayerRecursive(wc,layer);
    }
}