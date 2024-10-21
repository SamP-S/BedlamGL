#pragma once

#include <iostream>

#include "marathon.hpp"
#include "renderer/shader_source.hpp"
#include "renderer/shader.hpp"
#include "event/event.hpp"
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
    std::shared_ptr<renderer::ShaderSource> _vs;
    std::shared_ptr<renderer::ShaderSource> _fs;
    std::shared_ptr<renderer::Shader> _shader;
    std::shared_ptr<renderer::Mesh> _triangleMesh;
    std::shared_ptr<renderer::Mesh> _quadMesh;

public:
    Runtime() {}
    ~Runtime() {}

    void Start() override {
        // load shaders
        _vs = renderer::ShaderSource::Create("default_vs", renderer::defaultVertexShader, renderer::ShaderStage::VERTEX);
        _fs = renderer::ShaderSource::Create("default_fs", renderer::defaultFragmentShader, renderer::ShaderStage::FRAGMENT);
        _shader = renderer::Shader::Create("default_shader", _vs, _fs);

        // create triangle mesh
        _triangleMesh = renderer::Mesh::Create("default_triangle_mesh");
        _triangleMesh->vertices = renderer::defaultTriangleVertices;

        // create quad mesh
        _quadMesh = renderer::Mesh::Create("default_quad_mesh");
        _quadMesh->vertices = renderer::defaultQuadVertices;
        _quadMesh->indices = renderer::defaultQuadIndices;
        
        // create object
        _obj = MyObject();
        _obj.color = {1.0f, 0.2f, 0.2f, 1.0f};
        _obj.mesh = _triangleMesh;

        _obj2 = MyObject();
        _obj2.color = {0.2f, 1.0f, 0.2f, 1.0f};
        _obj2.mesh = _quadMesh;
    }

    void Update(double dt) override {
        // poll events
        std::shared_ptr<event::Signal> s;
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
        Renderer.BindShader(_shader);
        Renderer.Draw(_obj.mesh, LA::Transformation(_obj.position, LA::vec3(), LA::vec3(0.5f)));
        Renderer.Draw(_obj2.mesh, LA::Transformation(_obj2.position, LA::vec3(), LA::vec3(0.25f)));
    }

    void End() override {
        // clean up resources if necessary
    }
    
};

} // marathon
