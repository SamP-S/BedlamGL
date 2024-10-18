#pragma once

namespace marathon {

//// TODO:
// move module instances here

class Interactive {
public:

    virtual void Start() = 0;
    virtual void Update(double dt) = 0;
    virtual void End() = 0;
};

} // marathon
