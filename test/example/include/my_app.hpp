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
    LA::vec4 color = LA::vec4({1.0f, 1.0f, 1.0f, 1.0f});
    std::shared_ptr<renderer::Mesh> mesh = nullptr;
};

class MyApp : public App {
private:
    MyObject _obj; MyObject _obj2; MyObject _obj3;
    std::shared_ptr<renderer::Shader> _shader;
    std::shared_ptr<renderer::Mesh> _planeMesh;
    std::shared_ptr<renderer::Mesh> _quadMesh;
    std::shared_ptr<renderer::Mesh> _cubeMesh;

    /// TODO: remove this
    time::Time& Time = time::Time::Instance();
    window::Window& Window = window::Window::Instance();
    renderer::Renderer& Renderer = renderer::Renderer::Instance();
    events::Events& Events = events::Events::Instance();

public:
    MyApp() = default;
    ~MyApp() = default;

    void Start() override {
        // create defaults
        _shader = std::make_shared<renderer::Shader>();
        _shader->SetFragmentSource(renderer::defaultFragmentShader);
        _shader->SetVertexSource(renderer::defaultVertexShader);

        // create mesh with vbo & ibo
        _quadMesh = std::make_shared<renderer::QuadMesh>();
        _cubeMesh = std::make_shared<renderer::BoxMesh>();
        _planeMesh = std::make_shared<renderer::PlaneMesh>();

        // create object
        _obj = MyObject();
        _obj.color = {1.0f, 0.2f, 0.2f, 1.0f};
        _obj.mesh = _planeMesh;

        std::string err = "";
        if (!Renderer.ValidateShader(_shader, err)) {
            std::cout << "bedlam/include/runtime.hpp: shader validation failed: \n" << err << std::endl;
        }
        if (!Renderer.ValidateMesh(_planeMesh, err)) {
            std::cout << "bedlam/include/runtime.hpp: plane mesh validation failed: \n" << err << std::endl;
        }
        if (!Renderer.ValidateMesh(_quadMesh, err)) {
            std::cout << "bedlam/include/runtime.hpp: quad mesh validation failed: " << err << std::endl;
        }
        if (!Renderer.ValidateMesh(_cubeMesh, err)) {
            std::cout << "bedlam/include/runtime.hpp: quad mesh validation failed: " << err << std::endl;
        }
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

        int meshIdx = (int)time % 3;
        switch (meshIdx) {
            case 0:
                _obj.mesh = _cubeMesh;
                break;
            case 1:
                _obj.mesh = _quadMesh;
                break;
            case 2:
            default:
                _obj.mesh = _planeMesh;
                break;
        }

        // make draw call of obj at position
        Renderer.Clear();
        Renderer.SetShader(_shader);
        Renderer.SetDepthTest(true);
        
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
