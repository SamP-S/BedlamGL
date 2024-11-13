#include <iostream>
#include <string>

#include "marathon.hpp"
#include "renderer/default.hpp"
#include "renderer/shader.hpp"
#include "events/events.hpp"
#include "renderer/mesh.hpp"

using namespace marathon;

struct MyObject {
    LA::vec3 position = LA::vec3();
    LA::vec4 color = LA::vec4({1.0f, 1.0f, 1.0f, 1.0f});
    std::shared_ptr<renderer::Mesh> mesh = nullptr;
};

MyObject _obj; MyObject _obj2;
std::shared_ptr<renderer::Shader> _shader;
std::shared_ptr<renderer::Mesh> _triangleMesh;
std::shared_ptr<renderer::Mesh> _quadMesh;

time::Time& Time = time::Time::Instance();
window::Window& Window = window::Window::Instance();
renderer::Renderer& Renderer = renderer::Renderer::Instance();
events::Events& Events = events::Events::Instance();

void start() {
    // create defaults
    _shader = std::make_shared<renderer::Shader>();
    _shader->SetFragmentSource(renderer::defaultFragmentShader);
    _shader->SetVertexSource(renderer::defaultVertexShader);

    _triangleMesh = std::make_shared<renderer::Mesh>();
    _triangleMesh->SetVertexParams(
        renderer::defaultTriangleVertices.size(), 
        {
            {renderer::VertexAttribute::POSITION, 3, renderer::VertexAttributeFormat::FLOAT}
        }
    );
    _triangleMesh->SetVertexData((void*)&renderer::defaultTriangleVertices[0][0], renderer::defaultTriangleVertices.size() * sizeof(LA::vec3), 0, 0);


    // create mesh with vbo & ibo
    _quadMesh = std::make_shared<renderer::Mesh>();
    _quadMesh->SetVertexParams(
        renderer::defaultQuadVertices.size(), 
        {
            {renderer::VertexAttribute::POSITION, 3, renderer::VertexAttributeFormat::FLOAT}
        }
    );
    _quadMesh->SetVertexData((void*)&renderer::defaultQuadVertices[0][0], renderer::defaultQuadVertices.size() * sizeof(LA::vec3), 0, 0);   
    _quadMesh->SetIndexParams(renderer::defaultQuadIndices.size(), renderer::IndexFormat::UINT32, renderer::PrimitiveType::TRIANGLES);
    _quadMesh->SetIndexData((void*)&renderer::defaultQuadIndices[0], renderer::defaultQuadIndices.size() * sizeof(uint32_t), 0, 0);

    // create object
    _obj = MyObject();
    _obj.color = {1.0f, 0.2f, 0.2f, 1.0f};
    _obj.mesh = _triangleMesh;

    // create object
    _obj2 = MyObject();
    _obj2.color = {0.2f, 0.2f, 1.0f, 1.0f};
    _obj2.mesh = _quadMesh;

    std::string err = "";
    if (!Renderer.ValidateShader(_shader, err)) {
        std::cout << "bedlam/include/runtime.hpp: shader validation failed: \n" << err << std::endl;
    }
    if (!Renderer.ValidateMesh(_triangleMesh, err)) {
        std::cout << "bedlam/include/runtime.hpp: triangle mesh validation failed: \n" << err << std::endl;
    }
    if (!Renderer.ValidateMesh(_quadMesh, err)) {
        std::cout << "bedlam/include/runtime.hpp: quad mesh validation failed: " << err << std::endl;
    }
}

void update(double dt) {
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
        Renderer.SetUniform("uTransform", Renderer.GetModel());
        Renderer.Draw(_obj.mesh);
        Renderer.PopTransform();
        Renderer.PushTranslate(_obj2.position);
        Renderer.SetUniform("uTransform", Renderer.GetModel());
        Renderer.Draw(_obj2.mesh);
        Renderer.PopTransform();
        Renderer.PopTransform();
    } else {
        std::cout << "bedlam/include/runtime.hpp: test obj mesh is null" << std::endl;
    }
}

int main() {
    // initialise
    if (marathon::Init() != 0) {
        std::cout << "Failed to initialise marathon. :(" << std::endl;
        return 1;
    }

    start();


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
    if (marathon::Quit() != 0) {
        std::cout << "Failed to quit marathon. :(" << std::endl;
        return 1;
    }
    return 0;
}