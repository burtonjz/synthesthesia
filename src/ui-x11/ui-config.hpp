#ifndef UI_CONFIG_HPP_
#define UI_CONFIG_HPP_

// URIs
#define SYNTHESTHESIA_GUI_URI "https://github.com/burtonjz/synthesthesia#gui"

#define URID(x) (BURID(SYNTHESTHESIA_GUI_URI x)) /*> macro to convert text label to URID */

// OVERRIDE WIDGET DEFAULTS
#define BWIDGETS_DEFAULT_WINDOW_WIDTH 2250
#define BWIDGETS_DEFAULT_WINDOW_HEIGHT 1800

#define BWIDGETS_DEFAULT_FRAME_WIDTH 2250
#define BWIDGETS_DEFAULT_FRAME_HEIGHT 150

#define BWIDGETS_DEFAULT_HSWITCH_WIDTH 80
#define BWIDGETS_DEFAULT_HSWITCH_HEIGHT 40

#define BWIDGETS_DEFAULT_VALUEDIAL_WIDTH 90
#define BWIDGETS_DEFAULT_VALUEDIAL_HEIGHT 90

#define BWIDGETS_DEFAULT_COMBOBOX_WIDTH 160
#define BWIDGETS_DEFAULT_COMBOBOX_HEIGHT 40

#define BWIDGETS_DEFAULT_VALUEHSLIDER_WIDTH 160
#define BWIDGETS_DEFAULT_VALUEHSLIDER_HEIGHT 60


// WIDGET PARAMETERS

constexpr int UI_MODULE_HEIGHT = 150; // all modules will have the same height

// OSCILLATOR
constexpr int UI_OSC_WIDTH = BWIDGETS_DEFAULT_WINDOW_WIDTH; // full "rack", divide by two for "half rack"

constexpr int UI_OSC_SWITCH_ON_X = 80;
constexpr int UI_OSC_SWITCH_ON_Y = 45;

constexpr int UI_OSC_DIAL_GAIN_X = 220;
constexpr int UI_OSC_DIAL_GAIN_Y = 23;

constexpr int UI_OSC_DIAL_PAN_X = 350;
constexpr int UI_OSC_DIAL_PAN_Y = 23;

constexpr int UI_OSC_BOX_WAVEFORM_X = 488;
constexpr int UI_OSC_BOX_WAVEFORM_Y = 45;

constexpr int UI_OSC_DIAL_DETUNE_SEMI_X = 1155;
constexpr int UI_OSC_DIAL_DETUNE_SEMI_Y = 23;

constexpr int UI_OSC_DIAL_DETUNE_CENTS_X = 1328;
constexpr int UI_OSC_DIAL_DETUNE_CENTS_Y = 23;

constexpr int UI_OSC_BOX_MOD_FREQ_X = 1605;
constexpr int UI_OSC_BOX_MOD_FREQ_Y = 45;

constexpr int UI_OSC_BOX_MOD_AMP_X = 1815;
constexpr int UI_OSC_BOX_MOD_AMP_Y = 45;

constexpr int UI_OSC_BOX_MOD_PHASE_X = 2010;
constexpr int UI_OSC_BOX_MOD_PHASE_Y = 45;

// LFO

constexpr int UI_LFO_WIDTH = BWIDGETS_DEFAULT_WINDOW_WIDTH / 2.0; // half rack

constexpr int UI_LFO_SLIDER_FREQ_X = 100;
constexpr int UI_LFO_SLIDER_FREQ_Y = 45;

constexpr int UI_LFO_SLIDER_DEPTH_X = 270;
constexpr int UI_LFO_SLIDER_DEPTH_Y = 45;

constexpr int UI_LFO_BOX_WAVEFORM_X = 425;
constexpr int UI_LFO_BOX_WAVEFORM_Y = 55;

// ENVELOPE

constexpr int UI_ENV_WIDTH = BWIDGETS_DEFAULT_WINDOW_WIDTH / 2.0; // half rack

constexpr int UI_ENV_SLIDER_ATTACK_X = 95;
constexpr int UI_ENV_SLIDER_ATTACK_Y = 45;

constexpr int UI_ENV_SLIDER_DECAY_X = 265;
constexpr int UI_ENV_SLIDER_DECAY_Y = 45;

constexpr int UI_ENV_SLIDER_SUSTAIN_X = 415;
constexpr int UI_ENV_SLIDER_SUSTAIN_Y = 45;

constexpr int UI_ENV_SLIDER_RELEASE_X = 600;
constexpr int UI_ENV_SLIDER_RELEASE_Y = 45;

// FILTER

constexpr int UI_FLT_WIDTH = BWIDGETS_DEFAULT_WINDOW_WIDTH / 2.0; // half rack

constexpr int UI_FLT_BOX_TYPE_X = 100;
constexpr int UI_FLT_BOX_TYPE_Y = 45;

constexpr int UI_FLT_SLIDER_CUTOFF_X = 270;
constexpr int UI_FLT_SLIDER_CUTOFF_Y = 30;

constexpr int UI_FLT_DIAL_RES_X = 460;
constexpr int UI_FLT_DIAL_RES_Y = 23;

constexpr int UI_FLT_BOX_MOD_CUTOFF_X = 660;
constexpr int UI_FLT_BOX_MOD_CUTOFF_Y = 45;

constexpr int UI_FLT_BOX_MOD_RES_X = 870;
constexpr int UI_FLT_BOX_MOD_RES_Y = 45;

#endif // UI_CONFIG_HPP_