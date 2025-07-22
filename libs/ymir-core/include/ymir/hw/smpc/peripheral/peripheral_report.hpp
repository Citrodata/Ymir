#pragma once

#include "peripheral_defs.hpp"
#include "peripheral_state_common.hpp"

namespace ymir::peripheral {

/// @brief Control Pad report data.
struct ControlPadReport {
    Button buttons; ///< Button states (1=released, 0=pressed).
};

/// @brief 3D Control Pad report data.
struct AnalogPadReport {
    Button buttons; ///< Button states (1=released, 0=pressed).
    bool analog;    ///< Whether to use analog mode (`true`) or digital mode (`false`).
    uint8 x;        ///< Analog stick X coordinate (`0x00`=left, `0x80`=center, `0xFF`=right).
    uint8 y;        ///< Analog stick Y coordinate (`0x00`=top, `0x80`=center, `0xFF`=bottom).
    uint8 l;        ///< Left analog trigger value (`0x00`=fully released, `0xFF`=fully pressed).
    uint8 r;        ///< Right analog trigger value (`0x00`=fully released, `0xFF`=fully pressed).
};

/// @brief ArcadeRacer report data.
struct ArcadeRacerReport {
    Button buttons; ///< Button states (1=released, 0=pressed).
    uint8 wheel;    ///< Analog wheel value (`0x00`=left, `0x7F`=center/neutral, `0xFF`=right).
};

/// @brief Mission Stick report data.
struct MissionStickReport {
    Button buttons; ///< Button states (1=released, 0=pressed).
    bool sixAxis;   ///< Whether to use six-axis mode (`true`) or three-axis mode (`false`).
    uint8 x1;       ///< Main analog stick X coordinate (`0x00`=left, `0x7F`=center, `0xFF`=right).
    uint8 y1;       ///< Main analog stick Y coordinate (`0x00`=top, `0x7F`=center, `0xFF`=bottom).
    uint8 z1;       ///< Main analog throttle value (`0x00`=minimum/down, `0xFF`=maximum/up).
    uint8 x2;       ///< Sub analog stick X coordinate (`0x00`=left, `0x7F`=center, `0xFF`=right).
    uint8 y2;       ///< Sub analog stick Y coordinate (`0x00`=top, `0x7F`=center, `0xFF`=bottom).
    uint8 z2;       ///< Sub analog throttle value (`0x00`=minimum/down, `0xFF`=maximum/up).
};

/// @brief A report to be filled when a peripheral is read.
struct PeripheralReport {
    /// @brief The peripheral type being read.
    PeripheralType type;

    /// @brief The peripheral data report.
    union Report {
        /// @brief Control Pad report data.
        ///
        /// Valid when `type` is `PeripheralType::ControlPad`.
        ControlPadReport controlPad;

        /// @brief 3D Control Pad report data.
        ///
        /// Valid when `type` is `PeripheralType::AnalogPad`.
        AnalogPadReport analogPad;

        /// @brief Arcade Racer report data.
        ///
        /// Valid when `type` is `PeripheralType::ArcadeRacer`.
        ArcadeRacerReport arcadeRacer;

        /// @brief Mission Stick report data.
        ///
        /// Valid when `type` is `PeripheralType::MissionStick`.
        MissionStickReport missionStick;
    } report;
};

} // namespace ymir::peripheral
