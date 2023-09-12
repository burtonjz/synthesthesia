#ifndef UI_WAVEFORM_HPP_
#define UI_WAVEFORM_HPP_

#include "../../BWidgets/BWidgets/Image.hpp"
#include "../waveform.hpp"
#include <string>
#include <cstdint>

class WaveformImage : public BWidgets::Image {
private:

public:
    /**
     * @brief Feature Class to draw waveform images
     *  
     * @param urid image urid
     * @param title image title
    */
    WaveformImage(const uint32_t urid, const std::string& title);

    /**
     * @brief draw waveform
     * 
     * @param w Waveform
    */
    void draw(Waveform waveform);
};

#endif // UI_WAVEFORM_HPP_