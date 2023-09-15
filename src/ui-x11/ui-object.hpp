#ifndef UI_OBJECT_HPP_
#define UI_OBJECT_HPP_


#include "ui-config.hpp"
#include "../module-type.hpp"

#include "../../BWidgets/BEvents/Event.hpp"
#include "../../BWidgets/BWidgets/Window.hpp"
#include "../../BWidgets/BWidgets/Frame.hpp"

#include <string>
#include <cstdint>
#include <vector>

class ModuleFrame : public BWidgets::Frame {
protected:
    int x_index_;
    int y_index_;
    uint32_t instance_;
    std::vector<BWidgets::Widget*> control_widget_;
    std::vector<BWidgets::Widget*> connection_widget_;

    /**
     * @brief loop through widget hierarchy and set layer for each level
     * 
     * @param w widget pointer
     * @param layer layer value
    */
    void setLayerRecursive(BWidgets::Widget* w, int layer);

public:
    /**
     * @brief Virtual Class for UI Module Frames
    */
    ModuleFrame();

    /**
     * @brief Virtual Class for UI Module Frames
     * 
     * @param URID object URID
     * @param title widget title
    */
    ModuleFrame(const uint32_t urid, const std::string& title);

    /**
     * @brief virtual function to configure widget/frame positions
     * 
     * @param x_index frame x position index. Always 0 for full rack, 1 for half rack
     * @param y_index frame y position index. 0 corresponds to the top left corner
    */
    virtual void configure(int x_index, int y_index) = 0;

    /**
     * @brief virtual function to handle port events. Port index is relative to the object
    */
    virtual void port_event(uint32_t port, float value) = 0;


    /**
     * @brief set the layer based on rack position index
     * 
     * @param x_index frame x position index. Always 0 for full rack, 1 for half rack
     * @param y_index frame y position index. 0 corresponds to the top left corner
    */
    void set_position(int x_index, int y_index);

    /**
     * @brief get port_index and float value for specified widget
     * 
     * @param relative_index relative port index
    */
    virtual std::pair<uint32_t,float> get_callback_data(const uint32_t relative_index) = 0;

    /**
     * @brief virtual function to return ModuleType
    */
    virtual const ModuleType get_module_type();

    /**
     * @brief get the instance index for the module
    */
    virtual uint32_t get_instance();

    /**
     * @brief set the instance index for the module
    */
    virtual void set_instance(const uint32_t instance);

    /**
     * @brief gets the control widget array. 
     * 
     * For all derived classes, the widgets in this array should match the order of port_info.hpp, and widgets that don't correspond to 
     * LV2 Control Ports should not be included in the array
    */
    std::vector<BWidgets::Widget*> get_control_widgets();

    /**
     * @brief gets the connection widget array. 
     * 
     * For all derived classes, the widgets in this array should match the order of ports as defined in cfg-connection.hpp. Widgets are 
     * expected to be BWidget::ComboBoxes responsible for detailing a modulator connection.
     * 
    */
    std::vector<BWidgets::Widget*> get_connection_widgets();

};



#endif // UI_OBJECT_HPP_