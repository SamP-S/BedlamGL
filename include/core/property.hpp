#pragma once

#include <string>

#include "la_extended.h"

/// TODO:
// OPTIMIZATION - add cstdint for fixed size data to reduce memory usage

namespace marathon {

enum class PropertyType {
    INT,
    UINT,
    FLOAT,
    DOUBLE,
    BOOL,
    MAX_ENUM
};

// funky data type to hold any singular piece of data
class Property {
public:
    union Data{
        int i;
        uint ui;
        float f;
        double d;
        bool b;
    };

    Property(const Property& other);
    Property& operator=(const Property& other);

    Property(const int data);
    Property(const uint data);
    Property(const float data);
    Property(const double data);
    Property(const bool data);

    const PropertyType type;
    const Data data;
};

} // marathon
