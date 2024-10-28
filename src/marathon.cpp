#include "marathon.hpp"

namespace marathon {

    int Init() {
        int moduleBootFail = 0;
        if (!time::Time::Instance().Boot())
            moduleBootFail++;
        if (!window::Window::Instance().Boot())
            moduleBootFail++;
        if (!renderer::Renderer::Instance().Boot())
            moduleBootFail++;
        if (!events::Events::Instance().Boot())
            moduleBootFail++;
        return moduleBootFail;
    }

    int Quit() {
        int moduleShutdownFails = 0;
        if (!time::Time::Instance().Shutdown())
            moduleShutdownFails++;
        if (!window::Window::Instance().Shutdown())
            moduleShutdownFails++;
        if (!renderer::Renderer::Instance().Shutdown())
            moduleShutdownFails++;
        if (!events::Events::Instance().Shutdown())
            moduleShutdownFails++;
        return moduleShutdownFails;
    }

} // marathon