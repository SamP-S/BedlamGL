#pragma once

#include <string>
#include <chrono>

#include "core/module.hpp"

namespace marathon {

namespace time {

class Time : public marathon::Module {
protected:
    Time(const std::string& name);

public:
    ~Time() = default;

    static Time& Instance();

    // module interface
    void Boot() override;
    void Shutdown() override;

    /// TODO: 
    // implement averages rather than just the last frame/tick
    // common
    double Tick();
    double GetDeltaTime();
    double GetTime();
    double GetFPS();
};

} // time

} // marathon