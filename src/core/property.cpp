#include "core/property.hpp"

namespace marathon {

// property allows for copying
Property::Property(const Property& other)
    : type(other.type), data(other.data) {}

Property& Property::operator=(const Property& other) {
    if (this != &other) {
        new (this) Property(other); // create new object using other as type/data is const
    }
    return *this;
}

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
