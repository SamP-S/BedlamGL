#include "core/resource.hpp"

namespace marathon {

Resource::Resource(const std::string& type)
    : _mType(type) {}

UUID Resource::GetUUID() const { return _mUUID; }
std::string Resource::GetName() const { return _mName; }
void Resource::SetName(const std::string& name) { _mName = name; }
std::string Resource::GetType() const { return _mType; }

} // marathon