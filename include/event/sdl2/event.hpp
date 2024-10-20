#pragma once

#include "SDL2/SDL_events.h"

#include "event/event.hpp"


namespace marathon {

namespace event {

namespace sdl2 {

class Event : public marathon::event::Event {
public:
    Event();
    ~Event() override;

    void Boot() override;
    void Shutdown() override;

    void Fetch() override;
    void Clear();

private:
    std::shared_ptr<Signal> Convert(const SDL_Event& e);
    std::shared_ptr<Signal> ConvertWindowEvent(const SDL_Event& e);
    std::shared_ptr<Signal> ConvertMouseEvent(const SDL_Event& e);
    std::shared_ptr<Signal> ConvertKeyboardEvent(const SDL_Event& e);
};

} // sdl2

} // event

} // marathon