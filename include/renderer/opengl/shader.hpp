#pragma once

#include <unordered_map>

#include "renderer/shader.hpp"
#include "renderer/opengl/opengl.hpp"

namespace marathon {

namespace renderer {

class Renderer;

namespace opengl {

class Renderer;

class Shader : public renderer::Shader {
protected:
    friend class Renderer;
    friend class renderer::Renderer;

    GLuint _program = 0;
    std::string _warnings;

    static const std::unordered_map<ShaderType, GLenum> s_shaderTypeMap;

    Shader(const std::string& vSrc, const std::string& fSrc);
    
public:
    ~Shader() override;

    void Bind() override;
    void Unbind() override;

    std::string GetWarnings() const override;
    bool HasUniform(const std::string& key) const override;

    // single value uniforms
    bool SetUniform(const std::string& key, bool value) const override;
    bool SetUniform(const std::string& key, int value) const override;
    bool SetUniform(const std::string& key, uint32_t value) const override;
    bool SetUniform(const std::string& key, float value) const override;
    bool SetUniform(const std::string& key, double value) const override;
    // vector uniforms
    void SetUniform(const std::string& key, const LA::vec2& v) const override;
    void SetUniform(const std::string& key, float x, float y) const override;
    void SetUniform(const std::string& key, const LA::vec3& v) const override;
    void SetUniform(const std::string& key, float x, float y, float z) const override;
    void SetUniform(const std::string& key, const LA::vec4& v) const override;
    void SetUniform(const std::string& key, float x, float y, float z, float w) const override;
    // matrix uniforms
    void SetUniform(const std::string& key, const LA::mat2& m) const override;
    void SetUniform(const std::string& key, const LA::mat3& m) const override;
    void SetUniform(const std::string& key, const LA::mat4& m) const override;
};


} // namespace opengl

} // namespace renderer

} // namespace marathon

