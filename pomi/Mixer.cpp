
#include <cstdint>
#include "Mixer.h"

std::int32_t Pomi::Mixer::_to_dac = 128;
std::int32_t Pomi::Mixer::_gain_Q8 = 1 << 8;
Pomi::Mixer::Channel Pomi::Mixer::_channels[4];
