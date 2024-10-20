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
    virtual int PollInternal(void* event) = 0;
    virtual int ClearInternal() = 0;

    std::queue<std::shared_ptr<Signal>> _queue;

public:
    // virtual destructor to prevent event from being instantiated
    virtual ~Event() = default;
    
    // singleton instance accessor
    static Event& Instance();

    // module interface
    virtual void Boot() = 0;
    virtual void Shutdown() = 0;

    // common iterative access
    int Poll(std::shared_ptr<Signal> event);
    void Push(std::shared_ptr<Signal> event);
    void Clear();
    // add wait system for reduced CPU usage
};

} // event
    
} //  marathon
