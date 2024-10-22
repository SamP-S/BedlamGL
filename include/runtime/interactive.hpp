#pragma once

#include "renderer/renderer.hpp"
#include "window/window.hpp"
#include "events/events.hpp"
#include "time/time.hpp"

namespace marathon {

//// TODO:
// move module instances here

class Interactive {
public:
    renderer::Renderer& Renderer = renderer::Renderer::Instance();
    window::Window& Window = window::Window::Instance();
    event::Events& Events = event::Events::Instance();
    time::Time& Time = time::Time::Instance();

    virtual void Start() = 0;
    virtual void Update(double dt) = 0;
    virtual void End() = 0;
};

} // marathon
