#include "renderer/opengl/shader.hpp"

namespace marathon {

namespace renderer {

namespace opengl {


bool Shader::HasUniform(const std::string& key) const {
    return glGetUniformLocation(_program, key.c_str()) != -1;
}

// single value uniforms
bool Shader::SetUniform(const std::string& key, bool value) const {
    if (!HasUniform(key)) return false;
    glUniform1i(glGetUniformLocation(_program, key.c_str()), static_cast<int>(value));
    return true;
}

bool Shader::SetUniform(const std::string& key, int value) const {
    if (!HasUniform(key)) return false;
    glUniform1i(glGetUniformLocation(_program, key.c_str()), value);
    return true;
}

bool Shader::SetUniform(const std::string& key, uint32_t value) const {
    if (!HasUniform(key)) return false;
    glUniform1ui(glGetUniformLocation(_program, key.c_str()), value);
    return true;
}

bool Shader::SetUniform(const std::string& key, float value) const {
    if (!HasUniform(key)) return false;
    glUniform1f(glGetUniformLocation(_program, key.c_str()), value);
    return true;
}

bool Shader::SetUniform(const std::string& key, double value) const {
    if (!HasUniform(key)) return false;
    glUniform1d(glGetUniformLocation(_program, key.c_str()), value);
    return true;
}

// vector uniforms
void Shader::SetUniform(const std::string& key, const LA::vec2& v) const {
    if (!HasUniform(key)) return;
    glUniform2f(glGetUniformLocation(_program, key.c_str()), v.x, v.y);
}

void Shader::SetUniform(const std::string& key, float x, float y) const {
    if (!HasUniform(key)) return;
    glUniform2f(glGetUniformLocation(_program, key.c_str()), x, y);
}

void Shader::SetUniform(const std::string& key, const LA::vec3& v) const {
    if (!HasUniform(key)) return;
    glUniform3f(glGetUniformLocation(_program, key.c_str()), v.x, v.y, v.z);
}

void Shader::SetUniform(const std::string& key, float x, float y, float z) const {
    if (!HasUniform(key)) return;
    glUniform3f(glGetUniformLocation(_program, key.c_str()), x, y, z);
}

void Shader::SetUniform(const std::string& key, const LA::vec4& v) const {
    if (!HasUniform(key)) return;
    glUniform4f(glGetUniformLocation(_program, key.c_str()), v.x, v.y, v.z, v.w);
}

void Shader::SetUniform(const std::string& key, float x, float y, float z, float w) const {
    if (!HasUniform(key)) return;
    glUniform4f(glGetUniformLocation(_program, key.c_str()), x, y, z, w);
}

/// NOTE: uniforms dont need to be transposed from LA to OpenGL
// matrix uniforms
void Shader::SetUniform(const std::string& key, const LA::mat2& m) const {
    if (!HasUniform(key)) return;
    glUniformMatrix2fv(glGetUniformLocation(_program, key.c_str()), 1, GL_FALSE, &m[0][0]);
}

void Shader::SetUniform(const std::string& key, const LA::mat3& m) const {
    if (!HasUniform(key)) return;
    glUniformMatrix3fv(glGetUniformLocation(_program, key.c_str()), 1, GL_FALSE, &m[0][0]);
}

void Shader::SetUniform(const std::string& key, const LA::mat4& m) const {
    if (!HasUniform(key)) return;
    glUniformMatrix4fv(glGetUniformLocation(_program, key.c_str()), 1, GL_FALSE, &m[0][0]);
}

} // namespace opengl

} // namespace renderer

} // marathon

