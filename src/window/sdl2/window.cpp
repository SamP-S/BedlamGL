#include "window/sdl2/window.hpp"

namespace marathon {

namespace window {

namespace sdl2 {

Window::Window()
    : marathon::window::Window("marathon.window.sdl2"),
    _isOpen(false) {}
Window::~Window() {}

// setup context
void Window::Boot() {

    // Initialise SDL subsystems
    SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO);

    // OpenGL version
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    // Set max depth & stencil buffer size
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // msaa
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    
    _window = SDL_CreateWindow(
        "Example Title",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        _windowConfig.width, _windowConfig.height, 
        window_flags);
    _openglContext = SDL_GL_CreateContext(_window);
    _isOpen = true;

    SDL_GL_MakeCurrent(_window, _openglContext);
    SDL_GL_SetSwapInterval(1);
    
    SetWindowMinSize(_windowConfig.minWidth, _windowConfig.minHeight);
    SetWindowSize(_windowConfig.width, _windowConfig.height);

    // Must be done before any opengl call but after opengl context created
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Window.cpp: Failed to initialize GLEW" << std::endl;
        std::cerr << "GLEW init failed: " << glewGetErrorString(err) << std::endl;
    }
}

// destroy context
void Window::Shutdown() {
    Close();
    SDL_Quit();
}


void* Window::GetGLContext() {
    return _openglContext;
}
SDL_Window* Window::GetWindow() {
    return _window;
}

// call at the end of the frame to swap displayed buffer
void Window::SwapFrame() {
    SDL_GL_SwapWindow(_window);
}

bool Window::IsOpen() {
    return _isOpen;
}

bool Window::Close() {
    if (_openglContext) {
        SDL_GL_DeleteContext(_openglContext);
        _openglContext = nullptr;
    }
    if (_window) {
        SDL_DestroyWindow(_window);
        _window = nullptr;

        // remove any outstanding window events as window gone now
        SDL_FlushEvent(SDL_WINDOWEVENT);
    }
    _isOpen = false;
}

void Window::SetWindowMinSize(int minWidth, int minHeight) {
    SDL_SetWindowMinimumSize(_window, minWidth, minHeight);
}
void Window::GetWindowMinSize(int& minWidth, int& minHeight) {
    SDL_GetWindowMinimumSize(_window, &minWidth, &minHeight);
}

void Window::SetWindowSize(int width, int height) {
    SDL_SetWindowSize(_window, width, height);
}
void Window::GetWindowSize(int& width, int& height) {
    SDL_GetWindowSize(_window, &width, &height);
}

void Window::SetCursorCapture(bool capture) {
    SDL_SetRelativeMouseMode((SDL_bool)capture);
}
bool Window::GetCursorCapture() {
    return (bool)SDL_GetRelativeMouseMode();
}

} // sdl2

} // window

} // marathon