#pragma once

#include <string>
#include <memory>
#include <cassert>
#include <filesystem>


#include "marathon.hpp"
#include "runtime/interactive.hpp"
#include "window/window.hpp"
#include "renderer/renderer.hpp"
#include "events/events.hpp"
#include "time/time.hpp"

namespace marathon {

//// TODO:
// Implement events wrapping/parsing at the highest level before propogating
// Application should not be touched by end user, it is the higher level to the interactive
// Application should set project settings
// Application should handle the window manager
// Application should accept an interactive that controls the main game engine/game loop
// Allow for dynamic opengl config
// All modules should be boot/shutdown from application

struct ApplicationConfig {
    std::string name = "Example Project";
    std::filesystem::path cwd = ".";
    std::filesystem::path editor = "..";
};

class Application {
public:

    static Application* Create(ApplicationConfig cfg) {
        assert(_instance == nullptr && "Attempting to create application twice. Only 1 allowed.");
        // create new app and return ref
        _instance = new Application(cfg);
        return _instance;
    }

    // Accept externally instanced interactive
    void SetInteractive(Interactive* op) {
        if (op == nullptr) {
            std::cout << "ERROR (Application): Attempting to set interactive null." << std::endl;
        }
        _interactive = op;
        _interactive->Start();
    }
    
    // Application loop, independant of game/sim loop
    void Run() {
        while (window::Window::Instance().IsOpen()) {
            // fetch events from backend ready for polling
            events::Events::Instance().Fetch();

            double dt = time::Time::Instance().Tick();

            // interactive tick
            _interactive->Update(dt);
            
            // swap frame shown
            window::Window::Instance().SwapFrame();
        }
    }

    // Shutdown modules and free interactive
    // Shutdown window context
    ~Application() {
        _interactive->End();
        delete _interactive;
        Quit();
    }

private:
    // Construct application using cfg
    // Initialise context and boot modules
    Application(ApplicationConfig cfg)
        : _cfg(cfg) {
        Init();

        // set single app instance
        _instance = this;
    }
    

    ApplicationConfig _cfg;
    Interactive* _interactive = nullptr;
    static inline Application* _instance = nullptr;
};

} // marathon