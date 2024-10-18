#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <cassert>

// include core modules
#include "core/module.hpp"
#include "core/tick_timer.hpp"
#include "core/uuid.hpp"
#include "core/asset.hpp"

#include "window/window.hpp"
#include "renderer/renderer.hpp"
#include "input/input.hpp" 
// add audio


namespace marathon {

    bool Boot();
    bool Shutdown();

} // marathon
