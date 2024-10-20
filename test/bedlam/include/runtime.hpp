#pragma once

#include "marathon.hpp"

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
    double time = 0.0;

public:
    Runtime() {}
    ~Runtime() {}

    void Start() override {
        // create object
        _obj = MyObject();
        _obj.color = {1.0f, 0.2f, 0.2f, 1.0f};

        // load shaders
        
    }

    void Update(double dt) override {
        time += dt;
        _obj.position.x = 0.5f * sin(time);

        // make draw call of obj at position
    }

    void End() override {
        // clean up resources if necessary
    }
    
};

} // marathon
