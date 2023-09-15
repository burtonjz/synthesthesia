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

const ModuleType ModuleFrame::get_module_type(){
    return ModuleType::MODULE_NULL;
} 

uint32_t ModuleFrame::get_instance(){
    return instance_;
}

void ModuleFrame::set_instance(const uint32_t instance){
    instance_ = instance;
}

std::vector<BWidgets::Widget*> ModuleFrame::get_control_widgets(){
    return control_widget_;
}

std::vector<BWidgets::Widget*> ModuleFrame::get_connection_widgets(){
    return connection_widget_;
}
