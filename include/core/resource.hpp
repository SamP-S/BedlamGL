#pragma once

#include <string>
#include <memory>

#include "core/uuid.hpp"

namespace marathon {

struct Resource {
    public:
        UUID uuid;
        std::string name;

        Resource() = default;
        Resource(const Resource&) = default;
        Resource(const std::string& name="Resource");
        ~Resource() = default;
};

} // marathon