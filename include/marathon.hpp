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
#include "events/events.hpp"
#include "time/time.hpp"
// add audio
// add input


namespace marathon {

    bool Init();
    bool Quit();

} // marathon
