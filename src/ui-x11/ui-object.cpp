#include "ui-object.hpp"

ModuleFrame::ModuleFrame():
    BWidgets::Frame(URID("/frame"),"Module")
{}

ModuleFrame::ModuleFrame(const uint32_t urid, const std::string& title):
    BWidgets::Frame(urid,title)
{}
