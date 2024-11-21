#include "core/node.hpp"

namespace marathon {

Node::Node(const std::string& type) {
    _mType = type;
}

Node::~Node() {}

std::string Node::GetName() const {
    return _mName;
}

void Node::SetName(const std::string& name) {
    _mName = name;
}

std::string Node::GetType() const {
    return _mType;
}


}