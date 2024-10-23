#include "core/resource.hpp"

namespace marathon {

Resource::Resource(const std::string& name)
    : _name(name) {}

std::string Resource::GetName() const { return _name; }
UUID Resource::GetUUID() const { return _uuid; }

} // marathon