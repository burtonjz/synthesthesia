#include "ui-waveform.hpp"
#include "cairo/cairo.h"
#include <random>
#include <ctime>

WaveformImage::WaveformImage(const uint32_t urid, const std::string& title):
    BWidgets::Image(urid,title)
{}

void WaveformImage::draw(Waveform waveform){
    cairo_surface_t* surface = getImageSurface(BStyles::Status::normal);
    cairoplus_surface_clear(surface);
    cairo_t* cr = cairo_create(surface);

    const double width = cairo_image_surface_get_width(surface);
    const double height = cairo_image_surface_get_height(surface);
    
    // TODO: MAKE VARIABLES IN CONFIG
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    cairo_set_line_width(cr, 2.0);

    switch(waveform){
    case WAVE_SINE:
        for (double x = 0.0; x < 1.0; x += 1.0 / 32.0){
            cairo_line_to(cr, x * width, 0.5 * height - 0.5 * height * std::sin(2.0 * M_PI * x));
        }
        break;
    case WAVE_TRIANGLE:
        cairo_move_to(cr, 0.0, 0.5 * height);
        cairo_line_to(cr, 0.25 * width, 0.0);
        cairo_line_to(cr, 0.75 * width, height);
        cairo_line_to(cr, width, 0.5 * height);
        break;
    case WAVE_SQUARE:
        cairo_move_to(cr, 0.0, 0.5 * height);
        cairo_line_to(cr, 0.0, 0.0);
        cairo_line_to(cr, 0.5 * width, 0.0);
        cairo_line_to(cr, 0.5 * width, height);
        cairo_line_to(cr, width, height);
        cairo_line_to(cr, width, 0.5 * height);
        break;
    case WAVE_SAW:
        cairo_move_to(cr, 0.0, 0.5 * height);
        cairo_line_to(cr, 0.0, height);
        cairo_line_to(cr, width, 0.0);
        cairo_line_to(cr, width, 0.5 * height);
        break;
    case WAVE_NOISE:
        {
            std::minstd_rand rnd;
            std::uniform_real_distribution<float> dist;
            cairo_move_to(cr, 0.0, 0.5 * height);
            for (double x = 0.0; x < 1.0; x += 1.0 / 160){
                cairo_line_to(cr, x * width, 0.5 * height - 0.5 * height * dist(rnd));
            }
            cairo_line_to(cr, 10., 0.5 * height);
        }
        break;
    case WAVE_EXPERIMENT:
        break;
    default:
        break;
    }

    cairo_stroke(cr);
    cairo_destroy(cr);
    update();
}

