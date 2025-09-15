# Marathon Engine - Sam Pearson-Smith
### *Powered By: C++ | SDL2 | OpenGL*

## License
This project is licensed under the [Apache-2.0](https://www.apache.org/licenses/LICENSE-2.0).

## Requirements
- CMake
- OpenGL 3.3+
``` bash
    sudo apt install cmake pkg-config gdb   # installs build tools
    sudo apt install mesa-utils libglu1-mesa-dev freeglut3-dev mesa-common-dev llvm  # installs opengl libs
    sudo apt install vulkan-tools libvulkan-dev    # install vulkan libs
    sudo apt install libglew-dev libsdl2-dev    # window/low-level access libs
    export VK_ICD_FILENAMES="" # disable vulkan cause its breaking stuff on WSL
```

Run compile script:
``` bash
./compile.sh clean
```

Or use CMake/Make directly:
``` bash
cmake -S . -B build
cmake --build build
```

Run executable:
``` bash
./build/graphics
```

## Example Engines used for Research:
- https://www.unrealengine.com/en-US/ue-on-github
- https://sbox.game/about

## Summary
A simple 2D/3D game library using a module based system inspired by pygame/love2d featuring:
- [ ] Rendering 3D models
- [ ] Coordinate space transformation stack
- [ ] Orthoganol and Perspective camera types
- [x] Custom shader support
- [x] Custom mesh support
- [ ] Default resources/objects/meshes/shaders
- [ ] Multi Threading
- [ ] Filesystem management
- [ ] Support ImGui layer
- [x] Allow seperate subsystem initialisation
- [x] Allow objects to be created before initialisation
- [ ] Custom shader auto binding layer
- [x] User managed resources
- [ ] Support manual calls and base app extension


## Modules
The library consists of a set of fundamental modules that empower the user to create a wide variety off engine level applications:
- Window (SDL2)
- Renderer (OpenGL/Vulkan)
- Time
- Events (SDL2)
- Math (LA)

To-Do:
- Audio (SDL2)
- Input (SDL2)
- Filesystem

## Engine vs Library vs App
**Library: Core**
- Provides core data types, common helpers, logging, maths, config
- Various dependencies to each other
- May wrap third party libs
- Will be used across all other systems/modules/services

**Library: Services**
- Provide static interfaces to targeted/grouped functionality
- Services can depend on other services but should be minimised
- Services include: audio, rendering, window, input, events, physics
- Effectively each service is a static library of function calls with an internally managed state
- Each service should allow for thread safe usage by a driver/engine
- Each service should support backend impl switching at compile time
- Services can have internal classes to encapsulate functionality but these should not be exposed to the user. ID handlers should instead be given.
- Services should handle errors/failures by reporting to a logger but provide default functionality to fall back on.

**Engine: Resources**
- any class/object intended to hold data is considered a resources
- resource classes should not provide functionality applying the data but should provide a reasonable interface for modification
- resources should be easy to copy/duplicate and share across multiple nodes with clear distinction between the same instances vs same blueprints
- sets of standard resources should be implemented
- resources should make calls to appropriate library services
- resources should be managed with designated managers for each core resource base type
- scripts are considered a resource that utilise a base script class, scripts can only be created/defined before compile time

**Engine: Nodes**
- an instanced node should represent a specific game object/component in the game/scene tree
- each node class should be a blueprint for a specific type of game object
- node classes should link to resources using resource managers

**Engine: Engines**
- each engine should encapsulate the buisness logic of handling its corresponding services functionality using the scene tree data
- each engine should be thread safe
- each engine should handle frequent update class to progress the game/interactive
- a MainLoop class should be implemented that creates and operates the engines to update positions, call renders, calculate physics, play audio etc.
- different main loop's should be implemented for different desired circumstances:
    - Standard Engine for a typical game loop
    - Headless main loop for no rendering/window
    - Multiplayer loop?
- an app class should be created that the user can inherit and configure some high level global data like app name, multi threaded, choose main loop etc. and provide a run method to start the app.

**App: User App**
- the user's app should inherit from the engine and override provided virtual methods to setup the scene tree, instance nodes, create resources, custom nodes/resources, add scripts
- the scripts added to nodes should encapsulate all per frame/per physics tick functionality

## Target Applications:
- Simple collision simulation spheres, AABBs, boxes
- Live FPS controller
- Environment scene layout (with serialization)
- Standard asset model loader/viewer (with textures & lighting)

**ImGui Integrated:**
- Editor using a standard set of types & ptrs & base classes to auto configure gui elements to represent custom user classes
- Live shader editor
- Scene layout
- Sound player
- Skeleton animation display
- Live global illumination/shadows (directional, spot, point)


## Future Feature List
- Multiple cameras, quick switch abilities
- Camera preview
- 2D/3D Physics
- Entity component system
- Scene tree
- Custom canvases to draw to with variable attachments
- Baked in runtime layer
- Asset loading
- Automatic game state management
- Automatic serialization
- Scripting engine inside C++ Game Engine (Lua/Squirrel) OR Python Game Engine + Scripting
- Real Time lighting:
    - do directional lights as orthoganol projections
    - (use cascade shadow maps for increasing scale i.e. like mipmaps but calculating the shadows over increasingly large areas as the resolution gets worse covering larger areas, they are further from the main rendering camera so its not as noticable)
    - spot lights as projections to 2d texture
    - point lights as projection over 6 directions to a cubemap
    - fixed lighting limit PER MESH e.g. 8 lights of each type can affect the mesh (shader limitations)
    - baked lighting to support higher lighting maximums?
- Physics
    - custom sphere/AABB/box
    - PhysX



### Tasks

### High Level Operation
- [x] Create Module for main abstract modules to inherit
- [x] Implement each module to boot & shutdown
- [x] Implement each Interactive (game/simulation/editor) to start, update, stop
- [ ] Allow interactive to be chosen at runtime

### Engine/Editor Abstraction
- [ ] Editor should require project to open
- [x] Editor should be extracted from engine as seperate layer
- [ ] Implement operator that runs a given project (with game engine ticks and seperate gui ticks)

### Rendering
- [x] Implement OpenGL rasterization
- [x] Create graphics layer interface to abstract OpenGL implementation
- [x] Support lighting
- [x] Implement Point Lights
- [x] Implement Directional Lights
- [x] Implement Spot Lights
- [x] Support materials
- [ ] Support textures
- [x] Wireframe rendering
- [x] Impl shader loader as asset_loader
- [x] Create renderer interface
- [x] Use opengl calls directly in OpenGL rasterizer
- [x] Remove gl_interface

### Camera
- [x] Support cameras
- [x] Support camera movement
- [ ] Render all cameras in scene
- [ ] Show camera preview
- [ ] Show camera depth preview
- [ ] Show camera stencil preview

### Asset Manager
- [x] Implement asset manager
- [x] Use smart pointers
- [x] Implement generic, centralise all assets
- [x] Store each asset type in its own array
- [ ] Modify asset manager to store generic types

### Asset Loaders
- [ ] Support project loading and saving
- [x] Support scene loading and saving
- [x] Support json files
- [ ] Support audio files
- [ ] Support image files
- [ ] Support text files
- [x] Support shader files
- [x] Support 3D model files
- [ ] Support animation files

### Editor Camera
- [x] Implement editor camera
- [ ] Inherit from general camera

### Shaders
- [ ] Live shader editing

### Scene
- [ ] Implement scene manager
- [ ] Allow multiple scene preloaded
- [ ] Support dynamic scene switching
- [x] Load/save scene to file
- [x] Handle entity management

### Serialization
- [x] Serialize entities
- [x] Serialize scene
- [ ] Serialize assets
- [ ] Serialize project settings

### GUI
- [x] Implement ImGui
- [x] Support dynamic gui loading with interfaces
- [ ] Create resource preview
- [ ] Create asset manager
- [ ] Create resource viewer(s)
- [ ] Remove Imguizmo
Note: ImGui is immediate mode and simple to use because it doesn't require tons of state systems on the backend.
This means you should handle it, not that you should avoid having state with components/sub-components of the gui.

### Scripting
- [ ] Create scripting engine
- [ ] Support start and update functions
- [ ] Create gizmo rendering
- [ ] Implement python scripting support
- [ ] Implement Lua scripting support

### OS Support
- [x] Support Linux
- [ ] Support Windows

### Experimental
- [ ] Implement raycasting/raytracing rendering
- [ ] Implement radiosity lighting




