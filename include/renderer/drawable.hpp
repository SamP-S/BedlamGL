#pragma once

#include "la_extended.h"
#include "core/resource.hpp"

namespace marathon {

namespace renderer {

// forward declare
class Renderer;

class Drawable : public Resource {
public:
    Drawable(const std::string& name);
	virtual ~Drawable() = default;
	virtual void Draw(Renderer& renderer, const LA::mat4& m) = 0;
};

} // renderer

} // marathon
