#include <ngdevkit/neogeo.h>

#define MMAP_BACKDROP_COLOR_INDEX ((volatile u16*)0x401FFE)

#define BLACK 0x8000
#define WHITE 0x7FFF

const u16 palette[] = {
    // palette 0
    BLACK, WHITE, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,
    BLACK, BLACK, BLACK,
    // palette 1
    0x5f0f, 0x368, 0x2633, 0x4a33, 0x4d72, 0x656e, 0x7fff, 0x9033, 0xb69f, 0x8000, 0x8000, 0x8000,
    0x8000, 0x8000, 0x8000, 0x8000
};

#define PALETTE_SIZE (sizeof(palette) / sizeof(u16))

void init_palette() {
    for (u8 i = 0; i < PALETTE_SIZE; ++i) {
        MMAP_PALBANK1[i] = palette[i];
    }
}

void fix_clear() {
    u8 palette = 0;
    u16 tileValue = (palette << 12) | 0xFF;

    *REG_VRAMADDR = ADDR_FIXMAP;
    *REG_VRAMMOD = 1;

    for (u16 i = 0; i < 40 * 32; ++i) {
        *REG_VRAMRW = tileValue;
    }
}

void fix_print(u16 x, u16 y, const u8* text) {
    *REG_VRAMADDR = ADDR_FIXMAP + (x * 32) + y;

    *REG_VRAMMOD = 32;

    u8 palette = 0;

    while (*text) {
        u16 tileIndex = *text + 1;

        *REG_VRAMRW = (palette << 12) | tileIndex;

        text += 1;
    }
}

#define TO_SCREEN_Y(inputY) (-((inputY) + 496) - 32)
#define TO_SCREEN_X(inputX) (inputX)

#define FULL_SCALE 0xfff
#define HALF_SCALE 0x77f

// Address of Sprite Control Block in VRAM
#define ADDR_SCB1 0
#define ADDR_SCB2 0x8000
#define ADDR_SCB3 0x8200
#define ADDR_SCB4 0x8400

// how large, in words, the SCB2, SCB3 and SCB4 sections are (0x200)
#define SCB234_SIZE (ADDR_SCB4 - ADDR_SCB3)

// how many words one sprite entry in SCB1 takes up
#define SCB1_SPRITE_ENTRY_SIZE 64

void vram_sprite_scale_to_half() {
    const s16 x = 16;
    const s16 y = 32;
    const u8 height = 4;

    const s16 spriteIndex = 1;

    *REG_VRAMMOD = 1;
    *REG_VRAMADDR = ADDR_SCB1 + (spriteIndex)*SCB1_SPRITE_ENTRY_SIZE;

    for (u8 ty = 0; ty < height; ++ty) {
        *REG_VRAMRW = ty + 1;
        *REG_VRAMRW = (1 << 8);
    }

    *REG_VRAMMOD = SCB234_SIZE;
    *REG_VRAMADDR = ADDR_SCB2 + spriteIndex;

    // set scale (horizontal and vertical)
    *REG_VRAMRW = HALF_SCALE;

    // since this is scaled to half, divide height in half
    // to get a tighter window
    *REG_VRAMRW = (TO_SCREEN_Y(y) << 7) | height >> 1;
    // x position
    *REG_VRAMRW = TO_SCREEN_X(x) << 7;
}

void vram_32_sprite_scaled() {
    const s16 x = 32;
    const s16 y = 32;
    const u8 height = 32;

    const s16 spriteIndex = 2;

    *REG_VRAMMOD = 1;
    *REG_VRAMADDR = ADDR_SCB1 + (spriteIndex)*SCB1_SPRITE_ENTRY_SIZE;

    for (u8 ty = 0; ty < height; ++ty) {
        *REG_VRAMRW = (ty % 19) + 19;
        *REG_VRAMRW = (1 << 8);
    }

    *REG_VRAMMOD = SCB234_SIZE;
    *REG_VRAMADDR = ADDR_SCB2 + spriteIndex;

    // set scale (horizontal and vertical)
    *REG_VRAMRW = HALF_SCALE;

    // since this is scaled to half, divide height in half
    // to get a tighter window
    *REG_VRAMRW = (TO_SCREEN_Y(y) << 7) | height >> 1;
    // x position
    *REG_VRAMRW = TO_SCREEN_X(x) << 7;
}

int main() {
    init_palette();
    fix_clear();

    // a pale purple to let the tiles stand out more
    *MMAP_BACKDROP_COLOR_INDEX = 0xfcbd;

    fix_print(4, 4, "Sprite scaling test, gngeo");
    vram_sprite_scale_to_half();

    vram_32_sprite_scaled();

    for (;;) { }

    return 0;
}
