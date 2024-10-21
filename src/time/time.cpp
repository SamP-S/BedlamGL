#include "time/time.hpp"

namespace marathon {

namespace time {

Time::Time(const std::string& name)
    : Module(ModuleType::TIME, name),
      _start(std::chrono::high_resolution_clock::now()),
      _lastTick(std::chrono::high_resolution_clock::now()),
      _dt(0.0) {}

Time& Time::Instance() {
    static Time* instance;
    if (!instance)
        instance = new Time("marathon.time");
    return *instance;
}

// module interface
void Time::Boot() {
    _start = std::chrono::high_resolution_clock::now();
    _lastTick = _start;
}
void Time::Shutdown() {}

// common
double Time::Tick() {
    auto now = std::chrono::high_resolution_clock::now();
    _dt = std::chrono::duration<double>(now - _lastTick).count();
    _lastTick = now;
    return _dt;
}
double Time::GetDeltaTime() {
    return _dt;
}
double Time::GetTime() {
    auto now = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double>(now - _start).count();
}
double Time::GetFPS() {
    return 1.0 / _dt;
}

} // time

} // marathon