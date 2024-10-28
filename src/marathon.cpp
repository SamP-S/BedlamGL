#include "marathon.hpp"

namespace marathon {

    bool Init() {
        int moduleBootFail = 0;
        time::Time::Instance().Boot();
        window::Window::Instance().Boot();
        renderer::Renderer::Instance().Boot();
        events::Events::Instance().Boot();
        return true;
    }

    bool Quit() {
        Renderer.Shutdown();
        Events.Shutdown();
        Window.Shutdown();
        Time.Shutdown();
        return true;
    }

} // marathon