#include <iostream>

#include "runtime/application.hpp"
#include "runtime.hpp"

int main(int argc, char *argv[]) {
    // instance main application
    std::cout << "start of program" << std::endl;

    // instance app cfg
    marathon::ApplicationConfig appCfg;
    appCfg.name = "Bedlam";
    appCfg.cwd = "~/source/Marathon/test/bedlam";
    appCfg.editor = "~/source/Marathon";
    // write more application pre-launch configuration
    // appCfg.####

    // create app & run
    marathon::Application* app = marathon::Application::Create(appCfg);
    app->SetInteractive(new marathon::Runtime());
    app->Run();
    delete app;
    
    std::cout << "end of program" << std::endl;
    return 0;
}
