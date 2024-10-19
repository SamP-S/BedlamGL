#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <functional>
#include <iostream>

#include "core/module.hpp"

namespace marathon {

namespace window {

struct WindowConfig {
    std::string title = "Untitled Window";
    int minWidth = 800;
    int minHeight = 600;
    int width = 1280;
    int height = 720;
    int resizable = 1;
    int borderless = 0;
    int centred = 1;
    int fullscreen = 0;
};

class Window : public marathon::Module {
protected:
    Window(const std::string& name);
    
public:
    virtual ~Window();

    static Window& Instance();
    
    // module interface
    // void Boot() override;
    // void Shutdown() override;

    // common
    virtual void SwapFrame();
    
    // properties
    virtual void SetWindowMinSize(int minWidth, int minHeight);
    virtual void GetWindowMinSize(int& minWidth, int& minHeight);
    virtual void SetWindowSize(int width, int height);
    virtual void GetWindowSize(int& width, int& height);
    virtual void SetCursorCapture(bool capture);
    virtual bool GetCursorCapture();
    virtual bool IsOpen();
    virtual bool Close();
};

} // window

} // marathon