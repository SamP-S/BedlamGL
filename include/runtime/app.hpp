#pragma once

namespace marathon {
    
class App {
private:
    bool _mQuit = false;   

protected:
    App();
    ~App();

    virtual void Boot();
    virtual void Shutdown();
    virtual void Update(double delta_time);

public:
    virtual void Run();
    virtual void Quit();
};

} // namespace marathon
