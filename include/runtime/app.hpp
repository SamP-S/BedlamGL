#pragma once

namespace marathon {
    
class App {
private:
    bool _mQuit = false;   

protected:
    App();
    ~App();

    void Boot();
    void Shutdown();

public:
    // main methods for user to use
    void Run();
    void Quit();

    // virtual methods for user to implement
    virtual void Start() = 0;
    virtual void Update(double delta_time) = 0;
    
};

} // namespace marathon
