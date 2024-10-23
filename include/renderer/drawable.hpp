#pragma once

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
	void Draw(Renderer& renderer);
    virtual void Draw(Renderer& renderer, const LA::mat4& transform) = 0;
};

} // renderer

} // marathon
