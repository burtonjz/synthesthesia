#include "ui-object.hpp"

UIObject::UIObject(double x, double y, int index):
    x_(x),
    y_(y),
    index_(index)
{}

std::array<double,4> UIObject::getObjectDimensions() const {
    return { x_, y_, getObjectWidth(), getObjectHeight()  };
}
