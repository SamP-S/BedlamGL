#pragma once

// PRIVATE HEADER

#include "la_extended.h"
#include "core/resource.hpp"

namespace marathon {

namespace renderer {

// forward declare
class Renderer;

class Drawable : public Resource {
protected:
    Drawable(const std::string& name);
	virtual ~Drawable() = default;

public:
	virtual void Draw(Renderer& renderer) = 0;
};

} // renderer

} // marathon
