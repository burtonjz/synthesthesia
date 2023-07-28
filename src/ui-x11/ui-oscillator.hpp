#ifndef UI_OSC_WIDGETS_HPP_
#define UI_OSC_WIDGETS_HPP_

#include <array>
#include "ui-object.hpp"
#include "../../BWidgets/BWidgets/ValueHSlider.hpp"
#include "../../BWidgets/BWidgets/ComboBox.hpp"
#include "../../BWidgets/BWidgets/ValueDial.hpp"

class UIOscObject : public UIObject {
private:
    BWidgets::Button button_on;
    BWidgets::ValueDial dial_gain;
    BWidgets::ComboBox cb_waveform;
    BWidgets::ValueDial dial_detune_semi;
    BWidgets::ValueDial dial_detune_cents;
    BWidgets::ValueDial dial_pan;
    BWidgets::ComboBox cb_freq_mod;
    BWidgets::ComboBox cb_amp_mod;
    BWidgets::ComboBox cb_phase_mod;
    std::array<BWidgets::Widget*,9> widget;
public:
    /**
     * @brief UIOscObject constructor
     * 
     * @param x The x-coordinate corresponding with the top-left corner of the object
     * @param y The y-coordinate corresponding with the top-left corner of the object
     * @param index The URID to be used for the first BWidget within the object
    */
    UIOscObject(double x, double y, int index);

    int getNumWidgets() const override;
    double getObjectHeight() const override;
    double getObjectWidth() const override;
    std::array<BWidgets::Widget*,9> getWidgetArray() const;
};

#endif // UI_OSC_WIDGETS_HPP_