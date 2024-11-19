#include "runtime/app.hpp"

#include "window/window.hpp"
#include "events/events.hpp"
#include "time/time.hpp"

namespace marathon {

App::App() {}
App::~App() {}

// boot systems and modules
void App::Boot() {
    std::cout << "App::Boot()" << std::endl;
}

// shutdown systems and modules
void App::Shutdown() {
    std::cout << "App::Shutdown()" << std::endl;
}

void App::Run() {
    // boot all systems and modules
    Boot();

    // main game loop
    while (!_mQuit) {
        // fetch events from backend ready for polling
        events::Events::Instance().Fetch();
        
        // interactive tick
        double dt = time::Time::Instance().Tick();
        Update(dt);
        
        // swap frame shown
        window::Window::Instance().SwapFrame();
    }

    // shutdown safely
    Shutdown();
}

void App::Quit() {
    _mQuit = true;
}


} // namespace marathon