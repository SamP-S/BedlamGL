#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <cassert>

// include core modules
#include "core/module.hpp"
#include "core/uuid.hpp"
#include "core/property.hpp"
#include "core/resource.hpp"

#include "window/window.hpp"
#include "renderer/renderer.hpp"
#include "input/input.hpp"
#include "event/event.hpp"
#include "time/time.hpp"
// add audio


namespace marathon {

    bool Boot();
    bool Shutdown();

} // marathon
