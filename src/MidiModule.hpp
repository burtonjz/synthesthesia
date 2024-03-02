#ifndef __PARENT_MODULE_HPP_
#define __PARENT_MODULE_HPP_

#include "commonTypes.hpp"

#include "Module.hpp"
#include "ModuleType.hpp"
#include "ParameterType.hpp"
#include "KeyboardController.hpp"

#include "Oscillator.hpp"

#include "BMap.hpp"
#include <set>


template <ModuleType T>
struct ModContainer ;

// START supported child specializations
template <>
struct ModContainer <ModuleType::Oscillator> {
    using containerType = BMap<uint8_t, Oscillator, 128>;
};
// END supported child specializations

/**
 * @brief a module that spawns a child module for each midi note
 *
*/
template <ModuleType child>
class MidiModule : public Module {
public:
    using ChildContainer = typename ModContainer<child>::containerType ;

private:
    // member variables
    ModuleType childType_ ;
    KeyboardController* keyboardController_ ; 

protected:
    ChildContainer children_ ;

public:
    MidiModule(ModuleType modType):
        Module(modType),
        childType_(child),
        children_(),
        keyboardController_(nullptr)
    {}

    MidiModule(ModuleType typ, const double* sampleRate, int instance):
        Module(sampleRate,typ,instance),
        childType_(child),
        children_(),
        keyboardController_(nullptr)
    {}

    virtual void activate(KeyboardController* keyboardController){
        keyboardController_ = keyboardController ;
    }

    const ModuleType getChildType() const {
        return childType_ ;
    }

    std::set<ParameterType> getChildModulatableParameters() const {
        // TODO
        return {ParameterType::AMPLITUDE};
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
    virtual void repressChild(uint8_t midi_note, const Note note) = 0 ;

    virtual void tick(){  
        updateChildren();
        parameterController_.modulate();
        // TODO: make sure that the modulationController is updating the parent modulations (pan) when those values are changing
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