#ifndef __URIDS_HPP_
#define __URIDS_HPP_

#include <lv2/lv2plug.in/ns/lv2core/lv2.h>
#include <lv2/lv2plug.in/ns/ext/options/options.h>
#include <lv2/lv2plug.in/ns/ext/urid/urid.h>
#include <lv2/lv2plug.in/ns/ext/atom/atom.h>
#include <lv2/lv2plug.in/ns/ext/midi/midi.h>
#include <lv2/lv2plug.in/ns/ext/buf-size/buf-size.h>

struct Urids {
    LV2_URID midiEvent ;
    LV2_URID bufSize_maxBlockSize ;

    void initialize(LV2_URID_Map* m){
        midiEvent = m->map(m->handle, LV2_MIDI__MidiEvent);
        bufSize_maxBlockSize = m->map(m->handle, LV2_BUF_SIZE__maxBlockLength);
    }
};

#endif // __URIDS_HPP_