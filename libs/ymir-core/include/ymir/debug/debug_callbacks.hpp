#pragma once

/**
@file
@brief Debugger callbacks.
*/

#include "debug_break_info.hpp"

#include <ymir/util/callback.hpp>

namespace ymir::debug {

/// @brief Invoked when a debug break signal is raised.
using CBDebugBreakRaised = util::OptionalCallback<void(const DebugBreakInfo &info)>;

} // namespace ymir::debug
