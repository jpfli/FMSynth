
#include <Pokitto.h>
#include "mbed.h"
#include "SoftwareI2C.h"

#include "pomi/hw/Display.h"
#include "pomi/Mixer.h"
#include "FMSynth.h"
#include "FMSource.h"

constexpr std::uint32_t MATCHNUM = 1;

extern "C" void TIMER32_0_IRQHandler() {
    volatile unsigned int* TIMER = reinterpret_cast<unsigned int*>(0x40014000);
    if((TIMER[0] & (1 << Pomi::Mixer::MATCHNUM)) != 0) {
        TIMER[0] = 1 << Pomi::Mixer::MATCHNUM;
        
        // Clear the update flag so that the next FMSource can update its control values
        FMSource<FMSynth::SAMPLERATE>::clearUpdateFlag();
        
        Pomi::Mixer::onTimerMatch();
    }
}

inline void setHWVolume(std::int32_t vol) {
    // These correspond to {-inf, -30, -25, -20, -15, -10, -5, 0} desibels
    constexpr std::uint8_t hw_levels[8] = {0, 3, 7, 13, 25, 44, 80, 127};
    
    SoftwareI2C swvolpot(P0_4, P0_5);
    swvolpot.write(0x5e, hw_levels[vol >> 5]);
}

int main() {
    Pokitto::Core::begin();
    
    // Need to set volume manually because Pokitto sound is disabled.
    setHWVolume(eeprom_read_byte((std::uint16_t*)EESETTINGS_VOL));
    
    // Set port 2 mask and display auto increment direction
    LPC_GPIO_PORT->MASK[2] = ~0x7fff8;
    Pomi::Hw::Display::setDirection(Pomi::Hw::Display::Direction::HORIZONTAL);
    
    FMSynth::run();
    
    return 0;
}
