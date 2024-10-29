#include <vector>

#include "marathon.hpp"

namespace marathon {

    time::Time& Time = time::Time::Instance();
    window::Window& Window = window::Window::Instance();
    renderer::Renderer& Renderer = renderer::Renderer::Instance();
    events::Events& Events = events::Events::Instance();


    /// TODO: ensure the boot/shutdown orders are correct
    // ensure each module ensures their required modules have already booted successfully
    // e.g. renderer checks window has booted

    int Init() {
        // maintain specific module order for booting correctly
        std::vector<Module*> modules = {&Time, &Window, &Renderer, &Events};
        int moduleBootFail = 0;
        for (auto module : modules) {
            if (module != nullptr && !module->Boot())
                moduleBootFail++;
        }
        return moduleBootFail;
    }

    int Quit() {
        // maintain specific module order for shutting down correctly
        std::vector<Module*> modules = {&Time, &Window, &Renderer, &Events};
        int moduleShutdownFails = 0;
        for (auto module : modules) {
            if (module != nullptr && !module->Shutdown())
                moduleShutdownFails++;
        }
        return moduleShutdownFails;
    }

} // marathon