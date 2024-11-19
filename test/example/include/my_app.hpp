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
    std::shared_ptr<renderer::Mesh> _triangleMesh;
    std::shared_ptr<renderer::Mesh> _quadMesh;
    std::shared_ptr<renderer::Mesh> _cubeMesh;

    /// TODO: remove this
    time::Time& Time = time::Time::Instance();
    window::Window& Window = window::Window::Instance();
    renderer::Renderer& Renderer = renderer::Renderer::Instance();
    events::Events& Events = events::Events::Instance();

public:
    virtual void Start() override;
    virtual void Update(double deltaTime) override;
};
