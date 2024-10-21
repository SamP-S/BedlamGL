#pragma once

#include <iostream>

#include "marathon.hpp"
#include "renderer/shader_source.hpp"
#include "renderer/shader.hpp"
#include "event/event.hpp"


//// TODO:
// consolidate naming such that Tick is every frame/on timer

namespace marathon {

struct MyObject {
    LA::vec3 position = LA::vec3();
    LA::vec4 color = LA::vec4({1.0f, 1.0f, 1.0f, 1.0f});
};

class Runtime : public Interactive {
private:
    MyObject _obj;
    double _time = 0.0;
    std::shared_ptr<renderer::ShaderSource> _vs;
    std::shared_ptr<renderer::ShaderSource> _fs;
    std::shared_ptr<renderer::Shader> _shader;
    std::shared_ptr<renderer::Mesh> _mesh;

public:
    renderer::Renderer& Renderer = renderer::Renderer::Instance();
    window::Window& Window = window::Window::Instance();
    event::Events& Events = event::Events::Instance();

    Runtime() {}
    ~Runtime() {}

    void Start() override {
        // create object
        _obj = MyObject();
        _obj.color = {1.0f, 0.2f, 0.2f, 1.0f};

        // load shaders
        _vs = renderer::ShaderSource::Create("default_vs", renderer::defaultVertexShader, renderer::ShaderStage::VERTEX);
        _fs = renderer::ShaderSource::Create("default_fs", renderer::defaultFragmentShader, renderer::ShaderStage::FRAGMENT);
        _shader = renderer::Shader::Create("default_shader", _vs, _fs);

        // create mesh
        _mesh = renderer::Mesh::Create("default_mesh");
        _mesh->vertices = {
            {-0.5f, -0.5f, 0.0f},
            {0.5f, -0.5f, 0.0f},
            {0.0f,  0.5f, 0.0f}
        };

    }

    void Update(double dt) override {
        // poll events
        std::shared_ptr<event::Signal> s;
        while (Events.Poll(s)) {
            std::cout << "bedlam/include/runtime.hpp: event " << s->name << std::endl;
            if (s->name == "quit") {
                Window.Close();
                std::cout << "bye bye window" << std::endl;
                return;
            }
        }

        _time += dt;    // should be deleted and moved to Time system
        _obj.position.x = 0.5f * sin(_time);

        // make draw call of obj at position
        Renderer.Clear();
        _shader->Bind();
        _mesh->Draw();

    }

    void End() override {
        // clean up resources if necessary
    }
    
};

} // marathon
