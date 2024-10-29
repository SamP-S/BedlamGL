#pragma once

#include <iostream>

#include "marathon.hpp"
#include "renderer/shader.hpp"
#include "events/events.hpp"
#include "renderer/mesh.hpp"


namespace marathon {

struct MyObject {
    LA::vec3 position = LA::vec3();
    LA::vec4 color = LA::vec4({1.0f, 1.0f, 1.0f, 1.0f});
    std::shared_ptr<renderer::Mesh> mesh = nullptr;
};

class Runtime : public Interactive {
private:
    MyObject _obj; MyObject _obj2;
    std::shared_ptr<renderer::Shader> _shader;
    std::shared_ptr<renderer::Buffer> _triVBuf;
    std::shared_ptr<renderer::Mesh> _triangleMesh;
    std::shared_ptr<renderer::Mesh> _quadMesh;

public:
    time::Time& Time = time::Time::Instance();
    window::Window& Window = window::Window::Instance();
    renderer::Renderer& Renderer = renderer::Renderer::Instance();
    events::Events& Events = events::Events::Instance();

    Runtime() {}
    ~Runtime() {}

    void Start() override {
        // load shaders
        _shader = Renderer.CreateShader(renderer::defaultVertexShader, renderer::defaultFragmentShader);

        // create triangle mesh
        _triVBuf = Renderer.CreateBuffer((void*)&renderer::defaultTriangleVertices[0][0], renderer::defaultTriangleVertices.size() * sizeof(LA::vec3), renderer::BufferTarget::VERTEX, renderer::BufferUsage::STATIC);
        _triangleMesh = Renderer.CreateMesh(3, sizeof(LA::vec3), _triVBuf, {{0, 3, renderer::AttributeType::FLOAT}}, renderer::PrimitiveType::TRIANGLES);

        // // create quad mesh
        // _quadMesh = renderer::Mesh::Create("default_quad_mesh");
        // _quadMesh->vertices = renderer::defaultQuadVertices;
        // _quadMesh->indices = renderer::defaultQuadIndices;
        
        // create object
        _obj = MyObject();
        _obj.color = {1.0f, 0.2f, 0.2f, 1.0f};
        _obj.mesh = _triangleMesh;

        // _obj2 = MyObject();
        // _obj2.color = {0.2f, 1.0f, 0.2f, 1.0f};
        // _obj2.mesh = _quadMesh;
    }

    void Update(double dt) override {
        // poll events
        std::shared_ptr<events::Signal> s;
        while (Events.Poll(s)) {
            // std::cout << "bedlam/include/runtime.hpp: event " << s->name << std::endl;
            if (s->name == "quit") {
                Window.Close();
                std::cout << "bye bye window" << std::endl;
                return;
            }
        }

        double time = Time.GetTime();
        _obj.position.x = 0.5f * sin(time);
        _obj2.position.y = 0.5f * cos(time * 2);
        // std::cout << "bedlam/include/runtime.hpp: time = " << time << " :: sin = " << sin(time) << std::endl;

        // make draw call of obj at position
        Renderer.Clear();
        Renderer.SetShader(_shader);
        if (_obj.mesh != nullptr) {
            Renderer.PushScale({0.5f, 0.5f, 0.5f});
            Renderer.PushTranslate(_obj.position);
            Renderer.GetShader()->SetUniform("uTransform", Renderer.GetModel());
            Renderer.Draw(*_obj.mesh.get());
            Renderer.PopTransform();
            Renderer.PopTransform();
        } else {
            std::cout << "bedlam/include/runtime.hpp: test obj mesh is null" << std::endl;
        }
    }

    void End() override {
        // clean up resources if necessary
    }
    
};

} // marathon
