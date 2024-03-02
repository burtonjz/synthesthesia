#ifndef __PARENT_MODULE_HPP_
#define __PARENT_MODULE_HPP_

#include "commonTypes.hpp"

#include "Module.hpp"
#include "ModuleType.hpp"
#include "ParameterType.hpp"
#include "KeyboardController.hpp"

#include "BMap.hpp"
#include <set>

/**
 * @brief a module that spawns a child module for each midi note
 *
*/
template <ModuleType child>
class MidiModule : public Module {
private:
    ModuleType childType_ ;
    BMap<uint8_t, child, 128> children_ ;
    KeyboardController* keyboardController_ ; 

public:
    MidiModule(ModuleType modType):
        Module(modType),
        childModuleType_(child)
    {}

    MidiModule(ModuleType typ, const double* sampleRate, int instance):
        Module(sampleRate,typ,instance),
        childModuleType_(child)
    {}

    const ModuleType getChildType() const {
        return childType_ ;
    }

    std::set<ParameterType> getChildModulatableParameters() const {
        // TODO
    }

    /**
     * @brief virtual function to define how to create children
     * 
     * Any derived class must specify how it creates the child modules
     * from the midi data
    */
    virtual void createChild(uint8_t midi_note, const Note note) = 0 ;

    /**
     * @brief virtual function to define behavior on key repress
     * 
     * When a key is repressed, you may need to specify how children are updated.
     * Most commonly, this would be updating an amplitude parameter with the new
     * key velocity.
    */
    virtual void repressChild(uint8_)t midi_note, const Note note) = 0 ;

    virtual void tick(){  
        updateChildren();
        parameterController_.modulate();
    }

private:
    /**
     * @brief update all child oscillators
     * 
     * This function handles the logic for updating child oscillators
     * during a module tick(). Creates, deletes, and updates child modules.
    */
    void updateChildren(){
        if (!keyboardController_) return ;
        const KeyboardMap* notes_ptr = keyboardController_->get_active_notes();
        
        // For each active note, we need to create the child module, or process the repress behavior
        for (const auto& pair : *notes_ptr ){
            auto it = children_.find(pair.first);
            if ( it == children_.end() ){
                createChild(pair.first, pair.second);
            } else {
                repressChild(pair.first, pair.second);
            }
        }

        // now, loop through all children. If they aren't an active note
        // delete the child. Otherwise, tick the child.
        for (auto it = children_.begin(); it != children_.end(); ){
            if ( (*notes_ptr).find(it->first) == (*notes_ptr).end()){
                it = children_.erase(it);
            } else {
                children_[it->first].tick();
                ++it ;
            }
        }
    }
};

#endif // __PARENT_MODULE_HPP_