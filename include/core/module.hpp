#pragma once

#include <string>
#include <map>



namespace marathon {

// as enum for implicit int conversion
enum class ModuleType {
    WINDOW,
    INPUT,
    MATHS,
    RENDERER,
    AUDIO,
    EVENT,
    TIME,
    MAX_ENUM
};

//// TODO:
// Should probably keep modules in a registry together so all modules can access one another on the backend without making access public to fundamental module objects

class Module {
public:
    Module(ModuleType mType, const std::string& name);
    virtual ~Module() = default;

    // standard methods for safe start/end/rebooting modules
    /// TODO:
    // Should be removed and all boot/shutdown should be migrated to constructor/destructor
    virtual void Boot() = 0;
    virtual void Shutdown() = 0;
    
    std::string GetName() const;
    ModuleType GetType() const;

private:
    std::string _name;
    ModuleType _mType;
};

} // marathon
