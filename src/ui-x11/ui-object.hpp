#ifndef UI_OBJECT_HPP_
#define UI_OBJECT_HPP_


#include "ui-config.hpp"

#include "../../BWidgets/BWidgets/Window.hpp"
#include "../../BWidgets/BWidgets/Frame.hpp"

#include <string>
#include <cstdint>

class ModuleFrame : public BWidgets::Frame {
private:

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
    */
   virtual void configure(int x, int y) = 0;

   /**
    * @brief virtual function to handle port events. Port index is relative to the object
   */
  virtual void port_event(int port, float value) = 0;

};



#endif // UI_OBJECT_HPP_