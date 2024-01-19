#define GL_VERSION_4_4
#include <GL/glew.h>
#include <SDL_opengl.h>

// includes
#include <map>
#include <assert.h>
#include <string>
#include <vector>
#include <iostream>
#include <istream>
#include <fstream>
#include <sstream>
#include <memory>

#include "ngine/ngine.hpp"
#include "ngine/serializer.hpp"
#include "tai/tai.hpp"

#include "la_extended.h"

#include "core/frame_timer.hpp"
#include "platform/opengl/opengl_shader.hpp"
#include "platform/opengl/opengl_shader_source.hpp"
#include "platform/opengl/opengl_frame_buffer.hpp"

#include "renderer/mesh.hpp"
#include "renderer/editor_camera.hpp"

#include "renderer/model_loader.hpp"
#include "renderer/shader_loader.hpp"

class GraphicsEngine {
    private:
        int _width, _height;

    public:
        frame_timer ft = frame_timer();
        EditorCamera editorCamera = EditorCamera();
        std::shared_ptr<Scene> scene = std::make_shared<Scene>();
        Tai::AssetManager& assetManager = Tai::AssetManager::Instance();
        Tai::LoaderManager loaderManager = Tai::LoaderManager();
        std::shared_ptr<FrameBuffer> frameBuffer = nullptr;

        GraphicsEngine(int width, int height) :
        _width(width), _height(height) { }
        ~GraphicsEngine() {}

        void Initialise() {
            SetViewport(_width, _height);
            
            // Initialise GLEW
            // Must be done before any opengl call
            // easier done before application instanced
            glewExperimental = GL_TRUE;
            glewInit();

            frameBuffer = assetManager.CreateAsset<OpenGLFrameBuffer>("FBO", _width, _height);
            frameBuffer->SetClearColour(0.2f, 0.2f, 0.2f, 1.0f);

            // add loaders to asset libary
            loaderManager.AddLoader(new ModelLoader());
            loaderManager.AddLoader(new ShaderLoader());

            // load default model(s)
            loaderManager.Load("models/presets/cone.gltf");
            loaderManager.Load("models/presets/cube.gltf");
            loaderManager.Load("models/presets/cylinder.gltf");
            loaderManager.Load("models/presets/dome.gltf");
            loaderManager.Load("models/presets/ico_sphere.gltf");
            loaderManager.Load("models/presets/plane.gltf");
            loaderManager.Load("models/presets/prism.gltf");
            loaderManager.Load("models/presets/sphere.gltf");
            // load default shader(s)
            loaderManager.Load("shaders/base.vert");
            loaderManager.Load("shaders/base.frag");
            loaderManager.Load("shaders/lighting.vert");
            loaderManager.Load("shaders/lighting.frag");
            assetManager.CreateAsset<OpenGLShader>(
                "base",
                assetManager.GetAsset<OpenGLShaderSource>("base_vert"),
                assetManager.GetAsset<OpenGLShaderSource>("base_frag")
            );
            assetManager.CreateAsset<OpenGLShader>(
                "lighting",
                assetManager.GetAsset<OpenGLShaderSource>("lighting_vert"),
                assetManager.GetAsset<OpenGLShaderSource>("lighting_frag")
            );

            LoadScene("scene/Preset.json");    

            // setup editor camera
            editorCamera.transform.position = LA::vec3{-8,5,8};
            editorCamera.transform.rotation = LA::vec3({PI/2, PI, 0.0f});
        }
        
        // function to reset our viewport after a window resize
        void SetViewport(int width, int height) {
            // protect against divide by 0 and no resoltuion
            if (width == 0) {
                std::cout << "WARNING (Graphics): Trying to set width 0" << std::endl;
                width = 1;
            }
                
            if (height == 0) {
                std::cout << "WARNING (Graphics): Trying to set height 0" << std::endl;
                height = 1;
            }
                
            _width = width;
            _height = height;
            float ratio = width / height;
        }

        void LoadScene(const std::string& filepath) {
            // Load Scene
            JsonSerializer js = JsonSerializer(scene);
            js.Deserialize(filepath);
        }

        void SaveScene(const std::string& filepath) {
            std::cout << "NOT IMPLEMENTED" << std::endl;
        }

        void ShaderDirectionalLights(std::shared_ptr<Shader> shader) {
            std::vector<Entity> entities = scene->GetEntitiesWith<DirectionalLightComponent>();
            
            // directional lights
            if (entities.size() >= DIRECTIONAL_LIGHT_MAX) {
                std::cout << "WARNING (Graphics): Too many directional lights, only first 4 will be used" << std::endl;
            }
            for (int i = 0; i < DIRECTIONAL_LIGHT_MAX; i++) {
                std::string index = "[" + std::to_string(i) + "]";
                if (i < entities.size()) {
                    TransformComponent& tc = entities[i].GetComponent<TransformComponent>();
                    DirectionalLightComponent& dlc = entities[i].GetComponent<DirectionalLightComponent>();
                    shader->SetVec3("iDirectionalLights" + index + ".direction", tc.GetForward());
                    shader->SetFloat("iDirectionalLights" + index + ".intensity", dlc.intensity);
                    shader->SetVec3("iDirectionalLights" + index + ".colour", dlc.colour);
                    shader->SetInt("iDirectionalLights" + index + ".enabled", 1);
                } else {
                    shader->SetInt("iDirectionalLights" + index + ".enabled", 0);
                }
            }
        }

        void ShaderPointLights(std::shared_ptr<Shader> shader) {
            std::vector<Entity> entities = scene->GetEntitiesWith<PointLightComponent>();

            // point lights
            if (entities.size() >= POINT_LIGHT_MAX) {
                std::cout << "WARNING (Graphics): Too many point lights, only first 16 will be used" << std::endl;
            }
            for (int i = 0; i < POINT_LIGHT_MAX; i++) {
                std::string index = "[" + std::to_string(i) + "]";
                if (i < entities.size()) {
                    TransformComponent& tc = entities[i].GetComponent<TransformComponent>();
                    PointLightComponent& plc = entities[i].GetComponent<PointLightComponent>();
                    shader->SetVec3("iPointLights" + index + ".position", tc.position);
                    shader->SetFloat("iPointLights" + index + ".intensity", plc.intensity);
                    shader->SetVec3("iPointLights" + index + ".colour", plc.colour);
                    shader->SetInt("iPointLights" + index + ".enabled", 1);
                } else {
                    shader->SetInt("iPointLights" + index + ".enabled", 0);
                }
            }
        }

        void SetupShader(std::shared_ptr<Shader> shader) {
            shader->Bind();
            // vertex uniforms
            shader->SetMat4("iView", &inverse(editorCamera.transform.GetTransform())[0][0]);
            shader->SetMat4("iProjection", &(editorCamera.GetProjection())[0][0]);
            // fragment uniforms
            shader->SetVec3("iResolution", _width, _height, 1.0f);
            shader->SetFloat("iTime", ft.GetTotalElapsed());
            shader->SetFloat("iTimeDelta", ft.GetFrameElapsed());
            shader->SetInt("iFrame", ft.GetFrameCount());
            shader->SetVec3("iCameraPosition", editorCamera.transform.position); 
 
            ShaderDirectionalLights(shader);
            ShaderPointLights(shader);     
        }

        void Draw(MeshRendererComponent& renderer, bool wireframe=false) {
            if (renderer.mesh != 0) {
                std::shared_ptr<Mesh> mesh = assetManager.GetAsset<Mesh>(renderer.mesh);
                if (mesh == NULL || !mesh->GetIsGenerated()) {
                    std::cout << "WARNING (Graphics): Attempting to render a bad mesh." << std::endl;
                    return;
                }

                GL_Interface::BindVertexArrayObject(mesh->vao);
                if (wireframe) {
                    GL_Interface::PolygonMode(POLYGON_LINE);
                    GL_Interface::DrawArrays(DRAW_TRIANGLES, 0, mesh->GetVerticesSize());
                    GL_Interface::DrawElements(DRAW_TRIANGLES, mesh->GetIndiciesSize(), TYPE_UINT);
                    GL_Interface::PolygonMode(POLYGON_FILL);
                } else {
                    GL_Interface::DrawArrays(DRAW_TRIANGLES, 0, mesh->GetVerticesSize());
                    GL_Interface::DrawElements(DRAW_TRIANGLES, mesh->GetIndiciesSize(), TYPE_UINT);
                }
                return;
            }
        }

        void RenderObject(Entity entity, mat4 root_trans = mat4(), std::shared_ptr<Shader> shader=nullptr, bool wireframe=false) {
            TransformComponent& tc = entity.GetComponent<TransformComponent>();
            mat4 model = root_trans *  tc.GetTransform();

            // check entity has renderer
            if (!entity.HasComponent<MeshRendererComponent>())
                return;
            // check mesh is not empty
            MeshRendererComponent& mrc = entity.GetComponent<MeshRendererComponent>();
            if (mrc.mesh == 0)
                return;
            shader->SetMat4("iModel", &model[0][0]);
            Draw(mrc, wireframe);
        }

        void Render() {
            frameBuffer->Bind();
            frameBuffer->Clear();
            
            std::vector<std::shared_ptr<Tai::Asset>> shaders = assetManager.GetAssets<OpenGLShader>();
            for (auto shader : shaders) {
                auto ptr = std::dynamic_pointer_cast<Shader>(shader);
                SetupShader(ptr);
            }

            std::shared_ptr<Shader> baseShader = assetManager.GetAsset<OpenGLShader>("base");
            std::shared_ptr<Shader> lightingShader = assetManager.GetAsset<OpenGLShader>("lighting");

            editorCamera.Update();

            std::vector<Entity> renderables = scene->GetEntitiesWith<MeshRendererComponent>();
            for (auto& ent : renderables) {
                // draw lit
                GL_Interface::EnableFeature(FEATURE_DEPTH);
                GL_Interface::EnableFeature(FEATURE_CULL);
                GL_Interface::SetFrontFace(FRONT_CCW);
                lightingShader->Bind();
                RenderObject(ent, mat4(), lightingShader, false);

                // draw wireframe
                GL_Interface::EnableFeature(FEATURE_DEPTH);
                GL_Interface::EnableFeature(FEATURE_CULL);
                baseShader->Bind();
                RenderObject(ent, mat4(), baseShader, true);
            }

            frameBuffer->Unbind();
            
            ft.Frame();
        }

};