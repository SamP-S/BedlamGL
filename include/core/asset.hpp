#pragma once

#include <string>
#include <memory>

#include "core/uuid.hpp"

namespace marathon {

struct Asset {
    public:
        UUID uuid;
        std::string name;
        std::string path;

        Asset() = default;
        Asset(const Asset&) = default;
        Asset(const std::string& name="Asset", const std::string& path="");
        ~Asset() = default;
};

} // marathon