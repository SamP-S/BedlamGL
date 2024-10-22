#include "renderer/opengl/opengl_shader_source.hpp"

namespace marathon {

namespace renderer {

namespace opengl {

const std::map<ShaderStage, GLenum> ShaderSource::_stageToGL = {
    {ShaderStage::INVALID, 0},
    {ShaderStage::VERTEX, GL_VERTEX_SHADER},
    {ShaderStage::FRAGMENT, GL_FRAGMENT_SHADER},
    {ShaderStage::COMPUTE, GL_COMPUTE_SHADER},
    {ShaderStage::GEOMETRY, GL_GEOMETRY_SHADER},
    {ShaderStage::TESSELLATION_CONTROL, GL_TESS_CONTROL_SHADER},
    {ShaderStage::TESSELLATION_EVALUATION, GL_TESS_EVALUATION_SHADER}
};

ShaderSource::ShaderSource(const std::string& name, const std::string& source, ShaderStage stage)
    : ShaderSource(name, source, stage) {}

uint32_t ShaderSource::Compile() {
    // create and attempt to compile shader from source
    uint32_t shader = glCreateShader(_stageToGL.at(_stage));
    const GLchar* sourceCStr = _source.c_str();
    glShaderSource(shader, 1, &sourceCStr, NULL);
    glCompileShader(shader);

    // validate success
    int success;
    static char infoLog[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        throw infoLog;
    }
    return shader;
}

} // opengl

} // renderer

} // marathon