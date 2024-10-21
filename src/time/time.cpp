#include "time/time.hpp"

namespace marathon {

namespace time {

Time::Time(const std::string& name)
    : Module(ModuleType::TIME, name) {
    
}

Time& Time::Instance() {
    static Time* instance;
    if (!instance)
        instance = new Time("marathon.time");
    return *instance;
}

// module interface
void Boot() {

}
void Time::Shutdown() {

}

// common
double Time::Tick() {}
double Time::GetDeltaTime() {}
double Time::GetTime() {}
double Time::GetFPS() {}


} // time

} // marathon