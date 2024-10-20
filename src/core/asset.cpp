#include "core/asset.hpp"

namespace marathon {

Asset::Asset(const std::string& name, const std::string& path)
    : name(name), path(path) {}

} // namespace marathon