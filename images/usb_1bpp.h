
#pragma once

#include <cstdint>

constexpr std::uint8_t usb_1bpp[] = {
48, 22,
0x00,0x00,0x00,0x1e,0x00,0x00,
0x00,0x00,0x00,0x3f,0x00,0x00,
0x00,0x00,0x07,0xff,0x00,0x00,
0x00,0x00,0x1f,0xff,0x00,0x00,
0x00,0x00,0x3c,0x3f,0x00,0x00,
0x00,0x00,0x70,0x1e,0x00,0x00,
0x00,0x00,0xe0,0x00,0x00,0x00,
0x3c,0x01,0xc0,0x00,0x00,0x00,
0x7e,0x03,0x80,0x00,0x00,0x30,
0xff,0x0f,0x00,0x00,0x00,0x3c,
0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0x00,0x1e,0x00,0x00,0x3c,
0x7e,0x00,0x07,0x00,0x00,0x30,
0x3c,0x00,0x03,0x80,0x00,0x00,
0x00,0x00,0x01,0xc0,0x00,0x00,
0x00,0x00,0x00,0xe0,0x7e,0x00,
0x00,0x00,0x00,0x78,0x7e,0x00,
0x00,0x00,0x00,0x3f,0xfe,0x00,
0x00,0x00,0x00,0x0f,0xfe,0x00,
0x00,0x00,0x00,0x00,0x7e,0x00,
0x00,0x00,0x00,0x00,0x7e,0x00
};