const uint8_t Tile[][10] PROGMEM = {//These should be 8x8
  
{0xBF, 0x00, 0xFB, 0xFB, 0xFB, 0x00, 0xBF, 0xBF},//Floor 0x20
{0xDF, 0x40, 0x2B, 0x13, 0x0B, 0x04, 0x03, 0x01},//slant wall 0x21

{0x00, 0x80, 0x40, 0xA0, 0xD0, 0x08, 0xB4, 0xBA}, //0x22
{0xFF, 0x55, 0x00, 0xFB, 0xFB, 0x00, 0xBF, 0xBF}, //0x23
{0x01, 0x03, 0x04, 0x0B, 0x1B, 0x20, 0x5F, 0x9F}, //0x24
{0xBB, 0x01, 0xFB, 0xF9, 0xFB, 0x01, 0xBB, 0xB9}, //0x25
{0xDF, 0x80, 0xDB, 0x9B, 0xDB, 0x80, 0xDF, 0x9F}, //0x26
{0xBB, 0x02, 0xF4, 0xE8, 0xD0, 0x20, 0x40, 0x80}, //0x27

{0x80, 0x20, 0x31, 0x47, 0x6F, 0xCF, 0xFE, 0xFE},//Fire1 0x28
{0x20, 0x21, 0x04, 0x8E, 0xE7, 0xFE, 0xFE, 0x7F},//Fire2 0x29
{0x01, 0x04, 0x8C, 0xE2, 0xF6, 0xF7, 0x7F, 0x7F}, //Fire3 0x2A
{0x04, 0x84, 0x20, 0x71, 0xE7, 0x6F, 0x7F, 0xFE},//Fire4 0x2B
{0xFF, 0x81, 0x91, 0xA5, 0xA1, 0x95, 0x81, 0xFF}, //smiley 0x2C
{0xFF, 0x81, 0x91, 0xA5, 0xA1, 0x95, 0x81, 0xFF}, //smiley 0x2D

{0x00, 0x00, 0x00, 0x80, 0xC0, 0x60, 0xB0, 0xB0}, // 0x2E
{0xD8, 0xD8, 0x6C, 0x6C, 0xEC, 0xEC, 0xCC, 0xEC}, // 0x2F
{0xEC, 0xEC, 0x6C, 0xEC, 0xCC, 0xEC, 0xEC, 0x00},// 0x30
 
{0x00, 0x3E, 0x63, 0xD5, 0xB6, 0x77, 0xF7, 0xD5}, // 0x31
{0x94, 0x94, 0x14, 0x14, 0xF7, 0xF7, 0xD5, 0xD5}, // 0x32
{0xF7, 0xF7, 0xF6, 0xF7, 0xF7, 0x97, 0xF7, 0x00}, // 0x33

{0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x06, 0x06}, // 0x34
{0x0D, 0x0D, 0x1B, 0x1B, 0x1B, 0x1B, 0x19, 0x1B}, // 0x35
{0x1B, 0x1A, 0x1B, 0x1B, 0x19, 0x1B, 0x1B, 0x00}, // 0x36

{0x00, 0x00, 0x00, 0x80, 0xC0, 0x60, 0x30, 0x30}, // 0x37
{0x18, 0x18, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C}, // 0x38
{0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x00}, // 0x39

{0x00, 0x3E, 0x63, 0xC1, 0x80, 0x00, 0x00, 0x00}, // 0x3A
{0xFF, 0x81, 0x91, 0xA5, 0xA1, 0x95, 0x81, 0xFF}, //smiley 0x3B
{0xFF, 0x81, 0x91, 0xA5, 0xA1, 0x95, 0x81, 0xFF}, //smiley 0x3C

{0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x06, 0x06}, // 0x3D
{0x0C, 0x0C, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18},// 0x3E
{0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00},// 0x3F





{0xFF, 0x81, 0xFF, 0x81, 0xFF, 0x81, 0xFF, 0x81}, //ladder 0x40


 {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, //Switch left 0x41
 {0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0xE0, 0x20}, //Switch left 0x42
 {0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0xF0, 0xF0}, //Switch left 0x43
 {0x68, 0x14, 0x0A, 0x05, 0x02, 0x02, 0x0F, 0x08}, //Switch left 0x44
  
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},//Switch left 0x45
{0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x07, 0x04}, //Switch left 0x46
{0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x0F, 0x0F}, //Switch left 0x47
{0x16, 0x28, 0x50, 0xA0, 0x40, 0x40, 0xF0, 0x10},//Switch left 0x48


{0xFF, 0x81, 0x91, 0xA5, 0xA1, 0x95, 0x81, 0xFF}, //smiley
{0xFF, 0x81, 0x91, 0xA5, 0xA1, 0x95, 0x81, 0xFF}, //smiley
{0xFF, 0x81, 0x91, 0xA5, 0xA1, 0x95, 0x81, 0xFF}, //smiley

{0xFF, 0x81, 0x91, 0xA5, 0xA1, 0x95, 0x81, 0xFF}, //smiley
{0xFF, 0x81, 0x91, 0xA5, 0xA1, 0x95, 0x81, 0xFF}, //smiley
{0xFF, 0x81, 0x91, 0xA5, 0xA1, 0x95, 0x81, 0xFF}, //smiley
{0xFF, 0x81, 0x91, 0xA5, 0xA1, 0x95, 0x81, 0xFF}, //smiley
{0xFF, 0x81, 0x91, 0xA5, 0xA1, 0x95, 0x81, 0xFF}, //smiley
{0xFF, 0x81, 0x91, 0xA5, 0xA1, 0x95, 0x81, 0xFF}, //smiley
{0xFF, 0x81, 0x91, 0xA5, 0xA1, 0x95, 0x81, 0xFF}, //smiley
{0xFF, 0x81, 0x91, 0xA5, 0xA1, 0x95, 0x81, 0xFF}, //smiley
{0xFF, 0x81, 0x91, 0xA5, 0xA1, 0x95, 0x81, 0xFF}, //smiley
{0xFF, 0x81, 0x91, 0xA5, 0xA1, 0x95, 0x81, 0xFF}, //smiley








{0xFF, 0xFF, 0xFF, 0xFF, 0x03, 0xC0, 0x03, 0xC0}



};

