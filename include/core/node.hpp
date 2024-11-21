#pragma once

#include <string>
#include <memory>
#include <iostream>

namespace marathon {

/// CONSIDER: is should a base class be used for all objects across project

// Node class as a base for all instances in a scene
class Node {
private:
    std::string _mName = "default_name";
    std::string _mType = "marathon.invalid";

public:
    Node(const std::string& type);
    ~Node();

    // name get/set
    std::string GetName() const;
    void SetName(const std::string& name);

    // type string get
    std::string GetType() const;
};

} // marathon