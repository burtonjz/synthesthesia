#ifndef UI_OBJECT_HPP_
#define UI_OBJECT_HPP_


#include "ui-config.hpp"

#include "../../BWidgets/BWidgets/Window.hpp"
#include "../../BWidgets/BWidgets/Frame.hpp"

#include <string>
#include <cstdint>

class ModuleFrame : public BWidgets::Frame {
protected:
    int x_index_;
    int y_index_;

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
  virtual void port_event(int port, float value) = 0;

  /**
   * @brief set the layer based on rack position index
   * 
   * @param x_index frame x position index. Always 0 for full rack, 1 for half rack
   * @param y_index frame y position index. 0 corresponds to the top left corner
  */
  void set_position(int x_index, int y_index);




};



#endif // UI_OBJECT_HPP_