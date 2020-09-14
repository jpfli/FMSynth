
#pragma once

#include <cstdint>
#include "hw/Core.h"

namespace Pomi {

class Mixer {
    
    public:
    
        using Callback = std::int32_t (*)(void*);
        
        static constexpr std::uint32_t MATCHNUM = 1;
        
        Mixer() = delete;
        Mixer(const Mixer&) = delete;
        Mixer& operator =(const Mixer&) = delete;
        
        static inline void init(std::uint32_t samplerate) {
            Hw::Core::enableDAC();
            Hw::Core::enableTimer(0, MATCHNUM, samplerate);
        }
        
        static inline void setChannel(std::uint8_t channel, Callback callback, void* data=nullptr) {
            // Temporarily change to nullSource callback before setting data, to protect against random crashes,
            // when audio IRQ executes just after setting data but before the new callback is set
            Channel& ch = _channels[channel];
            ch.callback = nullSource;
            ch.data = data;
            if(callback) ch.callback = callback;
        }
        
        // Sets the master volume. 256 equals unit gain.
        static inline void setVolume(std::uint32_t volume) {
            _gain_Q8 = (volume * volume) >> 8;
        }
        
        static inline void onTimerMatch() {
            // Before anything else, the previous output value is written to the DAC.
            // This way DAC is fed at constant rate even if the processing time varies.
            Hw::Core::writeDAC(_to_dac);
            
            // Process all channels and mix them together
            // std::int32_t out_Q15 = _channels[0].callback(_channels[0].data);
            // out_Q15 += _channels[1].callback(_channels[1].data);
            // out_Q15 += _channels[2].callback(_channels[2].data);
            // out_Q15 += _channels[3].callback(_channels[3].data);
            
            std::int32_t out_Q15 = 0;
            for(std::uint32_t idx = 0; idx < 4; ++idx) {
                Channel& ch = _channels[idx];
                out_Q15 += ch.callback(ch.data);
            }
            
            // Convert output to unsigned 8 bit value
            _to_dac = 128 + (out_Q15 * _gain_Q8) / (1 << 16);
            _to_dac = (_to_dac > 255) ? 255 : ((_to_dac < 0) ? 0 : _to_dac);
        }
    
    private:
    
        struct Channel {
            constexpr Channel(): callback(nullSource), data(nullptr) {}
            
            volatile Callback callback;
            void* volatile data;
        };
        
        static constexpr std::int32_t nullSource(void* data) {
            return 0;
        }
        
        static std::int32_t _to_dac;
        static std::int32_t _gain_Q8;
        static Channel _channels[4];
};

} // namespace Pomi
