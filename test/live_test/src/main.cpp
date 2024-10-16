#include <iostream>

#include "runtime/application.hpp"
#include "runtime.hpp"

int main(int argc, char *argv[]) {
    // instance main application
    std::cout << "start of program" << std::endl;

    // instance app cfg
    ApplicationConfig appCfg;
    appCfg.name = "live_test";
    appCfg.cwd = "~/source/repos/hobby/SceneGL/sample_project";
    // write more application pre-launch configuration
    // appCfg.####

    // create app & run
    Application* app = Application::Create(appCfg);
    app->SetInteractive(new Runtime());
    app->Run();
    delete app;
    
    std::cout << "end of program" << std::endl;
    return 0;
}
