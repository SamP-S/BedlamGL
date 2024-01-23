#define GL_VERSION_4_4
#include <GL/glew.h>
#include <SDL.h>
#include "imgui.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <tinyfiledialogs.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#include "ngine/ngine.hpp"
using namespace Ngine;

#include "renderer/context_manager.hpp"
#include "renderer/components.hpp"
#include "renderer/graphics.hpp"
#include "input/input.hpp"

#include "la_extended.h"
using namespace LA;

#include "renderer/editor_camera.hpp"

class Editor {
    private:
        // OpenGL properties
        OpenGLConfig gl_cfg;

        // SDL
        ContextManager contextManager  = ContextManager();
        bool isQuit = false;

        // Engines
        GraphicsEngine Graphics = GraphicsEngine(gl_cfg.width, gl_cfg.height);

        // Application state
        bool show_editor_window = true;
        bool show_render_window = true;
        bool show_stats_window = true;
        bool show_world_window = true;
        bool show_properties_window = true;
        bool show_demo_window = false;
        float aspectRatio = 0.0f;
        bool renderer_focused = false;
        ImVec2 render_region_min = ImVec2();
        ImVec2 render_region_max = ImVec2();
        ImVec2 window_pos = ImVec2();
        Entity entitySelected;
        int new_entity_count = 0;
        int componentPanelCount = 0;

        AssetManager&  assetManager = AssetManager::Instance();

        std::map<char*, float> arMap = {
            {"None", 0.0f},
            {"16:9", 16.0f / 9.0f},
            {"5:4", 5.0f / 4.0f},
            {"4:3", 4.0f / 3.0f},
            {"1:1", 1.0f}
        };

    public:

        Editor() {
            // SDL
            contextManager.Initialise(gl_cfg);
            contextManager.AddEventHandler([this](SDL_Event& event) { EventHandler(event); });
            
            Initialise();
            Graphics.Initialise();

            // Main loop
            while (!isQuit)
            {
                contextManager.HandleEvents();

                // Start the Dear ImGui frame
                ImGui_ImplOpenGL3_NewFrame();
                ImGui_ImplSDL2_NewFrame();
                ImGui::NewFrame();

                ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
                const ImGuiViewport* viewport = ImGui::GetMainViewport();

                ImGui::SetNextWindowPos(viewport->WorkPos);
                ImGui::SetNextWindowSize(viewport->WorkSize);
                ImGui::SetNextWindowViewport(viewport->ID);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
                window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
                window_flags |= ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
                window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
                ImGui::Begin("CoreWindow", NULL, window_flags);
                window_pos = ImGui::GetWindowPos();
                ImGui::PopStyleVar(3);

                ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
                ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");
                ImVec2 dockspace_size = ImVec2(0, 0);
                ImGui::DockSpace(dockspace_id, dockspace_size, dockspace_flags);

                MenuBar();
                ImGui::End();

                if (show_render_window)
                    RenderWindow();
                if (show_stats_window) 
                    StatisticsWindow();
                if (show_world_window) 
                    WorldWindow();
                if (show_properties_window) 
                    PropertiesWindow();
                if (show_demo_window) {
                    ImGui::ShowDemoWindow();
                }

                ImGui::Render();
                ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

                if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
                    contextManager.BackupContext();
                    ImGui::UpdatePlatformWindows();
                    ImGui::RenderPlatformWindowsDefault();
                    contextManager.RestoreContext();
                }
                contextManager.SwapFrame();
            }
            Shutdown();
            contextManager.Destroy();
            // Destroy_SDL2();
        }

        void EventHandler(SDL_Event& event) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT) {
                isQuit = true;
            }
                        
            if (renderer_focused) {
                switch (event.type) {
                    case SDL_KEYUP:
                        Input::KeyEvent(event.key.keysym.scancode, KEY_UP);
                        break;
                    case SDL_KEYDOWN:
                        Input::KeyEvent(event.key.keysym.scancode, KEY_DOWN);
                        break;
                    case SDL_MOUSEMOTION:
                        {
                            // std::cout << render_region_min.x << ":" << render_region_min.y << std::endl;
                            // std::cout << event.motion.x << "@" << event.motion.y << std::endl;
                            if (event.motion.x >= render_region_min.x && event.motion.y >= render_region_min.y
                                && event.motion.x <= render_region_max.x && event.motion.y <= render_region_max.y) {
                                int x = event.motion.x - render_region_min.x;
                                int y = event.motion.y - render_region_min.y;
                                x = ((x >= 0) ? x : 0);
                                y = ((y >= 0) ? y : 0);
                                x = ((x <= render_region_max.x) ? x : render_region_max.x);
                                y = ((y <= render_region_max.y) ? y : render_region_max.y);
                                Input::MouseMoved(x, y);
                            }
                        }
                        break;
                    case SDL_MOUSEBUTTONDOWN:
                        Input::MouseButtonEvent(event.button.button, BUTTON_DOWN);
                        break;
                    case SDL_MOUSEBUTTONUP:
                        Input::MouseButtonEvent(event.button.button, BUTTON_UP);
                        break;
                }
            } else {
                Input::ClearStates();
            }
        }
        
        void Initialise() {
            // Setup Dear ImGui context
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO();
            io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
            io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
            //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

            // Setup Dear ImGui style
            ImGui::StyleColorsDark();
            // ImGui::StyleColorsClassic();

            // Setup Platform/Renderer backends
            ImGui_ImplSDL2_InitForOpenGL(contextManager.window, contextManager.gl_context);
            ImGui_ImplOpenGL3_Init(gl_cfg.glsl);
        }

        void Shutdown() {
            // Cleanup
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplSDL2_Shutdown();
            ImGui::DestroyContext();
        }

        ImVec2 AspectRatioLock(const ImVec2 maxSize, float aspectRatio) {
            float maxAspectRatio = maxSize.x / maxSize.y;
            ImVec2 wSize = maxSize;
            if (aspectRatio != 0) {
                if (aspectRatio >= maxAspectRatio)
                    wSize.y = wSize.x / aspectRatio;
                else if (aspectRatio < maxAspectRatio)
                    wSize.x = wSize.y * aspectRatio;
            }
            return wSize;
        }

        void MenuBar() {
            if (ImGui::BeginMenuBar()) {
                if (ImGui::BeginMenu("File")) {
                    if (ImGui::MenuItem("New")) {
                        Graphics.LoadScene("scene/Default.json");
                    }
                    if (ImGui::MenuItem("Open")) {
                        const char* filepath = tinyfd_openFileDialog("Open Scene", "./scene/Preset.json", 0, NULL, NULL, 0);
                        if (filepath == nullptr) {
                            std::cout << "DEBUG (App): No file selected." << std::endl;
                        } else {
                            Graphics.LoadScene(filepath);
                        }
                        
                    }
                    if (ImGui::MenuItem("Save As")) {
                        const char* filepath = tinyfd_saveFileDialog("Save Scene", "./scene/Test.json", 0, NULL, NULL);
                        if (filepath == nullptr) {
                            std::cout << "DEBUG (App): No file selected." << std::endl;
                        } else {
                            Graphics.SaveScene(filepath);
                        }
                    }
                    ImGui::Separator();
                    if (ImGui::MenuItem("Close")) {
                        isQuit = true;
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Entity")) {
                    if (ImGui::MenuItem("New Empty")) {
                        entitySelected = Graphics.scene->CreateEntity();
                    } else if (ImGui::MenuItem("New Cube")) {
                        entitySelected = Graphics.scene->CreateEntity();
                        MeshRendererComponent& mrc = entitySelected.AddComponent<MeshRendererComponent>();
                        mrc.mesh = assetManager.FindAsset<OpenGLMesh>("vertex_cube");
                    } else if (ImGui::MenuItem("New Camera")) {
                        entitySelected = Graphics.scene->CreateEntity();
                        entitySelected.AddComponent<CameraComponent>();
                    } else if (ImGui::MenuItem("New Directional Light")) {
                        entitySelected = Graphics.scene->CreateEntity();
                        entitySelected.AddComponent<DirectionalLightComponent>();
                    } else if (ImGui::MenuItem("New Point Light")) {
                        entitySelected = Graphics.scene->CreateEntity();
                        entitySelected.AddComponent<PointLightComponent>();
                    } else if (ImGui::MenuItem("New Spot Light")) {
                        entitySelected = Graphics.scene->CreateEntity();
                        entitySelected.AddComponent<SpotLightComponent>();
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Window")) {
                    ImGui::MenuItem("Render Display", NULL, &show_render_window);
                    ImGui::MenuItem("Stats/Performance", NULL, &show_stats_window);
                    ImGui::MenuItem("World Tree", NULL, &show_world_window);
                    ImGui::MenuItem("Demo Window", NULL, &show_demo_window);
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Aspect Ratio")) {
                    for (auto const& ar : arMap) {
                        if (ImGui::MenuItem(ar.first)) {
                            aspectRatio = ar.second;
                        }
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }
        }

        void RenderWindow() {
            ImGuiWindowFlags renderWindowFlags = ImGuiWindowFlags_None;
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("Render Window", &show_render_window, renderWindowFlags);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::PopStyleVar(1);
            renderer_focused = ImGui::IsWindowFocused();

            // content size
            ImVec2 pos = ImGui::GetWindowPos();
            pos.x -= window_pos.x;
            pos.y -= window_pos.y;
            // std::cout << "wpos= " << window_pos.x << ":" << window_pos.y << std::endl;
            // std::cout << "pos= " << pos.x << ":" << pos.y << std::endl;
            render_region_min = ImGui::GetWindowContentRegionMin();
            // std::cout << "min= " << render_region_min.x << ":" << render_region_min.y << std::endl;
            render_region_min.x += pos.x;
            render_region_min.y += pos.y;
            // std::cout << "min+= " << render_region_min.x << ":" << render_region_min.y << std::endl;
            render_region_max = ImGui::GetWindowContentRegionMax();
            render_region_max.x += pos.x;
            render_region_max.y += pos.y;
            
            // ImVec2 res = (ImGui::GetWindowContentRegionMax() - ImGui::GetWindowContentRegionMin()) + ImGui::GetWindowPos() - window_pos;
            float wWidth = render_region_max.x - render_region_min.x;
            float wHeight = render_region_max.y - render_region_min.y;
            ImVec2 wSize = AspectRatioLock(ImVec2(wWidth, wHeight), aspectRatio);
            Graphics.Render();
            ImGui::Image((ImTextureID)Graphics.frameBuffer->GetColourAttachment(), wSize, ImVec2(0, 1), ImVec2(1, 0));
            // ImGui::GetForegroundDrawList()->AddRect(render_region_min + pos, render_region_max + pos, IM_COL32(255, 255, 0, 255));
            ImGui::End();
        }

        void StatisticsWindow() {
            ImGuiWindowFlags statsWindowFlags = ImGuiWindowFlags_None;
            ImGui::Begin("Statistics Monitor", &show_stats_window, statsWindowFlags);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // flattened until scene tree implemented
        void WorldNode(Entity entitiy) {
            ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;
            if (entitySelected == entitiy)
                node_flags |= ImGuiTreeNodeFlags_Selected;
            node_flags |= ImGuiTreeNodeFlags_Leaf;
            bool node_open = ImGui::TreeNodeEx(entitiy.GetComponent<CoreComponent>().name.c_str(), node_flags);
            if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
                entitySelected = entitiy;
            if (node_open) {
                ImGui::TreePop();
            }   
        }
        
        void WorldWindow() {
            ImGuiWindowFlags worldWindowFlags = ImGuiWindowFlags_None;
            ImGui::Begin("World Tree", &show_world_window, worldWindowFlags);
            if (Graphics.scene != nullptr) {
                std::vector<Entity> entities = Graphics.scene->GetEntities();
                for (auto e : entities) {
                    WorldNode(e);
                }
            }
            ImGui::End();
        }

        void CorePanel(Entity e) {
            if (!e.HasComponent<CoreComponent>()) {
                std::cout << "ERROR (App): Missing core component." << std::endl;
                return;
            }
            ImGui::PushID(componentPanelCount);
            CoreComponent& cc = e.GetComponent<CoreComponent>();
            // entity name
            ImGui::Text("Name:");
            ImGui::SameLine();
            char buffer[256];
            std::strncpy(buffer, cc.name.c_str(), sizeof(buffer));
            buffer[sizeof(buffer) - 1] = 0; // Ensure null termination
            if (ImGui::InputText("##NameEntity", buffer, sizeof(buffer))) {
                cc.name = std::string(buffer);
            }
            ImGui::Checkbox("Active", &cc.active);
            ImGui::PopID();
            componentPanelCount++;
        }

        void TransformPanel(Entity e) {
            if (!e.HasComponent<TransformComponent>()) {
                CoreComponent& cc = e.GetComponent<CoreComponent>();
                std::cout << "ERROR (App): Missing transform component on " << cc.name << std::endl;
                return;
            }
            ImGui::PushID(componentPanelCount);
            TransformComponent& t = e.GetComponent<TransformComponent>();
            ImGui::Text("Transform:");
            // position
            ImGui::Text("Pos");
            ImGui::SameLine();
            ImGui::InputFloat3("##PosInput", t.position.m);
            // rotation
            ImGui::Text("Rot");
            ImGui::SameLine();
            ImGui::InputFloat3("##RotInput", t.rotation.m);
            // scale
            ImGui::Text("Scl");
            ImGui::SameLine();
            ImGui::InputFloat3("##SclInput", t.scale.m);
            ImGui::PopID();
            componentPanelCount++;
        }

        void MeshRendererPanel(Entity e) {
            if (!e.HasComponent<MeshRendererComponent>())
                return;
            MeshRendererComponent& mrc = e.GetComponent<MeshRendererComponent>();

            ImGui::PushID(componentPanelCount);
            ImGui::Separator();
            ImGui::Text("Mesh Renderer");
            ImGui::SameLine();
            if (ImGui::Button("X")) {
                e.RemoveComponent<MeshRendererComponent>();
            }

            if (ImGui::BeginCombo("Select Mesh", ((mrc.mesh == nullptr) ? "None": mrc.mesh->name.c_str()))) {
                if (ImGui::Selectable("None")) {
                    mrc.mesh = nullptr;
                }
                for (auto it = assetManager.begin<OpenGLMesh>(); it != assetManager.end<OpenGLMesh>(); ++it) {
                    std::shared_ptr<Mesh> mesh = std::dynamic_pointer_cast<Mesh>(*it);
                    if (ImGui::Selectable(mesh->name.c_str())) {
                        mrc.mesh = mesh;
                    }
                }
                ImGui::EndCombo();
            }

            if (ImGui::BeginCombo("Select Material", ((mrc.material == nullptr) ? "None": mrc.material->name.c_str()))) {
                if (ImGui::Selectable("None")) {
                    mrc.material = nullptr;
                } 
                for (auto it = assetManager.begin<OpenGLMaterial>(); it != assetManager.end<OpenGLMaterial>(); ++it) {
                    std::shared_ptr<Material> material = std::dynamic_pointer_cast<Material>(*it);
                    if (ImGui::Selectable(material->name.c_str())) {
                        mrc.material = material;
                    }
                }
                ImGui::EndCombo();
            }

            ImGui::PopID();
            componentPanelCount++;
        }

        void CameraPanel(Entity e) {
            if (!e.HasComponent<CameraComponent>())
                return;
            CameraComponent& cc = e.GetComponent<CameraComponent>();

            ImGui::PushID(componentPanelCount);
            ImGui::Separator();
            ImGui::Text("Camera");
            ImGui::SameLine();
            if (ImGui::Button("X")) {
                e.RemoveComponent<CameraComponent>();
            }
            ImGui::SliderFloat("FOV", &cc.fov, 45.0f, 90.0f, "%.1f", ImGuiSliderFlags_NoRoundToFormat);
            ImGui::PopID();
            componentPanelCount++;
        }
        
        void DirectionalLightWindow(Entity e) {
            if (!e.HasComponent<DirectionalLightComponent>())
                return;
            DirectionalLightComponent& dlc = e.GetComponent<DirectionalLightComponent>();

            ImGui::PushID(componentPanelCount);
            ImGui::Separator();
            ImGui::Text("Directional Light");
            ImGui::SameLine();
            if (ImGui::Button("X")) {
                e.RemoveComponent<DirectionalLightComponent>();
            }
            ImGui::InputFloat3("Colour", dlc.colour.m);
            ImGui::SliderFloat("Intensity", &dlc.intensity, 0.0f, 5.0f, "%.2f", ImGuiSliderFlags_NoRoundToFormat);
            ImGui::PopID();
            componentPanelCount++;
        }

        void PointLightWindow(Entity e) {
            if (!e.HasComponent<PointLightComponent>())
                return;
            PointLightComponent& plc = e.GetComponent<PointLightComponent>();

            ImGui::PushID(componentPanelCount);
            ImGui::Separator();
            ImGui::Text("Point Light");
            ImGui::SameLine();
            if (ImGui::Button("X")) {
                e.RemoveComponent<PointLightComponent>();
            }
            ImGui::InputFloat3("Colour", plc.colour.m);
            ImGui::SliderFloat("Intensity", &plc.intensity, 0.0f, 5.0f, "%.2f", ImGuiSliderFlags_NoRoundToFormat);
            ImGui::SliderFloat("Range", &plc.range, 0.01f, 20.0f, "%.2f", ImGuiSliderFlags_NoRoundToFormat);

            ImGui::PopID();
            componentPanelCount++;
        }

        void SpotLightWindow(Entity e) {
            if (!e.HasComponent<SpotLightComponent>())
                return;
            SpotLightComponent& slc = e.GetComponent<SpotLightComponent>();

            ImGui::PushID(componentPanelCount);
            ImGui::Separator();
            ImGui::Text("Spot Light");
            ImGui::SameLine();
            if (ImGui::Button("X")) {
                e.RemoveComponent<SpotLightComponent>();
            }
            ImGui::InputFloat3("Colour", slc.colour.m);
            ImGui::SliderFloat("Intensity", &slc.intensity, 0.0f, 5.0f, "%.2f", ImGuiSliderFlags_NoRoundToFormat);
            ImGui::SliderFloat("Cut Off", &slc.cutOff, 5.0f, 15.0f, "%.2f", ImGuiSliderFlags_NoRoundToFormat);
            ImGui::SliderFloat("Outer Cut Off", &slc.outerCutOff, 5.0f, 90.0f, "%.2f", ImGuiSliderFlags_NoRoundToFormat);

            ImGui::PopID();
            componentPanelCount++;
        }

        void PropertiesWindow() {
            ImGuiWindowFlags worldWindowFlags = ImGuiWindowFlags_None;
            ImGui::Begin("Entity Properties", &show_world_window, worldWindowFlags);
            if (!entitySelected) {
                ImGui::Text("Nothing selected");
            } else {
                Entity ent = entitySelected;
                // components display
                componentPanelCount = 0;
                if (ImGui::Button("Delete Entity")) {
                    Graphics.scene->DestroyEntity(ent);
                    entitySelected = Entity();
                } else {
                    // name/enable
                    CorePanel(ent);
                    // transform display
                    TransformPanel(ent);
                    // non-singlton components
                    MeshRendererPanel(ent);
                    CameraPanel(ent);
                    DirectionalLightWindow(ent);
                    PointLightWindow(ent);
                    SpotLightWindow(ent);
                    
                    // add component button/drop-down
                    if (ImGui::Button("Add Component")) {
                        ImGui::OpenPopup("Add Component");
                    }
                    if (ImGui::BeginPopup("Add Component")) {
                        if (ImGui::MenuItem("Camera")) {
                            ent.AddComponent<CameraComponent>();
                        } else if (ImGui::MenuItem("Directional Light")) {
                            ent.AddComponent<DirectionalLightComponent>();
                        } else if (ImGui::MenuItem("Point Light")) {
                            ent.AddComponent<PointLightComponent>();
                        } else if (ImGui::MenuItem("Spot Light")) {
                            ent.AddComponent<SpotLightComponent>();
                        } else if (ImGui::MenuItem("Mesh Renderer")) {
                            ent.AddComponent<MeshRendererComponent>();
                        }
                        ImGui::EndPopup();
                    }
                }
            }
            ImGui::End();
        }

};