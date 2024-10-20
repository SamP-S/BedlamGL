#pragma once

// std libs
#include <queue>
#include <vector>
#include <unordered_map>
#include <memory>

// marathon
#include "core/module.hpp"
#include "core/property.hpp"

namespace marathon {

namespace event {

// A signal consists of const data so it cannot be modified.
// Signals should be handled as is and discarded after use.
class Signal {
public:
    Signal(const std::string& name, const std::unordered_map<std::string, Property>& data);
    ~Signal() = default;

    const std::string name;
    const std::unordered_map<std::string, Property> data;
};

class Event : public marathon::Module {
protected:
    Event(const std::string& name);

    // interal event queueing
    virtual void PollInternal() = 0;
    virtual void ClearInternal() = 0;

    std::queue<std::shared_ptr<Signal>> _queue;

public:
    // virtual destructor to prevent event from being instantiated
    virtual ~Event() = default;
    
    // singleton instance accessor
    static Event& Instance();

    // module interface
    virtual void Boot() = 0;
    virtual void Shutdown() = 0;

    // common
    virtual void Fetch() = 0;   // collect all events from backend into our system
    virtual void Clear();   // dump all events from queue, backend should use its own clear method and this
    bool Poll(std::shared_ptr<Signal>& event);   // pass as reference to accept signal assignment
    void Push(std::shared_ptr<Signal> event);   // pass by value to copy event to event queue
    
    // add wait system for reduced CPU usage
};

} // event
    
} //  marathon
