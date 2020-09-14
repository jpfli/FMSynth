
#pragma once

#include <cstdint>

struct FMPatch {
    char name[16];
    
    std::int8_t algorithm;
    std::int8_t volume;
    std::int8_t feedback;
    std::int8_t glide;
    
    std::int8_t attack;
    std::int8_t decay;
    std::int8_t sustain;
    std::int8_t release;
    
    struct LFO {
        std::int8_t speed;
        std::int8_t attack;
        std::int8_t pmd;
    } lfo;
    
    struct Operator {
        std::uint8_t level;
        
        struct Pitch {
            bool fixed;
            std::int8_t coarse;
            std::int8_t fine;
        } pitch;
        
        std::int8_t detune;
        
        std::int8_t attack;
        std::int8_t decay;
        std::int8_t sustain;
        bool loop;
    } op[4];
};
