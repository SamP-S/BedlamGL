#include "SDL2/SDL_events.h"
#include "SDL2/SDL.h"

#include "events/sdl2/events.hpp"



namespace marathon {

namespace event {

namespace sdl2 {


Events::Events()
    : marathon::event::Events("marathon.event.sdl2") {}

Events::~Events() {}

void Events::Boot() {
    if (SDL_Init(SDL_INIT_EVENTS) != 0) {
        std::cerr << "events/sdl2/events.cpp: SDL_Init Error = " << SDL_GetError() << std::endl;
    }
}
void Events::Shutdown() {
    SDL_QuitSubSystem(SDL_INIT_EVENTS);
}

void Events::Fetch() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        std::shared_ptr<Signal> s = Convert(e);
        if (s != nullptr) {
            Events::Push(s);
        }
    }
}
void Events::Clear() {
    SDL_Event e;
	while (SDL_PollEvent(&e)) {}
    Events::Clear();
}

/// TODO:
// add parsing for more event types controller/touch/drop/audio
// more rigorously parse events rather than current dummy/generic stuff


std::shared_ptr<Signal> Events::Convert(const SDL_Event& e) {
    switch (e.type) {
        case SDL_QUIT:
            return std::make_shared<Signal>("quit", std::unordered_map<std::string, Property>());
        case SDL_WINDOWEVENT:
            return ConvertWindowEvent(e);
        case SDL_MOUSEMOTION:
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
        case SDL_MOUSEWHEEL:
            return ConvertMouseEvent(e);
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            return ConvertKeyboardEvent(e);
        default:
            return nullptr;
    }
}

std::shared_ptr<Signal> Events::ConvertWindowEvent(const SDL_Event& e) {
    std::unordered_map<std::string, Property> data;
    switch (e.window.event) {
        case SDL_WINDOWEVENT_CLOSE:
            return std::make_shared<Signal>("window.close", data);
        case SDL_WINDOWEVENT_RESIZED:
            data["width"] = Property(e.window.data1);
            data["height"] = Property(e.window.data2);
            return std::make_shared<Signal>("window.resize", data);
        case SDL_WINDOWEVENT_MOVED:
            data["x"] = Property(e.window.data1);
            data["y"] = Property(e.window.data2);
            return std::make_shared<Signal>("window.move", data);
        case SDL_WINDOWEVENT_MINIMIZED:
            return std::make_shared<Signal>("window.minimize", data);
        case SDL_WINDOWEVENT_MAXIMIZED:
            return std::make_shared<Signal>("window.maximize", data);
        case SDL_WINDOWEVENT_RESTORED:
            return std::make_shared<Signal>("window.restore", data);
        case SDL_WINDOWEVENT_ENTER:
            return std::make_shared<Signal>("window.enter", data);
        case SDL_WINDOWEVENT_LEAVE:
            return std::make_shared<Signal>("window.leave", data);
        case SDL_WINDOWEVENT_FOCUS_GAINED:
            return std::make_shared<Signal>("window.focus.gain", data);
        case SDL_WINDOWEVENT_FOCUS_LOST:
            return std::make_shared<Signal>("window.focus.lost", data);
        default:
            return nullptr;
    }
}
std::shared_ptr<Signal> Events::ConvertMouseEvent(const SDL_Event& e) {
    std::unordered_map<std::string, Property> data;
    switch (e.type) {
        case SDL_MOUSEMOTION:
            data["x"] = Property(e.motion.x);
            data["y"] = Property(e.motion.y);
            data["dx"] = Property(e.motion.xrel);
            data["dy"] = Property(e.motion.yrel);
            return std::make_shared<Signal>("mouse.move", data);
        case SDL_MOUSEBUTTONDOWN:
            data["x"] = Property(e.button.x);
            data["y"] = Property(e.button.y);
            data["button"] = Property(e.button.button);
            return std::make_shared<Signal>("mouse.down", data);
        case SDL_MOUSEBUTTONUP:
            data["x"] = Property(e.button.x);
            data["y"] = Property(e.button.y);
            data["button"] = Property(e.button.button);
            return std::make_shared<Signal>("mouse.up", data);
        case SDL_MOUSEWHEEL:
            data["x"] = Property(e.wheel.x);
            data["y"] = Property(e.wheel.y);
            return std::make_shared<Signal>("mouse.wheel", data);
        default:
            return nullptr;
    }
}
std::shared_ptr<Signal> Events::ConvertKeyboardEvent(const SDL_Event& e) {
    std::unordered_map<std::string, Property> data;
    switch (e.type) {
        case SDL_KEYDOWN:
            data["key"] = Property(e.key.keysym.sym);
            data["mod"] = Property(e.key.keysym.mod);
            return std::make_shared<Signal>("key.down", data);
        case SDL_KEYUP:
            data["key"] = Property(e.key.keysym.sym);
            data["mod"] = Property(e.key.keysym.mod);
            return std::make_shared<Signal>("key.up", data);
        default:
            return nullptr;
    }
}


} // sdl2

} // event

} // marathon