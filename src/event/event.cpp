#include "event/event.hpp"
#include "event/sdl2/event.hpp"

namespace marathon {

namespace event {

Signal::Signal(const std::string& name, const std::unordered_map<std::string, Property>& data)
    : name(name), data(data) {}

// Constructor
Event::Event(const std::string& name)
    : marathon::Module(marathon::ModuleType::EVENT, name) {}

// Singleton instance accessor
Event& Event::Instance() {
    static Event* instance;
    if (!instance)
        instance = new marathon::event::sdl2::Event();
    return *instance;
}

// Common iterative access
int Event::Poll(std::shared_ptr<Signal> event) {
    if (_queue.empty())
        return 0;
    event = _queue.front();
    _queue.pop();
    return _queue.size();
}
    
void Event::Push(std::shared_ptr<Signal> event) {
    _queue.push(event);
}
void Event::Clear() {
    while (!_queue.empty()) {
        _queue.pop();
    }
}

} // event
    
} // marathon