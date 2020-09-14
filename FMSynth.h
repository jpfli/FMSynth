
#pragma once

#include <cstdint>
#include <USBMIDI.h>

#include "FixedPoint/Fixed.h"
#include "FMPatch.h"
#include "FMSource.h"

class FMSynth {
    
    public:
    
        static constexpr std::uint32_t SAMPLERATE = 32000; // 8000, 12000, 16000, 22050, 24000, 32000, 44100, 48000
        
        FMSynth() = delete;
        ~FMSynth() = delete;
        
        FMSynth(const FMSynth&) = delete;
        FMSynth& operator =(const FMSynth&) = delete;
        
        static void run();
        
        static void noteOn(std::int8_t midikey, std::int8_t velocity);
        static void noteOff(std::int8_t midikey, std::int8_t velocity);
        static void setSustain(bool sustain);
        static void setPitchBend(std::int32_t value);
        
        static inline FMPatch& patch() { return _patch; }
        
        static constexpr std::int32_t numBanks() { return 2; }
        static constexpr std::int32_t numPatches(std::uint32_t bank_idx) { return _banks[bank_idx].num_patches; }
        static inline void selectPatch(std::uint32_t bank_idx, std::uint32_t patch_idx) { _patch = _banks[bank_idx].patches[patch_idx]; }
        
        static constexpr FixedPoint::Fixed<15> toFixedFrequency(std::int8_t coarse, std::int8_t fine) { return FMSource<SAMPLERATE>::toFixedFrequency(coarse, fine); }
        static constexpr FixedPoint::Fixed<15> toLFOFrequency(std::int8_t speed) { return FMSource<SAMPLERATE>::toLFOFrequency(speed); }
        
        static inline std::int8_t MIDIChannel() { return _midi_channel; }
        static inline void setMIDIChannel(std::int8_t channel) { _midi_channel = channel > 0 ? (channel < 15 ? channel : 15) : 0; }
        
        static bool sendMIDIPatchDump();
        static bool sendMIDINoteOff(std::int32_t midikey, std::int32_t velocity);
        static bool sendMIDINoteOn(std::int32_t midikey, std::int32_t velocity);
        static bool sendMIDIControlChange(std::int32_t controller, std::int32_t value);
        static bool sendMIDIPitchWheel(std::int32_t pitch);
        
        static void onMIDIReceived(MIDIMessage msg);
    
    private:
    
        static void _connectUSB();
        
        static constexpr std::uint32_t _PATCHES_PER_BANK = 16;
        struct FMBank {
            std::uint32_t num_patches;
            FMPatch patches[_PATCHES_PER_BANK];
        };
        
        static constexpr FMBank _banks[2] = {
            { // Bank 1
                .num_patches=6,
                .patches={
                    {
                        "SINE", 
                        .algorithm=1, .volume=80, .feedback=50, .glide=0, .attack=20, .decay=0, .sustain=100, .release=40, 
                        .lfo={.speed=0, .attack=0, .pmd=0}, 
                        .op={
                            {.level=100, .pitch={.fixed=false, .coarse=1, .fine=0}, .detune=50, .attack=0, .decay=0, .sustain=100, .loop=false}, 
                            {.level=0, .pitch={.fixed=false, .coarse=1, .fine=0}, .detune=50, .attack=0, .decay=0, .sustain=100, .loop=false}, 
                            {.level=0, .pitch={.fixed=false, .coarse=1, .fine=0}, .detune=50, .attack=0, .decay=0, .sustain=100, .loop=false}, 
                            {.level=0, .pitch={.fixed=false, .coarse=1, .fine=0}, .detune=50, .attack=0, .decay=0, .sustain=100, .loop=false}
                        }
                    }, {
                        "SQUARE", 
                        .algorithm=10, .volume=80, .feedback=50, .glide=0, .attack=20, .decay=0, .sustain=100, .release=40, 
                        .lfo={.speed=0, .attack=0, .pmd=0}, 
                        .op={
                            {.level=0, .pitch={.fixed=false, .coarse=1, .fine=0}, .detune=50, .attack=0, .decay=0, .sustain=100, .loop=false}, 
                            {.level=0, .pitch={.fixed=false, .coarse=1, .fine=0}, .detune=50, .attack=0, .decay=0, .sustain=100, .loop=false}, 
                            {.level=100, .pitch={.fixed=false, .coarse=1, .fine=0}, .detune=50, .attack=0, .decay=0, .sustain=100, .loop=false}, 
                            {.level=25, .pitch={.fixed=false, .coarse=2, .fine=0}, .detune=50, .attack=0, .decay=0, .sustain=100, .loop=false}
                        }
                    }, {
                        "SAW", 
                        .algorithm=11, .volume=80, .feedback=77, .glide=0, .attack=20, .decay=0, .sustain=100, .release=40, 
                        .lfo={.speed=0, .attack=0, .pmd=0}, 
                        .op={
                            {.level=0, .pitch={.fixed=false, .coarse=1, .fine=0}, .detune=50, .attack=0, .decay=0, .sustain=100, .loop=false}, 
                            {.level=0, .pitch={.fixed=false, .coarse=1, .fine=0}, .detune=50, .attack=0, .decay=0, .sustain=100, .loop=false}, 
                            {.level=0, .pitch={.fixed=false, .coarse=1, .fine=0}, .detune=50, .attack=0, .decay=0, .sustain=100, .loop=false}, 
                            {.level=100, .pitch={.fixed=false, .coarse=1, .fine=0}, .detune=50, .attack=0, .decay=0, .sustain=100, .loop=false}
                        }
                    }, {
                        "NOISE", 
                        .algorithm=10, .volume=80, .feedback=100, .glide=0, .attack=20, .decay=0, .sustain=100, .release=40, 
                        .lfo={.speed=0, .attack=0, .pmd=0}, 
                        .op={
                            {.level=0, .pitch={.fixed=false, .coarse=1, .fine=0}, .detune=50, .attack=0, .decay=0, .sustain=100, .loop=false}, 
                            {.level=0, .pitch={.fixed=false, .coarse=1, .fine=0}, .detune=50, .attack=0, .decay=0, .sustain=100, .loop=false}, 
                            {.level=100, .pitch={.fixed=true, .coarse=9, .fine=72}, .detune=50, .attack=0, .decay=0, .sustain=100, .loop=false}, 
                            {.level=100, .pitch={.fixed=true, .coarse=15, .fine=5}, .detune=50, .attack=0, .decay=0, .sustain=100, .loop=false}
                        }
                    }, {
                        "ECHO", 
                        .algorithm=3, .volume=80, .feedback=50, .glide=0, .attack=0, .decay=80, .sustain=0, .release=50, 
                        .lfo={.speed=0, .attack=0, .pmd=0}, 
                        .op={
                            {.level=100, .pitch={.fixed=false, .coarse=1, .fine=0}, .detune=50, .attack=0, .decay=45, .sustain=0, .loop=true}, 
                            {.level=0, .pitch={.fixed=false, .coarse=1, .fine=0}, .detune=50, .attack=0, .decay=0, .sustain=100, .loop=false}, 
                            {.level=0, .pitch={.fixed=false, .coarse=1, .fine=0}, .detune=50, .attack=0, .decay=0, .sustain=100, .loop=false}, 
                            {.level=30, .pitch={.fixed=false, .coarse=2, .fine=0}, .detune=50, .attack=0, .decay=80, .sustain=0, .loop=false}
                        }
                    }, {
                        "SWEEP", 
                        .algorithm=4, .volume=80, .feedback=77, .glide=0, .attack=0, .decay=0, .sustain=100, .release=50, 
                        .lfo={.speed=0, .attack=0, .pmd=0}, 
                        .op={
                            {.level=100, .pitch={.fixed=false, .coarse=1, .fine=0}, .detune=50, .attack=0, .decay=38, .sustain=0, .loop=true}, 
                            {.level=70, .pitch={.fixed=false, .coarse=1, .fine=0}, .detune=50, .attack=65, .decay=65, .sustain=0, .loop=true}, 
                            {.level=0, .pitch={.fixed=false, .coarse=1, .fine=0}, .detune=50, .attack=0, .decay=0, .sustain=100, .loop=false}, 
                            {.level=80, .pitch={.fixed=false, .coarse=2, .fine=0}, .detune=50, .attack=65, .decay=65, .sustain=0, .loop=true}
                        }
                    }
                }
            }, { // Bank 2
                .num_patches=12,
                .patches={
                    {
                        "PIANO",
                        .algorithm=3, .volume=80, .feedback=50, .glide=0, .attack=0, .decay=75, .sustain=100, .release=60,
                        .lfo={.speed=0, .attack=0, .pmd=0},
                        .op={
                            {.level=71, .pitch={.fixed=false, .coarse=1, .fine=0}, .detune=50, .attack=0, .decay=70, .sustain=0, .loop=false}, 
                            {.level=23, .pitch={.fixed=false, .coarse=1, .fine=0}, .detune=57, .attack=0, .decay=70, .sustain=0, .loop=false}, 
                            {.level=20, .pitch={.fixed=false, .coarse=5, .fine=0}, .detune=51, .attack=0, .decay=70, .sustain=0, .loop=false}, 
                            {.level=40, .pitch={.fixed=false, .coarse=1, .fine=0}, .detune=50, .attack=0, .decay=50, .sustain=0, .loop=false}
                        }
                    }, {
                        "E.PIANO", 
                        .algorithm=8, .volume=80, .feedback=57, .glide=0, .attack=0, .decay=75, .sustain=0, .release=50, 
                        .lfo={.speed=0, .attack=0, .pmd=0}, 
                        .op={
                            {.level=71, .pitch={.fixed=false, .coarse=1, .fine=0}, .detune=50, .attack=0, .decay=70, .sustain=0, .loop=false}, 
                            {.level=10, .pitch={.fixed=false, .coarse=14, .fine=0}, .detune=50, .attack=0, .decay=50, .sustain=0, .loop=false}, 
                            {.level=71, .pitch={.fixed=false, .coarse=1, .fine=0}, .detune=43, .attack=0, .decay=75, .sustain=0, .loop=false}, 
                            {.level=35, .pitch={.fixed=false, .coarse=1, .fine=0}, .detune=57, .attack=0, .decay=75, .sustain=0, .loop=false}
                        }
                    }, {
                        "ORGAN", 
                        .algorithm=8, .volume=80, .feedback=50, .glide=0, .attack=0, .decay=0, .sustain=100, .release=50, 
                        .lfo={.speed=35, .attack=65, .pmd=3}, 
                        .op={
                            {.level=77, .pitch={.fixed=false, .coarse=0, .fine=25}, .detune=50, .attack=20, .decay=0, .sustain=100, .loop=false}, 
                            {.level=28, .pitch={.fixed=false, .coarse=0, .fine=25}, .detune=57, .attack=0, .decay=0, .sustain=100, .loop=false}, 
                            {.level=60, .pitch={.fixed=false, .coarse=1, .fine=0}, .detune=50, .attack=30, .decay=0, .sustain=100, .loop=false}, 
                            {.level=20, .pitch={.fixed=false, .coarse=4, .fine=0}, .detune=50, .attack=45, .decay=0, .sustain=100, .loop=false}
                        }
                    }, {
                        "CELESTA", 
                        .algorithm=5, .volume=80, .feedback=50, .glide=0, .attack=5, .decay=70, .sustain=0, .release=55, 
                        .lfo={.speed=0, .attack=0, .pmd=0}, 
                        .op={
                            {.level=100, .pitch={.fixed=false, .coarse=1, .fine=0}, .detune=50, .attack=0, .decay=70, .sustain=0, .loop=false}, 
                            {.level=5, .pitch={.fixed=false, .coarse=1, .fine=0}, .detune=70, .attack=0, .decay=50, .sustain=0, .loop=false}, 
                            {.level=20, .pitch={.fixed=false, .coarse=12, .fine=0}, .detune=50, .attack=0, .decay=15, .sustain=0, .loop=false}, 
                            {.level=0, .pitch={.fixed=false, .coarse=1, .fine=0}, .detune=50, .attack=0, .decay=0, .sustain=100, .loop=false}
                        }
                    }, {
                        "GUITAR", 
                        .algorithm=6, .volume=80, .feedback=50, .glide=0, .attack=0, .decay=70, .sustain=0, .release=60, 
                        .lfo={.speed=0, .attack=0, .pmd=0}, 
                        .op={
                            {.level=100, .pitch={.fixed=false, .coarse=1, .fine=0}, .detune=50, .attack=0, .decay=0, .sustain=100, .loop=false}, 
                            {.level=90, .pitch={.fixed=false, .coarse=2, .fine=0}, .detune=50, .attack=8, .decay=55, .sustain=0, .loop=false}, 
                            {.level=25, .pitch={.fixed=false, .coarse=1, .fine=0}, .detune=50, .attack=0, .decay=50, .sustain=85, .loop=false}, 
                            {.level=53, .pitch={.fixed=false, .coarse=5, .fine=0}, .detune=50, .attack=0, .decay=40, .sustain=0, .loop=false}
                        }
                    }, {
                        "DIST.GUITAR", 
                        .algorithm=8, .volume=80, .feedback=32, .glide=0, .attack=0, .decay=0, .sustain=100, .release=45, 
                        .lfo={.speed=0, .attack=0, .pmd=0}, 
                        .op={
                            {.level=71, .pitch={.fixed=false, .coarse=0, .fine=50}, .detune=50, .attack=0, .decay=0, .sustain=100, .loop=false},
                            {.level=85, .pitch={.fixed=false, .coarse=0, .fine=50}, .detune=51, .attack=0, .decay=60, .sustain=70, .loop=false},
                            {.level=71, .pitch={.fixed=false, .coarse=0, .fine=50}, .detune=50, .attack=0, .decay=0, .sustain=100, .loop=false},
                            {.level=75, .pitch={.fixed=false, .coarse=1, .fine=50}, .detune=51, .attack=0, .decay=65, .sustain=60, .loop=false}
                        }
                    }, {
                        "BASS", 
                        .algorithm=8, .volume=80, .feedback=50, .glide=0, .attack=0, .decay=70, .sustain=0, .release=35, 
                        .lfo={.speed=0, .attack=0, .pmd=0}, 
                        .op={
                            {.level=100, .pitch={.fixed=false, .coarse=0, .fine=50}, .detune=50, .attack=0, .decay=70, .sustain=0, .loop=false}, 
                            {.level=62, .pitch={.fixed=false, .coarse=0, .fine=50}, .detune=54, .attack=25, .decay=55, .sustain=0, .loop=false}, 
                            {.level=100, .pitch={.fixed=false, .coarse=0, .fine=50}, .detune=50, .attack=0, .decay=70, .sustain=0, .loop=false}, 
                            {.level=20, .pitch={.fixed=false, .coarse=1, .fine=50}, .detune=50, .attack=0, .decay=60, .sustain=0, .loop=false}
                        }
                    }, {
                        "VIOLIN", 
                        .algorithm=6, .volume=80, .feedback=50, .glide=0, .attack=40, .decay=0, .sustain=100, .release=40, 
                        .lfo={.speed=0, .attack=0, .pmd=0}, 
                        .op={
                            {.level=71, .pitch={.fixed=false, .coarse=1, .fine=0}, .detune=53, .attack=50, .decay=60, .sustain=100, .loop=false},
                            {.level=71, .pitch={.fixed=false, .coarse=1, .fine=0}, .detune=50, .attack=40, .decay=70, .sustain=80, .loop=false},
                            {.level=40, .pitch={.fixed=false, .coarse=3, .fine=0}, .detune=50, .attack=25, .decay=0, .sustain=100, .loop=false},
                            {.level=30, .pitch={.fixed=false, .coarse=1, .fine=0}, .detune=51, .attack=25, .decay=65, .sustain=70, .loop=false}
                        }
                    }, {
                        "TRUMPET", 
                        .algorithm=8, .volume=80, .feedback=73, .glide=0, .attack=30, .decay=0, .sustain=100, .release=40, 
                        .lfo={.speed=0, .attack=0, .pmd=0}, 
                        .op={
                            {.level=71, .pitch={.fixed=false, .coarse=1, .fine=0}, .detune=50, .attack=0, .decay=0, .sustain=100, .loop=false}, 
                            {.level=52, .pitch={.fixed=false, .coarse=0, .fine=50}, .detune=55, .attack=40, .decay=50, .sustain=75, .loop=false}, 
                            {.level=71, .pitch={.fixed=false, .coarse=1, .fine=0}, .detune=57, .attack=0, .decay=0, .sustain=100, .loop=false}, 
                            {.level=44, .pitch={.fixed=false, .coarse=0, .fine=50}, .detune=53, .attack=35, .decay=50, .sustain=75, .loop=false}
                        }
                    }, {
                        "THEREMIN", 
                        .algorithm=1, .volume=80, .feedback=50, .glide=45, .attack=40, .decay=0, .sustain=100, .release=60, 
                        .lfo={.speed=70, .attack=60, .pmd=5}, 
                        .op={
                            {.level=100, .pitch={.fixed=false, .coarse=1, .fine=0}, .detune=50, .attack=0, .decay=0, .sustain=100, .loop=false}, 
                            {.level=20, .pitch={.fixed=false, .coarse=1, .fine=0}, .detune=51, .attack=0, .decay=0, .sustain=100, .loop=false}, 
                            {.level=0, .pitch={.fixed=false, .coarse=1, .fine=0}, .detune=50, .attack=0, .decay=0, .sustain=100, .loop=false}, 
                            {.level=0, .pitch={.fixed=false, .coarse=1, .fine=0}, .detune=50, .attack=0, .decay=0, .sustain=100, .loop=false}
                        }
                    }, {
                        "GONG", 
                        .algorithm=7, .volume=80, .feedback=50, .glide=0, .attack=0, .decay=85, .sustain=0, .release=50, 
                        .lfo={.speed=0, .attack=0, .pmd=0}, 
                        .op={
                            {.level=100, .pitch={.fixed=false, .coarse=0, .fine=50}, .detune=50, .attack=0, .decay=85, .sustain=0, .loop=false}, 
                            {.level=28, .pitch={.fixed=false, .coarse=0, .fine=80}, .detune=50, .attack=0, .decay=85, .sustain=0, .loop=false}, 
                            {.level=28, .pitch={.fixed=false, .coarse=0, .fine=75}, .detune=50, .attack=75, .decay=80, .sustain=25, .loop=false}, 
                            {.level=40, .pitch={.fixed=false, .coarse=1, .fine=40}, .detune=50, .attack=75, .decay=0, .sustain=100, .loop=false}
                        }
                    }, {
                        "COWBELL", 
                        .algorithm=8, .volume=80, .feedback=50, .glide=0, .attack=0, .decay=50, .sustain=0, .release=40, 
                        .lfo={.speed=0, .attack=0, .pmd=0}, 
                        .op={
                            {.level=71, .pitch={.fixed=true, .coarse=13, .fine=74}, .detune=50, .attack=0, .decay=30, .sustain=0, .loop=false}, 
                            {.level=25, .pitch={.fixed=true, .coarse=13, .fine=7}, .detune=50, .attack=0, .decay=0, .sustain=100, .loop=false}, 
                            {.level=71, .pitch={.fixed=true, .coarse=11, .fine=87}, .detune=50, .attack=0, .decay=50, .sustain=0, .loop=false}, 
                            {.level=13, .pitch={.fixed=true, .coarse=14, .fine=14}, .detune=50, .attack=0, .decay=20, .sustain=25, .loop=false}
                        }
                    }
                }
            }
        };
        
        static FMPatch _patch;
        
        static FMSource<SAMPLERATE> _voices[4];
        static std::uint32_t _voice_idx;
        
        static bool _sustain;
        static bool _sustained_voices[4];
        
        static USBMIDI* _midi;
        static std::int8_t _midi_channel;
        
        static std::int8_t _prev_midikey;
};
