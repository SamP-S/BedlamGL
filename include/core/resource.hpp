#pragma once

#include <string>
#include <memory>

#include "core/uuid.hpp"

namespace marathon {

struct Resource {
private:
    UUID _uuid;
    std::string _name;

public:
    Resource() = default;
    Resource(const Resource&) = default;
    Resource(const std::string& name="Resource");
    ~Resource() = default;

    UUID GetUUID() const;
    std::string GetName() const;

};

} // marathon