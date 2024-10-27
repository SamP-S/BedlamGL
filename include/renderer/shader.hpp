#pragma once

// PUBLIC HEADER

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>

#include "la_extended.h"
#include "core/resource.hpp"

namespace marathon {

namespace renderer {

/// TODO:
// add support for seperated shader stages utilising defaults
// support a general default data layout for vertex attributes
// support a wider range of uniform sends

enum class ShaderType {
    VERTEX,
    FRAGMENT
};

class Shader : public Resource {
protected:
    std::string _vSrc = "";
    std::string _fSrc = "";

    Shader(const std::string& name, const std::string& vSrc, const std::string& fSrc);
    virtual ~Shader();

    virtual void Bind() = 0;
    virtual void Unbind() = 0;

public:
    virtual std::string GetWarnings() const = 0;
    virtual bool HasUniform(const std::string& key) const = 0;

    // single value uniforms
    virtual bool SetUniform(const std::string& key, bool value) const = 0;
    virtual bool SetUniform(const std::string& key, int value) const = 0;
    virtual bool SetUniform(const std::string& key, uint32_t value) const = 0;
    virtual bool SetUniform(const std::string& key, float value) const = 0;
    virtual bool SetUniform(const std::string& key, double value) const = 0;
    // vector uniforms
    virtual void SetUniform(const std::string& key, const LA::vec2& v) const = 0;
    virtual void SetUniform(const std::string& key, float x, float y) const = 0;
    virtual void SetUniform(const std::string& key, const LA::vec3& v) const = 0;
    virtual void SetUniform(const std::string& key, float x, float y, float z) const = 0;
    virtual void SetUniform(const std::string& key, const LA::vec4& v) const = 0;
    virtual void SetUniform(const std::string& key, float x, float y, float z, float w) const = 0;
    // matrix uniforms
    virtual void SetUniform(const std::string& key, const LA::mat2& m) const = 0;
    virtual void SetUniform(const std::string& key, const LA::mat3& m) const = 0;
    virtual void SetUniform(const std::string& key, const LA::mat4& m) const = 0;
};

}

}