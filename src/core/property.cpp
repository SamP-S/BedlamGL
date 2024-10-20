#include "core/property.hpp"

namespace marathon {

// property constructors handle all core numerical data types
Property::Property(const int data)
    : type(PropertyType::INT), data({.i = data}) {}
Property::Property(const uint data)
    : type(PropertyType::UINT), data({.ui = data}) {}
Property::Property(const float data)
    : type(PropertyType::FLOAT), data({.f = data}) {}
Property::Property(const double data)
    : type(PropertyType::DOUBLE), data({.d = data}) {}
Property::Property(const bool data)
    : type(PropertyType::BOOL), data({.b = data}) {}

} // marathon
