#include "core/module.hpp"

namespace marathon {

// Constructor
Module::Module(ModuleType mType, const std::string& name)
    : _name(name), _mType(mType) {}

// Destructor
Module::~Module() {}

// Get the name of the module
std::string Module::GetName() const {
    return _name;
}

// Get the type of the module
ModuleType Module::GetType() const {
    return _mType;
}

} // namespace marathon