// Ported From the AROS ROM source code: https://github.com/aros-development-team/AROS/blob/master/rom/graphics/default_font.c
// Which is licensed under the APL license.

#pragma once

const unsigned char font_topaz[192][8] = {
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // 00
    { 0x00, 0x18, 0x18, 0x18, 0x18, 0x00, 0x18, 0x00}, // 01
    { 0x00, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00}, // 02
    { 0x00, 0x66, 0xff, 0x66, 0x66, 0xff, 0x66, 0x00}, // 03
    { 0x18, 0x7c, 0x06, 0x3c, 0x60, 0x3e, 0x18, 0x00}, // 04
    { 0x00, 0x66, 0x36, 0x18, 0x0c, 0x66, 0x62, 0x00}, // 05
    { 0x38, 0x6c, 0x38, 0x1c, 0xf6, 0x66, 0xdc, 0x00}, // 06
    { 0x00, 0x18, 0x18, 0x08, 0x00, 0x00, 0x00, 0x00}, // 07
    { 0x30, 0x18, 0x0c, 0x0c, 0x0c, 0x18, 0x30, 0x00}, // 08
    { 0x0c, 0x18, 0x30, 0x30, 0x30, 0x18, 0x0c, 0x00}, // 09
    { 0x00, 0x66, 0x3c, 0xff, 0x3c, 0x66, 0x00, 0x00}, // 0a
    { 0x00, 0x18, 0x18, 0x7e, 0x18, 0x18, 0x00, 0x00}, // 0b
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x0c}, // 0c
    { 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00}, // 0d
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00}, // 0e
    { 0x00, 0x60, 0x30, 0x18, 0x0c, 0x06, 0x02, 0x00}, // 0f
    { 0x00, 0x3c, 0x66, 0x76, 0x6e, 0x66, 0x3c, 0x00}, // 10
    { 0x00, 0x18, 0x1c, 0x18, 0x18, 0x18, 0x7e, 0x00}, // 11
    { 0x00, 0x3c, 0x66, 0x30, 0x18, 0x0c, 0x7e, 0x00}, // 12
    { 0x00, 0x7e, 0x30, 0x18, 0x30, 0x66, 0x3c, 0x00}, // 13
    { 0x00, 0x30, 0x38, 0x3c, 0x36, 0x7e, 0x30, 0x00}, // 14
    { 0x00, 0x7e, 0x06, 0x3e, 0x60, 0x66, 0x3c, 0x00}, // 15
    { 0x00, 0x3c, 0x06, 0x3e, 0x66, 0x66, 0x3c, 0x00}, // 16
    { 0x00, 0x7e, 0x60, 0x30, 0x18, 0x0c, 0x0c, 0x00}, // 17
    { 0x00, 0x3c, 0x66, 0x3c, 0x66, 0x66, 0x3c, 0x00}, // 18
    { 0x00, 0x3c, 0x66, 0x7c, 0x60, 0x30, 0x1c, 0x00}, // 19
    { 0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x00}, // 1a
    { 0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x0c}, // 1b
    { 0x00, 0x60, 0x18, 0x06, 0x18, 0x60, 0x00, 0x00}, // 1c
    { 0x00, 0x00, 0x7e, 0x00, 0x7e, 0x00, 0x00, 0x00}, // 1d
    { 0x00, 0x06, 0x18, 0x60, 0x18, 0x06, 0x00, 0x00}, // 1e
    { 0x00, 0x3c, 0x66, 0x30, 0x18, 0x00, 0x18, 0x00}, // 1f
    { 0x00, 0x3c, 0x66, 0x76, 0x76, 0x06, 0x7c, 0x00}, // 20
    { 0x00, 0x18, 0x3c, 0x66, 0x66, 0x7e, 0x66, 0x00}, // 21
    { 0x00, 0x3e, 0x66, 0x3e, 0x66, 0x66, 0x3e, 0x00}, // 22
    { 0x00, 0x3c, 0x66, 0x06, 0x06, 0x66, 0x3c, 0x00}, // 23
    { 0x00, 0x3e, 0x66, 0x66, 0x66, 0x66, 0x3e, 0x00}, // 24
    { 0x00, 0x7e, 0x06, 0x3e, 0x06, 0x06, 0x7e, 0x00}, // 25
    { 0x00, 0x7e, 0x06, 0x3e, 0x06, 0x06, 0x06, 0x00}, // 26
    { 0x00, 0x3c, 0x66, 0x06, 0x76, 0x66, 0x3c, 0x00}, // 27
    { 0x00, 0x66, 0x66, 0x7e, 0x66, 0x66, 0x66, 0x00}, // 28
    { 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x7e, 0x00}, // 29
    { 0x00, 0x60, 0x60, 0x60, 0x60, 0x66, 0x3c, 0x00}, // 2a
    { 0x00, 0x66, 0x36, 0x1e, 0x1e, 0x36, 0x66, 0x00}, // 2b
    { 0x00, 0x06, 0x06, 0x06, 0x06, 0x06, 0x7e, 0x00}, // 2c
    { 0x00, 0xc6, 0xee, 0xfe, 0xd6, 0xc6, 0xc6, 0x00}, // 2d
    { 0x00, 0x62, 0x66, 0x6e, 0x76, 0x66, 0x66, 0x00}, // 2e
    { 0x00, 0x3c, 0x66, 0x66, 0x66, 0x66, 0x3c, 0x00}, // 2f
    { 0x00, 0x3e, 0x66, 0x66, 0x3e, 0x06, 0x06, 0x00}, // 30
    { 0x00, 0x3c, 0x66, 0x66, 0x66, 0x36, 0x6c, 0x00}, // 31
    { 0x00, 0x3e, 0x66, 0x66, 0x3e, 0x36, 0x66, 0x00}, // 32
    { 0x00, 0x3c, 0x06, 0x3c, 0x60, 0x60, 0x3c, 0x00}, // 33
    { 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00}, // 34
    { 0x00, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3c, 0x00}, // 35
    { 0x00, 0x66, 0x66, 0x66, 0x66, 0x3c, 0x18, 0x00}, // 36
    { 0x00, 0xc6, 0xc6, 0xd6, 0xfe, 0xee, 0xc6, 0x00}, // 37
    { 0x00, 0x66, 0x66, 0x3c, 0x3c, 0x66, 0x66, 0x00}, // 38
    { 0x00, 0x66, 0x66, 0x3c, 0x18, 0x18, 0x18, 0x00}, // 39
    { 0x00, 0x7e, 0x30, 0x18, 0x0c, 0x06, 0x7e, 0x00}, // 3a
    { 0x78, 0x18, 0x18, 0x18, 0x18, 0x18, 0x78, 0x00}, // 3b
    { 0x00, 0x02, 0x06, 0x0c, 0x18, 0x30, 0x60, 0x00}, // 3c
    { 0x1e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1e, 0x00}, // 3d
    { 0x00, 0x18, 0x3c, 0x7e, 0x00, 0x00, 0x00, 0x00}, // 3e
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x00}, // 3f
    { 0x00, 0x18, 0x18, 0x10, 0x00, 0x00, 0x00, 0x00}, // 40
    { 0x00, 0x00, 0x3c, 0x60, 0x7c, 0x66, 0x7c, 0x00}, // 41
    { 0x00, 0x06, 0x06, 0x3e, 0x66, 0x66, 0x3e, 0x00}, // 42
    { 0x00, 0x00, 0x3c, 0x06, 0x06, 0x06, 0x3c, 0x00}, // 43
    { 0x00, 0x60, 0x60, 0x7c, 0x66, 0x66, 0x7c, 0x00}, // 44
    { 0x00, 0x00, 0x3c, 0x66, 0x7e, 0x06, 0x3c, 0x00}, // 45
    { 0x00, 0x70, 0x18, 0x7c, 0x18, 0x18, 0x18, 0x00}, // 46
    { 0x00, 0x00, 0x7c, 0x66, 0x66, 0x7c, 0x60, 0x3e}, // 47
    { 0x00, 0x06, 0x06, 0x3e, 0x66, 0x66, 0x66, 0x00}, // 48
    { 0x00, 0x18, 0x00, 0x1c, 0x18, 0x18, 0x3c, 0x00}, // 49
    { 0x00, 0x60, 0x00, 0x60, 0x60, 0x60, 0x60, 0x3c}, // 4a
    { 0x00, 0x06, 0x06, 0x36, 0x1e, 0x36, 0x66, 0x00}, // 4b
    { 0x00, 0x1c, 0x18, 0x18, 0x18, 0x18, 0x3c, 0x00}, // 4c
    { 0x00, 0x00, 0x3e, 0x56, 0x56, 0x56, 0x56, 0x00}, // 4d
    { 0x00, 0x00, 0x3e, 0x66, 0x66, 0x66, 0x66, 0x00}, // 4e
    { 0x00, 0x00, 0x3c, 0x66, 0x66, 0x66, 0x3c, 0x00}, // 4f
    { 0x00, 0x00, 0x3e, 0x66, 0x66, 0x3e, 0x06, 0x06}, // 50
    { 0x00, 0x00, 0x7c, 0x66, 0x66, 0x7c, 0x60, 0x60}, // 51
    { 0x00, 0x00, 0x3e, 0x66, 0x06, 0x06, 0x06, 0x00}, // 52
    { 0x00, 0x00, 0x7c, 0x06, 0x3c, 0x60, 0x3e, 0x00}, // 53
    { 0x00, 0x18, 0x7e, 0x18, 0x18, 0x18, 0x70, 0x00}, // 54
    { 0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x7c, 0x00}, // 55
    { 0x00, 0x00, 0x66, 0x66, 0x66, 0x3c, 0x18, 0x00}, // 56
    { 0x00, 0x00, 0x42, 0x42, 0x5a, 0x7e, 0x66, 0x00}, // 57
    { 0x00, 0x00, 0x66, 0x3c, 0x18, 0x3c, 0x66, 0x00}, // 58
    { 0x00, 0x00, 0x66, 0x66, 0x66, 0x7c, 0x30, 0x1e}, // 59
    { 0x00, 0x00, 0x7e, 0x30, 0x18, 0x0c, 0x7e, 0x00}, // 5a
    { 0x70, 0x18, 0x18, 0x0e, 0x18, 0x18, 0x70, 0x00}, // 5b
    { 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00}, // 5c
    { 0x0e, 0x18, 0x18, 0x70, 0x18, 0x18, 0x0e, 0x00}, // 5d
    { 0x00, 0x00, 0x4c, 0x32, 0x00, 0x00, 0x00, 0x00}, // 5e
    { 0xf0, 0x3c, 0x0f, 0xc3, 0xf0, 0x3c, 0x0f, 0x00}, // 5f
    { 0x00, 0x7e, 0x66, 0x66, 0x66, 0x66, 0x7e, 0x00}, // 60
    { 0x18, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00}, // 61
    { 0x00, 0x30, 0x7c, 0x36, 0x7c, 0x30, 0x00, 0x00}, // 62
    { 0x38, 0x6c, 0x0c, 0x1e, 0x0c, 0x0c, 0x7e, 0x00}, // 63
    { 0x38, 0x44, 0x1e, 0x04, 0x0e, 0x44, 0x38, 0x00}, // 64
    { 0xc3, 0x66, 0x3c, 0x18, 0x3c, 0x18, 0x18, 0x00}, // 65
    { 0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x00}, // 66
    { 0x3c, 0x06, 0x3c, 0x66, 0x3c, 0x60, 0x3c, 0x00}, // 67
    { 0x66, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // 68
    { 0x7e, 0x81, 0xb9, 0x8d, 0xb9, 0x81, 0x7e, 0x00}, // 69
    { 0x38, 0x24, 0x22, 0x3c, 0x00, 0x7e, 0x00, 0x00}, // 6a
    { 0x00, 0xcc, 0x66, 0x33, 0x66, 0xcc, 0x00, 0x00}, // 6b
    { 0x7c, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // 6c
    { 0x7e, 0x81, 0x9d, 0xa5, 0x9d, 0xa5, 0x81, 0x7e}, // 6d
    { 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // 6e
    { 0x3c, 0x66, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00}, // 6f
    { 0x18, 0x18, 0x7e, 0x18, 0x18, 0x00, 0x7e, 0x00}, // 70
    { 0x1e, 0x30, 0x18, 0x0c, 0x3e, 0x00, 0x00, 0x00}, // 71
    { 0x1e, 0x30, 0x18, 0x30, 0x1e, 0x00, 0x00, 0x00}, // 72
    { 0x18, 0x0c, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00}, // 73
    { 0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0xfe, 0x06}, // 74
    { 0x7c, 0x5e, 0x5e, 0x5c, 0x50, 0x50, 0x50, 0x00}, // 75
    { 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00}, // 76
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x0c}, // 77
    { 0x0c, 0x0e, 0x0c, 0x0c, 0x0c, 0x00, 0x00, 0x00}, // 78
    { 0x1c, 0x22, 0x22, 0x1c, 0x00, 0x3e, 0x00, 0x00}, // 79
    { 0x00, 0x33, 0x66, 0xcc, 0x66, 0x33, 0x00, 0x00}, // 7a
    { 0x02, 0x63, 0x32, 0x1a, 0x4c, 0x66, 0xf3, 0x40}, // 7b
    { 0x02, 0x63, 0x32, 0x1a, 0x7c, 0x46, 0x23, 0x70}, // 7c
    { 0x03, 0xc4, 0x66, 0x34, 0x9b, 0xcc, 0xe6, 0x80}, // 7d
    { 0x18, 0x00, 0x18, 0x0c, 0x06, 0x66, 0x3c, 0x00}, // 7e
    { 0x0c, 0x18, 0x3c, 0x66, 0x7e, 0x66, 0x66, 0x00}, // 7f
    { 0x30, 0x18, 0x3c, 0x66, 0x7e, 0x66, 0x66, 0x00}, // 80
    { 0x18, 0x66, 0x3c, 0x66, 0x7e, 0x66, 0x66, 0x00}, // 81
    { 0x8e, 0x71, 0x3c, 0x66, 0x7e, 0x66, 0x66, 0x00}, // 82
    { 0x66, 0x00, 0x3c, 0x66, 0x7e, 0x66, 0x66, 0x00}, // 83
    { 0x18, 0x24, 0x3c, 0x66, 0x7e, 0x66, 0x66, 0x00}, // 84
    { 0xf8, 0x3c, 0x3c, 0xf6, 0x3e, 0x33, 0xf3, 0x00}, // 85
    { 0x78, 0x0c, 0x06, 0x06, 0x0c, 0x78, 0x30, 0x18}, // 86
    { 0x0c, 0x18, 0x7e, 0x06, 0x1e, 0x06, 0x7e, 0x00}, // 87
    { 0x30, 0x18, 0x7e, 0x06, 0x1e, 0x06, 0x7e, 0x00}, // 88
    { 0x18, 0x66, 0x7e, 0x06, 0x1e, 0x06, 0x7e, 0x00}, // 89
    { 0x66, 0x00, 0x7e, 0x06, 0x1e, 0x06, 0x7e, 0x00}, // 8a
    { 0x0c, 0x18, 0x3c, 0x18, 0x18, 0x18, 0x3c, 0x00}, // 8b
    { 0x30, 0x18, 0x3c, 0x18, 0x18, 0x18, 0x3c, 0x00}, // 8c
    { 0x18, 0x66, 0x3c, 0x18, 0x18, 0x18, 0x3c, 0x00}, // 8d
    { 0x66, 0x00, 0x3c, 0x18, 0x18, 0x18, 0x3c, 0x00}, // 8e
    { 0x1e, 0x36, 0x66, 0x6f, 0x66, 0x36, 0x1e, 0x00}, // 8f
    { 0x8e, 0x73, 0x67, 0x6f, 0x7b, 0x73, 0x63, 0x00}, // 90
    { 0x0c, 0x18, 0x3c, 0x66, 0x66, 0x66, 0x3c, 0x00}, // 91
    { 0x30, 0x18, 0x3c, 0x66, 0x66, 0x66, 0x3c, 0x00}, // 92
    { 0x18, 0x66, 0x3c, 0x66, 0x66, 0x66, 0x3c, 0x00}, // 93
    { 0x8e, 0x71, 0x3c, 0x66, 0x66, 0x66, 0x3c, 0x00}, // 94
    { 0x00, 0x66, 0x3c, 0x66, 0x66, 0x66, 0x3c, 0x00}, // 95
    { 0x00, 0x63, 0x36, 0x1c, 0x36, 0x63, 0x00, 0x00}, // 96
    { 0xfc, 0x66, 0x76, 0x7e, 0x6e, 0x66, 0x3f, 0x00}, // 97
    { 0x0c, 0x18, 0x66, 0x66, 0x66, 0x66, 0x3c, 0x00}, // 98
    { 0x30, 0x18, 0x66, 0x66, 0x66, 0x66, 0x3c, 0x00}, // 99
    { 0x18, 0x24, 0x66, 0x66, 0x66, 0x66, 0x3c, 0x00}, // 9a
    { 0x66, 0x00, 0x66, 0x66, 0x66, 0x66, 0x3c, 0x00}, // 9b
    { 0x60, 0x10, 0xc3, 0x66, 0x3c, 0x18, 0x18, 0x00}, // 9c
    { 0x03, 0x03, 0x3f, 0x63, 0x3f, 0x03, 0x03, 0x00}, // 9d
    { 0x00, 0x3c, 0x66, 0x3e, 0x66, 0x66, 0x3e, 0x06}, // 9e
    { 0x0c, 0x18, 0x3c, 0x60, 0x7c, 0x66, 0x7c, 0x00}, // 9f
    { 0x30, 0x18, 0x3c, 0x60, 0x7c, 0x66, 0x7c, 0x00}, // a0
    { 0x18, 0x66, 0x3c, 0x60, 0x7c, 0x66, 0x7c, 0x00}, // a1
    { 0x8e, 0x71, 0x3c, 0x60, 0x7c, 0x66, 0x7c, 0x00}, // a2
    { 0x66, 0x00, 0x3c, 0x60, 0x7c, 0x66, 0x7c, 0x00}, // a3
    { 0x18, 0x24, 0x3c, 0x60, 0x7c, 0x66, 0x7c, 0x00}, // a4
    { 0x00, 0x00, 0x7e, 0xd8, 0xfe, 0x1b, 0xee, 0x00}, // a5
    { 0x00, 0x00, 0x3c, 0x06, 0x06, 0x06, 0x3c, 0x18}, // a6
    { 0x0c, 0x18, 0x3c, 0x66, 0x7e, 0x06, 0x3c, 0x00}, // a7
    { 0x30, 0x18, 0x3c, 0x66, 0x7e, 0x06, 0x3c, 0x00}, // a8
    { 0x18, 0x66, 0x3c, 0x66, 0x7e, 0x06, 0x3c, 0x00}, // a9
    { 0x66, 0x00, 0x3c, 0x66, 0x7e, 0x06, 0x3c, 0x00}, // aa
    { 0x0c, 0x18, 0x00, 0x18, 0x18, 0x18, 0x30, 0x00}, // ab
    { 0x30, 0x18, 0x00, 0x18, 0x18, 0x18, 0x30, 0x00}, // ac
    { 0x18, 0x66, 0x00, 0x18, 0x18, 0x18, 0x30, 0x00}, // ad
    { 0x00, 0x66, 0x00, 0x18, 0x18, 0x18, 0x30, 0x00}, // ae
    { 0x06, 0x3f, 0x18, 0x3c, 0x66, 0x66, 0x3c, 0x00}, // af
    { 0x8e, 0x71, 0x00, 0x3e, 0x66, 0x66, 0x66, 0x00}, // b0
    { 0x0c, 0x18, 0x00, 0x3c, 0x66, 0x66, 0x3c, 0x00}, // b1
    { 0x30, 0x18, 0x00, 0x3c, 0x66, 0x66, 0x3c, 0x00}, // b2
    { 0x18, 0x66, 0x00, 0x3c, 0x66, 0x66, 0x3c, 0x00}, // b3
    { 0x8e, 0x71, 0x00, 0x3c, 0x66, 0x66, 0x3c, 0x00}, // b4
    { 0x00, 0x66, 0x00, 0x3c, 0x66, 0x66, 0x3c, 0x00}, // b5
    { 0x00, 0x18, 0x00, 0x7e, 0x00, 0x18, 0x00, 0x00}, // b6
    { 0x00, 0x40, 0x3e, 0x73, 0x6b, 0x67, 0x3e, 0x01}, // b7
    { 0x0c, 0x18, 0x00, 0x66, 0x66, 0x66, 0x7c, 0x00}, // b8
    { 0x30, 0x18, 0x00, 0x66, 0x66, 0x66, 0x7c, 0x00}, // b9
    { 0x18, 0x66, 0x00, 0x66, 0x66, 0x66, 0x7c, 0x00}, // ba
    { 0x00, 0x66, 0x00, 0x66, 0x66, 0x66, 0x7c, 0x00}, // bb
    { 0x30, 0x18, 0x00, 0x66, 0x66, 0x3c, 0x18, 0x0c}, // bc
    { 0x06, 0x06, 0x3e, 0x66, 0x66, 0x3e, 0x06, 0x06}, // bd
    { 0x00, 0x66, 0x00, 0x66, 0x66, 0x3c, 0x18, 0x0c}, // be
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // bf
};