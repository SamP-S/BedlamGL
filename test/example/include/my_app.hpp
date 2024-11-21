#pragma once

#include <memory>

#include "runtime/app.hpp"
#include "renderer/renderer.hpp"
#include "events/events.hpp"
#include "time/time.hpp"
#include "window/window.hpp"
#include "la_extended.h"

using namespace marathon;

struct MyObject {
    LA::vec3 position = LA::vec3();
    LA::vec3 rotation = LA::vec3();
    std::shared_ptr<renderer::Mesh> mesh = nullptr;
};


class MyApp : public App {
private:
    MyObject _obj; MyObject _obj2; MyObject _obj3;
    std::shared_ptr<renderer::ColourMaterial> _colourMaterial;
    std::shared_ptr<renderer::PlaneMesh> _planeMesh;
    std::shared_ptr<renderer::QuadMesh> _quadMesh;
    std::shared_ptr<renderer::BoxMesh> _boxMesh;
    std::shared_ptr<renderer::SphereMesh> _sphereMesh;

    /// TODO: remove this
    time::Time& Time = time::Time::Instance();
    window::Window& Window = window::Window::Instance();
    renderer::Renderer& Renderer = renderer::Renderer::Instance();
    events::Events& Events = events::Events::Instance();

public:
    MyApp() = default;
    ~MyApp() = default;

    void Start() override {
        // create material
        _colourMaterial = std::make_shared<renderer::ColourMaterial>();
        // create mesh with vbo & ibo
        _quadMesh = std::make_shared<renderer::QuadMesh>();
        _quadMesh->SetMaterial(_colourMaterial);
        _boxMesh = std::make_shared<renderer::BoxMesh>();
        _boxMesh->SetMaterial(_colourMaterial);
        _planeMesh = std::make_shared<renderer::PlaneMesh>();
        _planeMesh->SetMaterial(_colourMaterial);
        _sphereMesh = std::make_shared<renderer::SphereMesh>();
        _sphereMesh->SetMaterial(_colourMaterial);

        // renderer setup
        Renderer.SetDepthTest(true);
        Renderer.SetCullTest(false);

        // create object
        _obj = MyObject();
        _obj.mesh = _planeMesh;

        std::string err = "";
        if (!Renderer.ValidateShader(_colourMaterial->GetShader(), err)) {
            std::cout << "bedlam/include/runtime.hpp: shader validation failed: \n" << err << std::endl;
        }
        if (!Renderer.ValidateMesh(_planeMesh, err)) {
            std::cout << "bedlam/include/runtime.hpp: plane mesh validation failed: \n" << err << std::endl;
        }
        if (!Renderer.ValidateMesh(_quadMesh, err)) {
            std::cout << "bedlam/include/runtime.hpp: quad mesh validation failed: " << err << std::endl;
        }
        if (!Renderer.ValidateMesh(_boxMesh, err)) {
            std::cout << "bedlam/include/runtime.hpp: quad mesh validation failed: " << err << std::endl;
        }
        if (!Renderer.ValidateMesh(_sphereMesh, err)) {
            std::cout << "bedlam/include/runtime.hpp: sphere mesh validation failed: " << err << std::endl;
        }

        std::cout << "start complete" << std::endl;
    }

    void Update(double deltaTime) override {
        // poll events
        std::shared_ptr<events::Signal> s;
        while (Events.Poll(s)) {
            // std::cout << "bedlam/include/runtime.hpp: event " << s->name << std::endl;
            if (s->name == "quit") {
                Quit();
            }
        }

        double time = Time.GetTime();
        _obj.position.x = 0.25f * sin(time);
        _obj.position.y = 0.2f * cos(time * 2);
        _obj.rotation.y = time * 4.0f;
        _obj.rotation.x = time * 2.0f;
        _colourMaterial->SetColour(
            LA::vec4({0.5f + sin(time), 0.5f + cos(time), 0.5f + sin(time * 2), 1.0f})
        );

        int meshIdx = (int)time % 4;
        switch (meshIdx) {
            case 0:
                _obj.mesh = _boxMesh;
                break;
            case 1:
                _obj.mesh = _quadMesh;
                break;
            case 2:
                _obj.mesh = _sphereMesh;
                break;
            default:
                _obj.mesh = _planeMesh;
                break;
        }

        // make draw call of obj at position
        Renderer.Clear();
        Renderer.PushScale({0.5f, 0.5f, 0.5f}); // scale down

        // draw cube
        Renderer.PushRotate(_obj.rotation);
        Renderer.PushTranslate(_obj.position);
        Renderer.Draw(_obj.mesh);
        Renderer.PopTransform();
        Renderer.PopTransform();

        Renderer.PopTransform();    // scale down
       
    }
};
