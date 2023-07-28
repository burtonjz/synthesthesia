#ifndef UI_ENV_WIDGETS_HPP_
#define UI_ENV_WIDGETS_HPP_

#include <array>
#include "ui-object.hpp"
#include "../../BWidgets/BWidgets/ValueHSlider.hpp"

class UIEnvObject : public UIObject {
private:
    BWidgets::ValueHSlider slider_attack;
    BWidgets::ValueHSlider slider_decay;
    BWidgets::ValueHSlider slider_sustain;
    BWidgets::ValueHSlider slider_release;
    std::array<BWidgets::Widget*,4> widget;
public:
    /**
     * @brief UIEnvObject constructor
     * 
     * @param x The x-coordinate corresponding with the top-left corner of the object
     * @param y The y-coordinate corresponding with the top-left corner of the object
     * @param index The URID to be used for the first BWidget within the object
    */
    UIEnvObject(double x, double y, int index);

    int getNumWidgets() const override;
    double getObjectHeight() const override;
    double getObjectWidth() const override;
    std::array<BWidgets::Widget*,4> getWidgetArray() const;
};

#endif // UI_ENV_WIDGETS_HPP_