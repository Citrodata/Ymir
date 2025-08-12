#pragma once

#include <app/shared_context.hpp>

#include <span>

namespace app::ui::widgets {

struct StereoSample {
    float left, right;
};

// Draws a monaural oscilloscope with the specified dimensions.
// The waveform data will be clamped to -1.0..+1.0.
void Oscilloscope(SharedContext &ctx, std::span<const float> waveform, ImVec2 size = {0, 0});

// Draws a stereo oscilloscope with the specified dimensions.
// The waveform data will be clamped to -1.0..+1.0.
void Oscilloscope(SharedContext &ctx, std::span<const StereoSample> waveform, ImVec2 size = {0, 0});

} // namespace app::ui::widgets
