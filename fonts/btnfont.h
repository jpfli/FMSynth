
#pragma once

constexpr const unsigned char btnfont[] = {
  5, 6, 32, 1, // width, height, start char, caps only
  0x04, 0x00, 0x00, 0x00, 0x00, 0x00,       // 32: ' '
  0x04, 0x00, 0x2F, 0x07, 0x00, 0x00,       // 33: '!'
  0x04, 0x03, 0x00, 0x03, 0x00, 0x00,       // 34: '"'
  0x04, 0x1F, 0x0A, 0x0A, 0x1F, 0x00,       // 35: '#'
  0x04, 0x16, 0x3F, 0x1A, 0x00, 0x00,       // 36: '$'
  0x04, 0x31, 0x1C, 0x0E, 0x23, 0x00,       // 37: '%'
  0x04, 0x1A, 0x25, 0x3A, 0x30, 0x00,       // 38: '&'
  0x04, 0x00, 0x06, 0x03, 0x00, 0x00,       // 39: '''
  0x04, 0x00, 0x1E, 0x21, 0x00, 0x00,       // 40: '('
  0x04, 0x00, 0x21, 0x1E, 0x00, 0x00,       // 41: ')'
  0x04, 0x12, 0x0C, 0x0C, 0x12, 0x00,       // 42: '*'
  0x04, 0x00, 0x08, 0x1C, 0x08, 0x00,       // 43: '+'
  0x04, 0x20, 0x38, 0x18, 0x00, 0x00,       // 44: ','
  0x04, 0x00, 0x08, 0x08, 0x08, 0x00,       // 45: '-'
  0x04, 0x00, 0x30, 0x30, 0x00, 0x00,       // 46: '.'
  0x04, 0x30, 0x1C, 0x0E, 0x03, 0x00,       // 47: '/'
  0x04, 0x1E, 0x3F, 0x29, 0x1E, 0x00,       // 48: '0'
  0x04, 0x22, 0x3F, 0x3F, 0x20, 0x00,       // 49: '1'
  0x04, 0x31, 0x39, 0x2F, 0x26, 0x00,       // 50: '2'
  0x04, 0x11, 0x25, 0x3F, 0x1A, 0x00,       // 51: '3'
  0x04, 0x0C, 0x0A, 0x3F, 0x3F, 0x00,       // 52: '4'
  0x04, 0x17, 0x27, 0x3D, 0x19, 0x00,       // 53: '5'
  0x04, 0x1E, 0x3F, 0x25, 0x19, 0x00,       // 54: '6'
  0x04, 0x01, 0x39, 0x3F, 0x07, 0x00,       // 55: '7'
  0x04, 0x1A, 0x3F, 0x2D, 0x1A, 0x00,       // 56: '8'
  0x04, 0x06, 0x29, 0x3F, 0x1E, 0x00,       // 57: '9'
  0x04, 0x00, 0x00, 0x1B, 0x1B, 0x00,       // 58: ':'
  0x04, 0x00, 0x20, 0x3B, 0x1B, 0x00,       // 59: ';'
  0x04, 0x00, 0x04, 0x0A, 0x11, 0x00,       // 60: '<'
  0x04, 0x0A, 0x0A, 0x0A, 0x0A, 0x00,       // 61: '='
  0x04, 0x00, 0x11, 0x0A, 0x04, 0x00,       // 62: '>'
  0x04, 0x01, 0x2D, 0x07, 0x02, 0x00,       // 63: '?'
  0x04, 0x1E, 0x23, 0x2F, 0x2E, 0x00,       // 64: '@'
  0x04, 0x3E, 0x3F, 0x09, 0x3E, 0x00,       // 65: 'A'
  0x04, 0x3F, 0x3F, 0x25, 0x1A, 0x00,       // 66: 'B'
  0x04, 0x1E, 0x3F, 0x21, 0x21, 0x00,       // 67: 'C'
  0x04, 0x3F, 0x3F, 0x21, 0x1E, 0x00,       // 68: 'D'
  0x04, 0x3F, 0x3F, 0x25, 0x21, 0x00,       // 69: 'E'
  0x04, 0x3F, 0x3F, 0x05, 0x01, 0x00,       // 70: 'F'
  0x04, 0x1E, 0x3F, 0x21, 0x39, 0x00,       // 71: 'G'
  0x04, 0x3F, 0x3F, 0x08, 0x3F, 0x00,       // 72: 'H'
  0x04, 0x21, 0x3F, 0x3F, 0x21, 0x00,       // 73: 'I'
  0x04, 0x18, 0x31, 0x3F, 0x1F, 0x00,       // 74: 'J'
  0x04, 0x3F, 0x3F, 0x0C, 0x33, 0x00,       // 75: 'K'
  0x04, 0x3F, 0x3F, 0x20, 0x20, 0x00,       // 76: 'L'
  0x04, 0x3F, 0x3F, 0x06, 0x3F, 0x00,       // 77: 'M'
  0x04, 0x3F, 0x3F, 0x01, 0x3E, 0x00,       // 78: 'N'
  0x04, 0x1E, 0x3F, 0x21, 0x1E, 0x00,       // 79: 'O'
  0x04, 0x3F, 0x3F, 0x09, 0x06, 0x00,       // 80: 'P'
  0x04, 0x1E, 0x3F, 0x31, 0x3E, 0x00,       // 81: 'Q'
  0x04, 0x3F, 0x3F, 0x09, 0x36, 0x00,       // 82: 'R'
  0x04, 0x26, 0x2F, 0x3D, 0x19, 0x00,       // 83: 'S'
  0x04, 0x01, 0x3F, 0x3F, 0x01, 0x00,       // 84: 'T'
  0x04, 0x1F, 0x3F, 0x20, 0x1F, 0x00,       // 85: 'U'
  0x04, 0x1F, 0x3F, 0x10, 0x0F, 0x00,       // 86: 'V'
  0x04, 0x3F, 0x3F, 0x18, 0x3F, 0x00,       // 87: 'W'
  0x04, 0x33, 0x0E, 0x1C, 0x33, 0x00,       // 88: 'X'
  0x04, 0x07, 0x3F, 0x3C, 0x07, 0x00,       // 89: 'Y'
  0x04, 0x31, 0x3D, 0x2F, 0x23, 0x00,       // 90: 'Z'
  0x04, 0x00, 0x3F, 0x21, 0x00, 0x00,       // 91: '['
  0x04, 0x03, 0x0E, 0x1C, 0x30, 0x00,       // 92: '\'
  0x04, 0x00, 0x21, 0x3F, 0x00, 0x00        // 93: ']'
};
