#pragma once

#include <ymir/core/types.hpp>
#include <ymir/util/inline.hpp>

#include <ymir/hw/vdp/vdp_defs.hpp>

#include <cmath>
#include <utility>

namespace ymir::vdp {

FORCE_INLINE auto SafeDiv(auto dividend, auto divisor) {
    return (divisor != 0) ? dividend / divisor : 0;
}

class Slope {
public:
    static constexpr sint64 kFracBits = 24;
    static constexpr sint64 kFracOne = 1ll << kFracBits;

    FORCE_INLINE static constexpr sint64 ToFrac(sint32 value) {
        return static_cast<sint64>(value) << kFracBits;
    }

    FORCE_INLINE static constexpr sint64 ToFracHalfBias(sint32 value) {
        return ((static_cast<sint64>(value) << 1) + 1) << (kFracBits - 1);
    }

    FORCE_INLINE Slope(CoordS32 coord1, CoordS32 coord2) {
        auto [x1, y1] = coord1;
        auto [x2, y2] = coord2;

        const sint32 dx = x2 - x1;
        const sint32 dy = y2 - y1;

        dmaj = std::max(abs(dx), abs(dy));

        xmajor = abs(dx) >= abs(dy);
        if (xmajor) {
            majinc = dx >= 0 ? kFracOne : -kFracOne;
            mininc = SafeDiv(ToFrac(dy), dmaj);
            majcounter = ToFrac(x1);
            majcounterend = ToFrac(x2) + majinc;
            mincounter = ToFracHalfBias(y1);
        } else {
            majinc = dy >= 0 ? kFracOne : -kFracOne;
            mininc = SafeDiv(ToFrac(dx), dmaj);
            majcounter = ToFrac(y1);
            majcounterend = ToFrac(y2) + majinc;
            mincounter = ToFracHalfBias(x1);
        }
    }

    // Clips the slope to the area 0x0..width x height.
    // Returns the number of increments skipped.
    FORCE_INLINE uint32 SystemClip(sint32 width, sint32 height) {
        // Add one pixel of padding to compensate for minor inaccuracies
        ++width;
        ++height;

        uint32 length = (majcounterend - majcounter) / majinc;

        // Bail out early if the line length is zero
        if (length == 0) {
            return 0u;
        }

        const sint64 xinc = xmajor ? majinc : mininc;
        const sint64 yinc = xmajor ? mininc : majinc;
        const sint64 fxs = FracX();
        const sint64 fys = FracY();
        const sint64 fxe = fxs + xinc * length;
        const sint64 fye = fys + yinc * length;
        const sint64 xs = fxs >> kFracBits;
        const sint64 ys = fys >> kFracBits;
        const sint64 xe = fxe >> kFracBits;
        const sint64 ye = fye >> kFracBits;

        static constexpr sint64 kPadding = 1;

        // Bail out early if the line is entirely in-bounds
        if (xs >= -kPadding && xs <= width && ys >= -kPadding && ys <= height && xe >= -kPadding && xe <= width &&
            ye >= -kPadding && ye <= height) {
            return 0u;
        }

        // Fully clip line if it is entirely out of bounds
        if ((xs < -kPadding && xe < -kPadding) || (xs > width && xe > width) || (ys < -kPadding && ye < -kPadding) ||
            (ys > height && ye > height)) {
            majcounterend = majcounter;
            return 0u;
        }

        // Determine how many pixels to clip from the start
        uint32 xclip;
        if (xinc > 0 && xs < -kPadding) {
            xclip = -xs - 1 - kPadding;
        } else if (xinc < 0 && xs > width) {
            xclip = xs - width;
        } else {
            xclip = 0;
        }
        uint32 yclip;
        if (yinc > 0 && ys < -kPadding) {
            yclip = -ys - 1 - kPadding;
        } else if (yinc < 0 && ys > height) {
            yclip = ys - height;
        } else {
            yclip = 0;
        }

        // Use the higher of the two counts to step the line ahead
        uint32 startClip = std::max(xclip, yclip);
        startClip = std::min(startClip, length - 1u);
        majcounter += majinc * startClip;
        mincounter += mininc * startClip;
        length -= startClip;

        // Determine how many pixels to clip from the end
        if (xinc < 0 && xe < -kPadding) {
            xclip = -xe - 1 - kPadding;
        } else if (xinc > 0 && xe > width) {
            xclip = xe - width;
        } else {
            xclip = 0;
        }
        if (yinc < 0 && ye < -kPadding) {
            yclip = -ye - 1 - kPadding;
        } else if (yinc > 0 && ye > height) {
            yclip = ye - height;
        } else {
            yclip = 0;
        }

        uint32 endClip = std::max(xclip, yclip);
        endClip = std::min(endClip, length - 1u);
        majcounterend -= majinc * endClip;

        return startClip;
    }

    // Steps the slope to the next coordinate.
    // Should not be invoked when CanStep() returns false
    FORCE_INLINE void Step() {
        majcounter += majinc;
        mincounter += mininc;
    }

    // Determines if the slope can be stepped
    FORCE_INLINE bool CanStep() const {
        return majcounter != majcounterend;
    }

    // Returns the current fractional position in the line, where 0.0 is the start point and 1.0 is the end point.
    FORCE_INLINE uint64 FracPos() const {
        return kFracOne - SafeDiv((majcounterend - majcounter) * (majinc >> kFracBits), dmaj + 1);
    }

    // Retrieves the current X coordinate (no fractional bits)
    FORCE_INLINE sint32 X() const {
        return (xmajor ? majcounter : mincounter) >> kFracBits;
    }

    // Retrieves the current Y coordinate (no fractional bits)
    FORCE_INLINE sint32 Y() const {
        return (xmajor ? mincounter : majcounter) >> kFracBits;
    }

    // Retrieves the current X and Y coordinates (no fractional bits)
    FORCE_INLINE CoordS32 Coord() const {
        return {X(), Y()};
    }

    // Retrieves the slope's longest span length
    FORCE_INLINE sint32 DMajor() const {
        return dmaj;
    }

protected:
    sint32 dmaj;   // major span of the slope: max(abs(dx), abs(dy))
    sint64 majinc; // fractional increment on the major axis (+1.0 or -1.0)
    sint64 mininc; // fractional increment on the minor axis

    bool xmajor; // true if abs(dx) >= abs(dy)

    sint64 majcounter;    // coordinate counter for the major axis (fractional, incremented by majinc per step)
    sint64 majcounterend; // final coordinate counter for the major axis
    sint64 mincounter;    // coordinate counter for the minor axis (fractional, incremented by mininc per step)

    // Retrieves the current fractional X coordinate
    FORCE_INLINE sint64 FracX() const {
        return xmajor ? majcounter : mincounter;
    }

    // Retrieves the current fractional Y coordinate
    FORCE_INLINE sint64 FracY() const {
        return xmajor ? mincounter : majcounter;
    }

    friend class QuadEdgesStepper;
};

// Steps over the pixels of a line.
class LineStepper : public Slope {
public:
    FORCE_INLINE LineStepper(CoordS32 coord1, CoordS32 coord2)
        : Slope(coord1, coord2) {
        auto [x1, y1] = coord1;
        auto [x2, y2] = coord2;

        const bool samesign = (x1 > x2) == (y1 > y2);
        if (xmajor) {
            aaxinc = samesign ? 0 : majinc;
            aayinc = samesign ? (y1 <= y2 ? kFracOne : -kFracOne) : 0;
        } else {
            aaxinc = samesign ? 0 : (x1 <= x2 ? kFracOne : -kFracOne);
            aayinc = samesign ? majinc : 0;
        }
    }

    // Determines if the current step needs antialiasing
    FORCE_INLINE bool NeedsAntiAliasing() const {
        // Antialiasing is needed when the coordinate on the minor axis has changed from the previous step
        return ((mincounter - mininc) >> kFracBits) != (mincounter >> kFracBits);
    }

    // Returns the X coordinate of the antialiased pixel
    FORCE_INLINE sint32 AAX() const {
        return (FracX() - aaxinc) >> kFracBits;
    }

    // Returns the Y coordinate of the antialiased pixel
    FORCE_INLINE sint32 AAY() const {
        return (FracY() - aayinc) >> kFracBits;
    }

    // Returns the X and Y coordinates of the antialiased pixel
    FORCE_INLINE CoordS32 AACoord() const {
        return {AAX(), AAY()};
    }

private:
    sint64 aaxinc; // X increment for antialiasing
    sint64 aayinc; // Y increment for antialiasing
};

// Edge iterator for a quad with vertices A-B-C-D arranged in clockwise order from top-left:
//
//    A-->B
//    ^   |
//    |   v
//    D<--C
//
// The stepper uses the edges A-D and B-C and steps over each pixel on the longer edge, advancing the position on the
// other edge proportional to their lengths.
class QuadEdgesStepper {
public:
    FORCE_INLINE QuadEdgesStepper(CoordS32 coordA, CoordS32 coordB, CoordS32 coordC, CoordS32 coordD)
        : slopeL(coordA, coordD)
        , slopeR(coordB, coordC) {

        swapped = slopeL.dmaj < slopeR.dmaj;

        Slope &majslope = MajSlope();
        Slope &minslope = MinSlope();

        mininc = SafeDiv(Slope::kFracOne * minslope.dmaj, majslope.dmaj);
        minstep = Slope::kFracOne >> 1ll;
    }

    // Steps both slopes of the edge to the next coordinate.
    // The major slope is stepped by a full pixel.
    // The minor slope is stepped in proportion to the major slope.
    // Should not be invoked when CanStep() returns false
    FORCE_INLINE void Step() {
        MajSlope().Step();

        // Step minor slope by a fraction proportional to minslope.dmaj / majslope.dmaj
        minstep += mininc;
        if (minstep >= Slope::kFracOne) {
            minstep -= Slope::kFracOne;
            MinSlope().Step();
        }
    }

    // Determines if the edge can be stepped
    FORCE_INLINE bool CanStep() const {
        return MajSlope().CanStep();
    }

    // Retrieves the current X coordinate of the left slope
    FORCE_INLINE sint32 LX() const {
        return slopeL.X();
    }

    // Retrieves the current Y coordinate of the left slope
    FORCE_INLINE sint32 LY() const {
        return slopeL.Y();
    }

    // Retrieves the current X coordinate of the right slope
    FORCE_INLINE sint32 RX() const {
        return slopeR.X();
    }

    // Retrieves the current Y coordinate of the right slope
    FORCE_INLINE sint32 RY() const {
        return slopeR.Y();
    }

    // Determines if the left and right edges have been swapped
    /*FORCE_INLINE bool Swapped() const {
        return swapped;
    }*/

    // Returns the current fractional position in the line, where 0.0 is the start point and 1.0 is the end point.
    FORCE_INLINE uint64 FracPos() const {
        return MajSlope().FracPos();
    }

protected:
    Slope slopeL; // left slope (A-D)
    Slope slopeR; // right slope (B-C)

    sint64 mininc;  // fractional minor slope interpolation increments
    sint64 minstep; // accumulated fractional step on minor slope

    bool swapped; // whether the original slopes have been swapped

    // Returns the slope with the longest span
    FORCE_INLINE Slope &MajSlope() {
        return swapped ? slopeR : slopeL;
    }

    // Returns the slope with the shortest span
    FORCE_INLINE Slope &MinSlope() {
        return swapped ? slopeL : slopeR;
    }

    // Returns the slope with the longest span
    FORCE_INLINE const Slope &MajSlope() const {
        return swapped ? slopeR : slopeL;
    }

    // Returns the slope with the shortest span
    FORCE_INLINE const Slope &MinSlope() const {
        return swapped ? slopeL : slopeR;
    }
};

// Steps over the pixels of a textured line, interpolating the texture's U coordinate based on the character width.
class TexturedLineStepper : public LineStepper {
public:
    TexturedLineStepper(CoordS32 coord1, CoordS32 coord2, uint32 charSizeH, bool flipU)
        : LineStepper(coord1, coord2) {
        uinc = SafeDiv(ToFrac(charSizeH), dmaj + 1);
        if (flipU) {
            uinc = -uinc;
        }
        ustart = flipU ? Slope::ToFrac(static_cast<uint64>(charSizeH)) - 1 : 0u;
        ustart += uinc >> 1;
        u = ustart;
    }

    // Clips the slope to the area 0x0..(width-1)x(height-1)
    FORCE_INLINE void SystemClip(uint32 width, uint32 height) {
        const uint32 steps = LineStepper::SystemClip(width, height);
        u += uinc * steps;
        ustart += uinc * steps;
    }

    // Steps the slope to the next coordinate.
    // The U coordinate is stepped in proportion to the horizontal character size
    // Should not be invoked when CanStep() returns false
    FORCE_INLINE void Step() {
        LineStepper::Step();
        u += uinc;
    }

    // Retrieves the current U texel coordinate.
    FORCE_INLINE uint32 U() const {
        return u >> kFracBits;
    }

    // Retrieves the current fractinal U texel coordinate.
    FORCE_INLINE uint64 FracU() const {
        return u;
    }

    uint64 ustart; // starting U texel coordinate, fractional
    uint64 u;      // current U texel coordinate, fractional
    sint64 uinc;   // U texel coordinate increment per step, fractional
};

// Edge iterator for a textured quad with vertices A-B-C-D arranged in clockwise order from top-left, interpolating the
// texture's V coordinate based on the character height.
class TexturedQuadEdgesStepper : public QuadEdgesStepper {
public:
    TexturedQuadEdgesStepper(CoordS32 coordA, CoordS32 coordB, CoordS32 coordC, CoordS32 coordD, uint32 charSizeV,
                             bool flipV)
        : QuadEdgesStepper(coordA, coordB, coordC, coordD) {
        vinc = SafeDiv(Slope::ToFrac(charSizeV), MajSlope().DMajor() + 1);
        if (flipV) {
            vinc = -vinc;
        }
        vstart = flipV ? Slope::ToFrac(static_cast<uint64>(charSizeV)) - 1 : 0u;
        vstart += vinc >> 1;
        v = vstart;
    }

    // Steps both slopes of the edge to the next coordinate.
    // The major slope is stepped by a full pixel.
    // The minor slope is stepped in proportion to the major slope.
    // The V coordinate is stepped in proportion to the vertical character size
    // Should not be invoked when CanStep() returns false
    FORCE_INLINE void Step() {
        QuadEdgesStepper::Step();
        v += vinc;
    }

    // Retrieves the current V texel coordinate.
    FORCE_INLINE uint32 V() const {
        return v >> Slope::kFracBits;
    }

    // Retrieves the current fractional V texel coordinate.
    FORCE_INLINE uint64 FracV() const {
        return v;
    }

    uint64 vstart; // starting V texel coordinate, fractional
    uint64 v;      // current V texel coordinate, fractional
    sint64 vinc;   // V texel coordinate increment per step, fractional
};

} // namespace ymir::vdp
