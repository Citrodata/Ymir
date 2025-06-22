#pragma once

#include <ymir/state/state_scsp_slot.hpp>

#include <ymir/util/inline.hpp>

#include <ymir/core/types.hpp>

#include <array>

namespace ymir::scsp {

struct Slot {
    Slot();

    void Reset();

    bool TriggerKey();

    // -------------------------------------------------------------------------

    template <typename T>
    T ReadReg(uint32 address);

    template <typename T>
    void WriteReg(uint32 address, T value);

    template <bool lowerByte, bool upperByte>
    uint16 ReadReg00() const;

    template <bool lowerByte, bool upperByte>
    void WriteReg00(uint16 value);

    uint16 ReadReg02() const;

    template <bool lowerByte, bool upperByte>
    void WriteReg02(uint16 value);

    uint16 ReadReg04() const;

    template <bool lowerByte, bool upperByte>
    void WriteReg04(uint16 value);

    uint16 ReadReg06() const;

    template <bool lowerByte, bool upperByte>
    void WriteReg06(uint16 value);

    template <bool lowerByte, bool upperByte>
    uint16 ReadReg08() const;

    template <bool lowerByte, bool upperByte>
    void WriteReg08(uint16 value);

    template <bool lowerByte, bool upperByte>
    uint16 ReadReg0A() const;

    template <bool lowerByte, bool upperByte>
    void WriteReg0A(uint16 value);

    template <bool lowerByte, bool upperByte>
    uint16 ReadReg0C() const;

    template <bool lowerByte, bool upperByte>
    void WriteReg0C(uint16 value);

    template <bool lowerByte, bool upperByte>
    uint16 ReadReg0E() const;

    template <bool lowerByte, bool upperByte>
    void WriteReg0E(uint16 value);

    template <bool lowerByte, bool upperByte>
    uint16 ReadReg10() const;

    template <bool lowerByte, bool upperByte>
    void WriteReg10(uint16 value);

    template <bool lowerByte, bool upperByte>
    uint16 ReadReg12() const;

    template <bool lowerByte, bool upperByte>
    void WriteReg12(uint16 value);

    template <bool lowerByte, bool upperByte>
    uint16 ReadReg14() const;

    template <bool lowerByte, bool upperByte>
    void WriteReg14(uint16 value);

    template <bool lowerByte, bool upperByte>
    uint16 ReadReg16() const;

    template <bool lowerByte, bool upperByte>
    void WriteReg16(uint16 value);

    // -------------------------------------------------------------------------
    // Save states

    void SaveState(state::SCSPSlotState &state) const;
    bool ValidateState(const state::SCSPSlotState &state) const;
    void LoadState(const state::SCSPSlotState &state);

    // -------------------------------------------------------------------------
    // Parameters

    // This slot's index, for debugging
    uint32 index;

    // -------------------------------------------------------------------------
    // Registers

    // --- Loop Control Register ---

    uint32 startAddress;     // (R/W) SA - Start Address
    uint16 loopStartAddress; // (R/W) LSA - Loop Start Address
    uint16 loopEndAddress;   // (R/W) LEA - Loop End Address
    bool pcm8Bit;            // (R/W) PCM8B - Wave format (true=8-bit PCM, false=16-bit PCM)
    bool keyOnBit;           // (R/W) KYONB - Key On Bit

    // Loop control specifies how the loop segment is played if the sound is held continuously.
    // All modes play the segment between SA and LSA forwards.
    //   Off disables sample looping. The sample stops at LEA.
    //   Normal loops the segment between LSA and LEA forwards.
    //   Reverse plays forwards from SA to LSA, then jumps to LEA and repeats the loop segment in reverse.
    //   Alternate plays the loop segment forwards, then backwards, then forwards, ...
    //
    //            SA     LSA         LEA
    //            |       |           |
    //       Off  +--->---+--->-------X   sample stops playing at LEA
    //            |       |           |
    //    Normal  +--->---+--->------->   sample repeats from LSA when it hits
    //            |       +--->------->   LEA and always plays forwards
    //            |       +--->------->
    //            |       |           |
    //   Reverse  +--->--->  >  >  >  |   sample skips LSA,
    //            |       <-------<---+   plays backwards from LEA,
    //            |       <-------<---+   and repeats from LEA upon reaching LSA;
    //            |       <-------<---+   always plays in reverse
    //            |       |           |
    // Alternate  +--->---+--->------->   sample plays forwards until LEA
    //            |       <-------<---+   then plays backwards until LSA,
    //            |       +--->------->   and keeps bouncing back and forth
    enum class LoopControl { Off, Normal, Reverse, Alternate };
    LoopControl loopControl; // (R/W) LPCTL

    // SBCTL enables XORing sample data.
    //   bit 0 flips every bit other than the sign bit
    //   bit 1 flips the sign bit
    // This is useful for supporting samples in different formats (e.g. unsigned)
    // Implementation notes:
    //   SBCTL0: 0x7FFF
    //   SBCTL1: 0x8000
    uint16 sampleXOR; // (R/W) SBCTL0/1

    enum class SoundSource { SoundRAM, Noise, Silence, Unknown };
    SoundSource soundSource; // (R/W) SSCTL

    // --- Envelope Generator Register ---

    // Starts from Attack on Key ON.
    // While Key ON is held, goes through Attack -> Decay 1 -> Decay 2 and stays at the minimum value of Decay 2.
    // On Key OFF, it will immediately skip to Release state, decrementing the envelope from whatever point it was.
    //
    // when EGHOLD=0:
    // 0x000       _
    //            /|\
    //           / | \
    //          /  |  +-__
    //         /   |  |   -+ DL
    // 0x3FF  /    |  |    |\_____...
    //       |atk  |d1|d2  |release
    // Key ON^     Key OFF^
    //
    // when EGHOLD=1:
    // 0x000 _______
    //       |     |\
    //       |     | \
    //       |     |  +-__
    //       |     |  |   -+ DL
    // 0x3FF |     |  |    |\_____...
    //       |atk  |d1|d2  |release
    // Key ON^        OFF^
    //
    // Note: attack takes the same amount of time it would take if going from 0x3FF to 0x000 normally

    // Value ranges are from minimum to maximum.
    uint8 attackRate;  // (R/W) AR  - 0x00 to 0x1F
    uint8 decay1Rate;  // (R/W) D1R - 0x00 to 0x1F
    uint8 decay2Rate;  // (R/W) D2R - 0x00 to 0x1F
    uint8 releaseRate; // (R/W) RR  - 0x00 to 0x1F

    uint8 decayLevel; // (R/W) DL  - 0x1F to 0x00
                      //   specifies the MSB 5 bits of the EG value where to switch from decay 1 to decay 2

    uint8 keyRateScaling; // (R/W) KRS - 0x00 to 0x0E; 0x0F turns off scaling

    bool egHold; // (R/W) EGHOLD
                 //   false: volume raises during attack state
                 //   true:  volume is set to maximum during attack phase while maintaining the same duration

    bool loopStartLink; // (R/W) LPSLNK
                        //   true:  switches to decay 1 state on LSA
                        //          attack state is interrupted if too slow or held if too fast
                        //          if the state change happens below DL, decay 2 state is never reached
                        //   false: state changes are dictated by rates only

    bool egBypass; // (R/W) EGBYPASS(?) (undocumented)
                   //  true:  volume is always set to maximum regardless of EG state
                   //  false: volume follows EG level

    // --- FM Modulation Control Register ---

    uint8 modLevel;         // (R/W) MDL - add +- n * pi where n is:
                            // 0-4   5     6    7    8   9  A  B  C  D   E   F
                            //  0   1/16  1/8  1/4  1/2  1  2  4  8  16  32  64
    uint8 modXSelect;       // (R/W) MDXSL - selects modulation input X
    uint8 modYSelect;       // (R/W) MDYSL - selects modulation input Y
    bool stackWriteInhibit; // (R/W) STWINH - when set, blocks writes to direct data stack (SOUS)

    // --- Sound Volume Register ---

    uint8 totalLevel; // (R/W) TL - 0x00 = no attenuation, 0xFF = max attenuation (-95.7 dB)
    bool soundDirect; // (R/W) SDIR - true causes the sound from this slot to bypass the EG, TL, ALFO, etc.

    // --- Pitch Register ---

    uint8 octave;         // (R/W) OCT - octave
    uint16 freqNumSwitch; // (R/W) FNS - frequency number switch
    bool maskMode;        // (R/W) MM(?) - mask mode (undocumented)

    // --- LFO Register ---

    enum class Waveform : uint8 { Saw, Square, Triangle, Noise };

    static constexpr std::array<uint32, 32> s_lfoStepTbl = {1020, 892, 764, 636, 508, 444, 380, 316, 252, 220, 188,
                                                            156,  124, 108, 92,  76,  60,  52,  44,  36,  28,  24,
                                                            20,   16,  12,  10,  8,   6,   4,   3,   2,   1};

    bool lfoReset;             // (R/W) LFORE - true resets the LFO (TODO: is this a one-shot action?)
    uint8 lfofRaw;             // (R/W) LFOF - 0x00 to 0x1F (raw value)
    uint32 lfoStepInterval;    // (R/W) LFOF - determines the LFO increment interval (from s_lfoStepTbl)
    uint8 ampLFOSens;          // (R/W) ALFOS - 0 (none) to 7 (maximum) intensity of tremor effect
    uint8 pitchLFOSens;        // (R/W) PLFOS - 0 (none) to 7 (maximum) intensity of tremolo effect
    Waveform ampLFOWaveform;   // (R/W) ALFOWS - unsigned from 0x00 to 0xFF (all waveforms start at zero and increment)
    Waveform pitchLFOWaveform; // (R/W) PLFOWS - signed from 0x80 to 0x7F (zero at 0x00, starting point of saw/triangle)

    // --- Mixer Register ---

    uint8 inputMixingLevel; // (R/W) IMXL - 0 (no mix) to 7 (maximum) - into MIXS DSP stack
    uint8 inputSelect;      // (R/W) ISEL - 0 to 15 - indexes a MIXS DSP stack
    uint8 directSendLevel;  // (R/W) DISDL - 0 (no send) to 7 (maximum)
    uint8 directPan;        // (R/W) DIPAN - 0 to 31 - [100% left]  31..16  [center]  0..15  [100% right]

    // These are not tied to slots, but they exist within the slot register address space.
    // "Slots" 0 through 15 refer to DSP.EFREG[0-15].
    // "Slots" 16 and 17 refer to DSP.EXTS[0-1].
    uint8 effectSendLevel; // (R/W) EFSDL - 0 (no send) to 7 (maximum)
    uint8 effectPan;       // (R/W) EFPAN - 0 to 31 - [100% left]  31..16  [center]  0..15  [100% right]

    // --- Extra bits ---

    // Storage for unused but writable bits.
    uint16 extraBits0C; // bits 10 and 11
    uint16 extraBits14; // bit 7

    // -------------------------------------------------------------------------
    // State

    bool active;

    enum class EGState : uint8 { Attack, Decay1, Decay2, Release };
    EGState egState;

    // Current envelope level.
    // Ranges from 0x3FF (minimum) to 0x000 (maximum) - 10 bits.
    uint16 egLevel;

    uint16 currEGLevel;

    bool egAttackBug; // Is the EG stuck in attack phase?

    uint32 currSample;
    uint32 currPhase;
    uint32 nextPhase;
    sint16 modXSample;
    sint16 modYSample;
    sint32 modulation;
    bool reverse;
    bool crossedLoopStart;

    uint32 lfoCycles; // Incremented every sample
    uint8 lfoStep;    // Incremented when lfoCycles reaches lfoStepInterval

    uint8 alfoOutput;

    sint16 sample1;
    sint16 sample2;
    sint16 output;

    sint32 finalLevel;

    uint32 CalcEffectiveRate(uint8 rate) const;

    void CheckAttackBug();

    uint8 GetCurrentEGRate() const;
    uint16 GetEGLevel() const;

    void IncrementLFO();

    void IncrementPhase(sint32 pitchLFO);
    void IncrementSampleCounter();
    void IncrementEG(uint64 sampleCounter);
};

} // namespace ymir::scsp
