#include <redlib.h>
#include <Kernel/framebuffer.h>
#include <Kernel/font.h>

/*
-> manages the rendering of Bitmaps;
-> It assumes depth = 16, so make sure that it is, or else it won't work;
-> It also uses the default Font Bitmap;
-> for now it's a little hardwritten, as its only task is to render 8x16 characters, but i'll work on making it more standard;
*/

void OPTIMIZE(3) renderBitmap(uint16 foreground, uint16 background, char character, uint8 bitmap_width, uint8 bitmap_height, uint16 bitmap_x, uint16 bitmap_y) {
    uint32 *screen = (uint32 *)Framebuffer.pointer;
    uint32 x = bitmap_x * (bitmap_width / 2);
    uint32 y = bitmap_y * bitmap_height;

    for (size_t row = 0; row < bitmap_height; row++) {
        uint8 characterRow = ConsoleFont[character][row];
        uint32 linearOffset = (y + row) * (Framebuffer.pitch >> 2);

        screen[(x + 0) + linearOffset] =
            (background ^ (-((characterRow >> 7) & 1U ) & (foreground ^ background))) |
            ((background ^ (-((characterRow >> 6) & 1U ) & (foreground ^ background))) << CHARACTER_HEIGHT);

        screen[(x + 1) + linearOffset] =
            (background ^ (-((characterRow >> 5) & 1U ) & (foreground ^ background))) |
            ((background ^ (-((characterRow >> 4) & 1U ) & (foreground ^ background))) << CHARACTER_HEIGHT);

        screen[(x + 2) + linearOffset] =
            (background ^ (-((characterRow >> 3) & 1U ) & (foreground ^ background))) |
            ((background ^ (-((characterRow >> 2) & 1U ) & (foreground ^ background))) << CHARACTER_HEIGHT);

        screen[(x + 3) + linearOffset] =
            (background ^ (-((characterRow >> 1) & 1U ) & (foreground ^ background))) |
            ((background ^ (-((characterRow >> 0) & 1U ) & (foreground ^ background))) << CHARACTER_HEIGHT);
    }
}

// ---------------------------------------------- //

