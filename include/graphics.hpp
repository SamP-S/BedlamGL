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

#include "window.hpp"
#include "frame_timer.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "mesh.hpp"
#include "entity.hpp"
#include "la_extended.h"
using namespace LA;
#include "camera.hpp"
#include "model.hpp"
#include "frame.hpp"
#include "mesh_renderer.hpp"
#include "first_person.hpp"
#include "resource_manager.hpp"
#include "asset_manager.hpp"
#include "light_directional.hpp"
#include "light_point.hpp"
#include "filepath.hpp"
#include "json.hpp"
using namespace nlohmann;


class GraphicsEngine {

    public:
        void* context;
        WindowManager* window;
        frame_timer ft = frame_timer();
        Frame* frame = NULL;
        int width, height;
        Entity* rootEntity = NULL;
        Entity* workspaceCamera = NULL;

        FirstPersonController* fpc;
        Camera* camera;

        GraphicsEngine(WindowManager* window) {
            AttachWindow(window);
            SetViewport(window->width, window->height);
            
            // Initialise GLEW
            // Must be done before any opengl call
            // easier done before application instanced
            glewExperimental = GL_TRUE;
            glewInit();

            frame = new Frame(window->width, window->height);
            frame->SetClearColour(0.0f, 0.0f, 0.0f, 1.0f);
             
            GL_Interface::BindFrameBufferObj(0);

            // load default shader(s)
            resourceShaders.Add(new Shader("base", "shaders/base.vs", "shaders/base.fs"));
            resourceShaders.Add(new Shader("lighting", "shaders/lighting.vs", "shaders/lighting.fs"));

            // load default model(s)
            assetManager.Load("models/presets/cone.gltf");
            assetManager.Load("models/presets/cube.gltf");
            assetManager.Load("models/presets/cylinder.gltf");
            assetManager.Load("models/presets/dome.gltf");
            assetManager.Load("models/presets/ico_sphere.gltf");
            assetManager.Load("models/presets/plane.gltf");
            assetManager.Load("models/presets/prism.gltf");
            assetManager.Load("models/presets/sphere.gltf");

            /// TODO: load project shader(s)
            rootEntity = new Entity("scene", NULL);

            LoadScene("scene/Preset.json");

            // Entity* cube = new Entity("cube", rootEntity);
            // rootEntity->AddChild(cube);
            // Component* c = new MeshRenderer(*cube, resourceMeshes.GetId("cube::0::Cube"));
            // cube->AddComponent(c);

            workspaceCamera = new Entity("Workspace Camera", NULL);
            camera = new Camera(*workspaceCamera);
            fpc = new FirstPersonController(*workspaceCamera);
            
            workspaceCamera->transform.SetPosition(vec3{-8,5,8});
            workspaceCamera->transform.SetRotation(vec3({PI/2, PI, 0.0f}));

            resourceShaders.Get("base")->Use();
            camera->SetResolution(width, height);
        }

        ~GraphicsEngine() {
            // delete context; delete makes error even though pointer?!?
            // delete window; causesd unknown signal error?
            delete frame;
        }

        /// TODO: Implement as recursive and implement FromJson & ToJson functions for everything
        bool LoadScene(std::string filepath) {
            std::ifstream inputFile(filepath);
            if (!inputFile.good()) {
                std::cout << "WARNING (Graphics): Scene file does not exist: " << filepath << std::endl;
            }
            if (!inputFile.is_open()) {
                std::cout << "WARNING (Graphics): Can't open file: " << filepath << std::endl;
                return false;
            }

            json jsonFile = json::parse(inputFile);
            for (const auto& entJson : jsonFile) {
                // create entity
                Entity* ent = new Entity("Default Name", rootEntity);
                ent->FromJson(entJson);
                rootEntity->AddChild(ent);
            }
            inputFile.close();
            return true;
        }

        bool AttachWindow(WindowManager* window) {
            if (window == NULL) {
                return false;
            }
            this->window = window;
            return true;
        }

        bool DetachWindow() {
            this->window = NULL;
            return true;
        }
        
        // function to reset our viewport after a window resize
        void SetViewport(int _width, int _height) {
            // protect against divide by 0 and no resoltuion
            if (_width == 0) {
                std::cout << "WARNING (Graphics): Trying to set width 0" << std::endl;
                _width = 1;
            }
                
            if (_height == 0) {
                std::cout << "WARNING (Graphics): Trying to set height 0" << std::endl;
                _height = 1;
            }
                
            this->width = _width;
            this->height = _height;
            float ratio = width / height;
        }

        void ShaderDirectionalLights(Shader& shader) {
            // directional lights
            std::vector<DirectionalLight*> dirLights = rootEntity->GetComponentsInChildren<DirectionalLight>();
            if (dirLights.size() >= DIRECTIONAL_LIGHT_MAX) {
                    std::cout << "WARNING (Graphics): Too many directional lights, only first 4 will be used" << std::endl;
                }
            for (int i = 0; i < DIRECTIONAL_LIGHT_MAX; i++) {
                std::string index = "[" + std::to_string(i) + "]";
                if (i < dirLights.size()) {
                    shader.SetVec3("iDirectionalLights" + index + ".direction", dirLights[i]->transform.GetForward());
                    shader.SetFloat("iDirectionalLights" + index + ".intensity", dirLights[i]->GetIntensity());
                    shader.SetVec3("iDirectionalLights" + index + ".colour", dirLights[i]->GetColour());
                    shader.SetInt("iDirectionalLights" + index + ".enabled", 1);
                } else {
                    shader.SetInt("iDirectionalLights" + index + ".enabled", 0);
                }
            }
        }

        void ShaderPointLights(Shader& shader) {
            // point lights
            std::vector<PointLight*> pointLights = rootEntity->GetComponentsInChildren<PointLight>();
            if (pointLights.size() >= POINT_LIGHT_MAX) {
                std::cout << "WARNING (Graphics): Too many point lights, only first 16 will be used" << std::endl;
            }
            for (int i = 0; i < POINT_LIGHT_MAX; i++) {
                std::string index = "[" + std::to_string(i) + "]";
                if (i < pointLights.size()) {
                    shader.SetVec3("iPointLights" + index + ".position", pointLights[i]->transform.GetPosition());
                    shader.SetFloat("iPointLights" + index + ".intensity", pointLights[i]->GetIntensity());
                    shader.SetVec3("iPointLights" + index + ".colour", pointLights[i]->GetColour());
                    shader.SetInt("iPointLights" + index + ".enabled", 1);
                } else {
                    shader.SetInt("iPointLights" + index + ".enabled", 0);
                }
            }
        }

        void SetupShader(Shader& shader) {
            shader.Use();
            // vertex uniforms
            shader.SetMat4("iView", &fpc->view[0][0]);
            shader.SetMat4("iProjection", &camera->proj[0][0]);
            // fragment uniforms
            shader.SetVec3("iResolution", window->width, window->height, 1.0f);
            shader.SetFloat("iTime", ft.GetTotalElapsed());
            shader.SetFloat("iTimeDelta", ft.GetFrameElapsed());
            shader.SetInt("iFrame", ft.GetFrameCount());
            shader.SetVec3("iCameraPosition", workspaceCamera->transform.GetPosition());       
            ShaderDirectionalLights(shader);
            ShaderPointLights(shader);     
        }

        /// TODO: Migrate to transform
        mat4 GetCoordinateSystem(Entity* entCoord) {
            if (entCoord == nullptr)
                return mat4();
            mat4 result = entCoord->transform.GetTransform();
            while (true) {
                if (entCoord->GetParent() != nullptr)
                    break;
                entCoord = entCoord->GetParent();
                result = entCoord->transform.GetTransform() * result;
            }
            return result;
        }

        void Draw(MeshRenderer& renderer, bool wireframe=false) {
            ObjId meshId = renderer.GetMeshId();
            if (meshId != 0) {
                Mesh* mesh = resourceMeshes.Get(meshId);
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

        void RenderSelected(Entity* entity, Shader* shader=NULL, bool wireframe=false) {
            MeshRenderer* renderer = entity->GetComponent<MeshRenderer>();
            if (renderer == nullptr || renderer->GetMeshId() == 0)
                return;
            mat4 model = GetCoordinateSystem(entity);
            shader->SetMat4("iModel", &model[0][0]);
            Draw(*renderer);
        }

        void RenderObject(Entity* entity, mat4 root_trans = mat4(), Shader* shader=NULL, bool wireframe=false) {
            mat4 model = root_trans *  entity->transform.GetTransform();
            for (int i = 0; i < entity->GetNumChildren(); i++) {
                RenderObject(entity->GetChild(i), model, shader, wireframe);
            }

            // ensure mesh not empty
            MeshRenderer* renderer = entity->GetComponent<MeshRenderer>();
            if (renderer == nullptr || renderer->GetMeshId() == 0)
                return; 
            shader->SetMat4("iModel", &model[0][0]);
            Draw(*renderer, wireframe);
        }

        void Render() {
            frame->Bind();
            frame->Clear();
            
            GL_Interface::SetViewport(width, height);
            GL_Interface::SetClearColour(0.2f, 0.2f, 0.2f, 1.0f);

            for (auto& shaderPair : resourceShaders) {
                SetupShader(*shaderPair.second);
            }

            Shader* baseShader = resourceShaders.Get("base");
            Shader* lightingShader = resourceShaders.Get("lighting");

            camera->SetResolution(width, height);
            fpc->Update();

            // draw lit
            GL_Interface::EnableFeature(FEATURE_DEPTH);
            GL_Interface::EnableFeature(FEATURE_CULL);
            GL_Interface::SetFrontFace(FRONT_CCW);
            lightingShader->Use();
            RenderObject(rootEntity, mat4(), lightingShader, false);

            // draw wireframe
            GL_Interface::EnableFeature(FEATURE_DEPTH);
            GL_Interface::EnableFeature(FEATURE_CULL);
            baseShader->Use();
            RenderObject(rootEntity, mat4(), baseShader, true);

            // draw selected
            // baseShader->Use();
            // RenderSelected()

            frame->Unbind();
            
            ft.Frame();
        }

};