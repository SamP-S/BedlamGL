#include "marathon.hpp"

using namespace marathon;

time::Time& Time = time::Time::Instance();
window::Window& Window = window::Window::Instance();
renderer::Renderer& Renderer = renderer::Renderer::Instance();
events::Events& Events = events::Events::Instance();

void update(double dt) {

}

int main() {
    // initialise
    if (!marathon::Init()) {
        std::cout << "Failed to initialise marathon. :(" << std::endl;
        return 1;
    }

    // main loop
    while (Window.IsOpen()) {
        // fetch events from backend ready for polling
        Events.Fetch();

        double dt = Time.Tick();

        // interactive tick
        update(dt);
        
        // swap frame shown
        Window.SwapFrame();
    }


    // safely shutdown
    if (!marathon::Quit()) {
        std::cout << "Failed to quit marathon. :(" << std::endl;
        return 1;
    }
    return 0;
}