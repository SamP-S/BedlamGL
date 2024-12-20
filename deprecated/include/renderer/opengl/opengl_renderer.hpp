#pragma once

#include <map>
#include <assert.h>
#include <string>
#include <vector>
#include <iostream>
#include <istream>
#include <fstream>
#include <sstream>
#include <memory>

#include "renderer/renderer.hpp"
#include "renderer/opengl/opengl.hpp"
#include "renderer/opengl/opengl_shader.hpp"
#include "renderer/opengl/opengl_mesh.hpp"

namespace marathon {

namespace renderer {

namespace opengl {

//// TODO:
// no method for glViewport(0, 0, width, height);

//// NOTES:
// switching shader will need all uniforms reassigned
// switching framebuffer will ONLY affect where pixels are drawn to
// switching material/proj/view will ONLY affect bound shader

class OpenGLRenderer : public Renderer {
public:
    void Boot() override;
    void Shutdown() override;

    // set null to draw directly to window
    void SetFrameBuffer(std::shared_ptr<FrameBuffer> fb);
    std::shared_ptr<FrameBuffer> GetFrameBuffer();
    void SetProjection(const LA::mat4& proj) override;
    LA::mat4 GetProjection() override;
    void SetView(const LA::mat4& view) override;
    LA::mat4 GetView() override;

    void Draw(std::shared_ptr<Drawable> drawable, const LA::mat4& transform) override;
    void BindShader(std::shared_ptr<Shader> shader) override;

    void Clear() override;
    
    static Renderer& Instance();

protected:
    OpenGLRenderer()
        : Renderer("marathon.renderer.opengl.OpenGLRenderer") {}
    ~OpenGLRenderer() = default;
};

} // opengl

} // renderer

} // marathon