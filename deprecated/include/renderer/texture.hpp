#pragma once

#include <string>

#include "renderer/drawable.hpp"

namespace marathon {

namespace renderer {

class Texture : public Drawable {
protected:


    int width;
    int height;
    int channels;

    virtual ~Texture() = 0;

public:
    Texture(const std::string& name);

    /// TODO:
    /// mipmaps
    /// pixel formats
    /// get/set texture parameters(wrapping/filtering)
    /// texture types

    int GetWidth() { return width; }
    int GetHeight() { return height; }
    int GetChannels() { return channels;}
    void Bind();


};

} // renderer

} // marathon
