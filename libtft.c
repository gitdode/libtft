/*
 * File:   libtft.c
 * Author: torsten.roemer@luniks.net
 *
 * Created on 6. November 2023, 18:45
 */

#include "libtft.h"

static width_t _displayWidth;
static height_t _displayHeight;
static bool _hflip;
static bool _vflip;
static bool _bgr;

/**
 * Sends the given command to the display.
 *
 * @param cmd
 */
static void displayCmd(uint8_t cmd) {
    _tftSetCmd();
    _tftTx(cmd);
}

/**
 * Sends the given data to the display.
 *
 * @param data
 */
static void displayData(uint8_t data) {
    _tftSetData();
    _tftTx(data);
}

/**
 * Sets horizontal and/or vertical flip.
 *
 * @param hflip
 * @param vflip
 */
static void madctl(bool hflip, bool vflip) {
    // Memory data access control
    uint8_t madctl = 0b00110110;
    if (_hflip) madctl |= (1 << 7);
    if (_vflip) madctl |= (1 << 6);
    if (_bgr) madctl |= (1 << 3);

    if (hflip) {
        // Row Address Order (MY)
        madctl ^= (1 << 7);
    }
    if (vflip) {
        // Column Address Order (MX)
        madctl ^= (1 << 6);
    }

    _tftSel();
    displayCmd(TFT_MADCTL);
    displayData(madctl);
    _tftDes();
}

/**
 * Sets the given column start and end address.
 *
 * @param xs start address
 * @param xe end address
 */
static void caset(x_t xs, x_t xe) {
    _tftSel();
    displayCmd(TFT_CASET);
    displayData(xs >> 8);
    displayData(xs);
    displayData(xe >> 8);
    displayData(xe);
    _tftDes();
}

/**
 * Sets the given row start and end address.
 *
 * @param ys start address
 * @param ye end address
 */
static void raset(y_t ys, y_t ye) {
    _tftSel();
    displayCmd(TFT_RASET);
    displayData(ys >> 8);
    displayData(ys);
    displayData(ye >> 8);
    displayData(ye);
    _tftDes();
}

/**
 * Converts the given 8 pixel in 1-Bit monochrome to 16-Bit RGB (5/6/5) color
 * stored in the given array of 16 bytes, with the given background and
 * foreground color.
 *
 * @param mono 8 pixel in 1-Bit monochrome
 * @param rgb 8 pixel in 16-Bit RGB (5/6/5) color
 * @param bg background color
 * @param fg foreground color
 */
static void mono1ToRGB16(uint8_t mono, uint8_t *rgb,
                         uint16_t bg, uint16_t fg) {
    for (uint8_t i = 0; i < 16; i++) {
        uint8_t _bg = i % 2 == 0 ? bg >> 8 : bg;
        uint8_t _fg = i % 2 == 0 ? fg >> 8 : fg;
        rgb[i] = (mono & (1 << ((15 - i) >> 1))) ? _fg : _bg;
    }
}

/*
 * Converts the given two pixel in 4-Bit greyscale to 16-Bit RGB (5/6/5) color
 * stored in the given array of four bytes.
 *
 * @param grey two pixel in 4-Bit greyscale
 * @param rgb two pixel in 16-Bit RGB (5/6/5) color
 */
static void grey4ToRGB16(uint8_t grey, uint8_t *rgb) {
    uint8_t grey4 = ((grey >> 4) & 1);
    uint8_t grey0 = ((grey >> 0) & 1);

    rgb[0] = (grey & 0xf0);
    rgb[0] |= (grey4 << 3);
    rgb[0] |= (grey >> 5);

    rgb[1] = ((grey & 0xf0) << 3);
    rgb[1] |= ((grey & 0xf0) >> 3);
    rgb[1] |= (grey4 << 6) | (grey4 << 5) | (grey4 << 0);

    rgb[2] = (grey << 4);
    rgb[2] |= (grey0 << 3);
    rgb[2] |= ((grey & 0x0f) >> 1);

    rgb[3] = (grey << 7);
    rgb[3] |= ((grey & 0x0f) << 1);
    rgb[3] |= (grey0 << 6) | (grey0 << 5) | (grey0 << 0);
}

void tftInit(width_t width, height_t height,
             bool hflip, bool vflip,
             bool bgr, bool invert) {
    _displayWidth = width;
    _displayHeight = height;
    _hflip = hflip;
    _vflip = vflip;
    _bgr = bgr;

    _tftDelay100();

    // Hardware reset
    _tftReset();

    // TODO necessary?
    // _delay_ms(10);

    // Software reset
    _tftSel();
    displayCmd(TFT_SWRESET);
    _tftDes();

    // TODO necessary?
    // _delay_ms(10);

    // Sleep out & booster on
    _tftSel();
    displayCmd(TFT_SLPOUT);
    _tftDes();

    // Partial off (Normal)
    _tftSel();
    displayCmd(TFT_NORON);
    _tftDes();

    // Display Inversion on/off
    _tftSel();
    displayCmd(invert ? TFT_INVON : TFT_INVOFF);
    _tftDes();

    // Interface pixel format
    _tftSel();
    displayCmd(TFT_COLMOD);
    displayData(0b00111101);
    _tftDes();

    // Display on
    _tftSel();
    displayCmd(TFT_DISPON);
    _tftDes();

    // Sleep in & booster off
    // _tftSel();
    // displayCmd(SLPIN);
    // _tftDes();

    // printString("Display init done\r\n");
}

void tftWriteStart(void) {
    // Memory write
    _tftSel();
    displayCmd(TFT_RAMWR);
    _tftSetData();
}

void tftWriteRestart(void) {
    _tftSel();
}

void tftWriteByte(uint8_t byte) {
    // Memory write
    _tftTx(byte);
}

void tftWriteEnd(void) {
    // Memory write
    _tftDes();
}

void tftFillArea(x_t x, x_t y,
                 width_t width, height_t height,
                 uint16_t color) {

    madctl(false, false);

    // X address start/end
    uint16_t xs = x;
    uint16_t xe = x + width - 1;
    caset(xs, xe);

    // Y address start/end
    uint16_t ys = y;
    uint16_t ye = y + height - 1;
    raset(ys, ye);

    tftWriteStart();

    bytes_t pixels = (bytes_t)width * (bytes_t)height;
    for (bytes_t i = 0; i < pixels; i++) {
        _tftTx(color >> 8);
        _tftTx(color);
    }

    tftWriteEnd();
}

void tftSetArea(x_t x, y_t y,
                width_t width, height_t height,
                bool hflip, bool vflip) {

    madctl(hflip, vflip);

    // X address start/end
    uint16_t xs = x;
    uint16_t xe = x + width - 1;
    if (hflip) {
        xs = _displayWidth - x - width;
        xe = _displayWidth - x - 1;
    }
    caset(xs, xe);

    // Y address start/end
    uint16_t ys = y;
    uint16_t ye = y + height - 1;
    if (vflip) {
        ys = _displayHeight - y - height;
        ye = _displayHeight - y - 1;
    }
    raset(ys, ye);
}

void tftSetFrame(uint16_t color) {
    tftFillArea(0, 0, _displayWidth, _displayHeight, color);
}

void tftWriteData(const __flash uint8_t *bitmap,
                  width_t width, height_t height,
                  space_t space, uint16_t bg, uint16_t fg) {
    tftWriteStart();
    switch (space) {
        case SPACE_MONO1: {
            bytes_t bytes = width * height / 8;
            for (uint16_t i = 0; i < bytes; i++) {
                uint8_t rgb[16];
                mono1ToRGB16(bitmap[i], rgb, bg, fg);
                for (uint8_t j = 0; j < 16; j++) {
                    _tftTx(rgb[j]);
                }
            }
        }; break;
        case SPACE_GREY4: {
            bytes_t bytes = width * height / 2;
            for (uint16_t i = 0; i < bytes; i++) {
                uint8_t rgb[4];
                grey4ToRGB16(bitmap[i], rgb);
                for (uint8_t j = 0; j < 4; j++) {
                    _tftTx(rgb[j]);
                }
            }
        }; break;
        default: {
            // SPACE_RGB16
            bytes_t bytes = width * height * 2;
            for (uint16_t i = 0; i < bytes; i++) {
                _tftTx(bitmap[i]);
            }
        }
    }
    tftWriteEnd();
}

width_t tftWriteBitmap(x_t x, y_t y, uint16_t index,
                       uint16_t bg, uint16_t fg) {
    const __flash Bitmap *bitmap = &bitmaps[index];
    tftSetArea(x, y, bitmap->width, bitmap->height, false, false);
    tftWriteData(bitmap->bitmap, bitmap->width, bitmap->height,
              bitmap->space, bg, fg);

    return bitmap->width;
}

width_t tftWriteGlyph(x_t x, y_t y, const __flash Font *font, code_t code,
                      uint16_t bg, uint16_t fg) {
    const __flash Glyph *glyph = getGlyphAddress(font, code);
    tftSetArea(x, y, glyph->width, font->height, false, false);
    tftWriteData(glyph->bitmap, glyph->width, font->height,
              font->space, bg, fg);

    return glyph->width;
}

width_t tftWriteString(x_t x, y_t y, const __flash Font *font, const char *string,
                       uint16_t bg, uint16_t fg) {
    width_t xorig = x;
    uint8_t offset = 0;
    for (; *string != '\0'; string++) {
        uint8_t c = (uint8_t) *string;
        if (c == 194) {
            // multibyte
        } else if (c == 195) {
            // multibyte, add 64 to get code point
            offset = 64;
        } else {
            code_t code = c + offset;
            x += tftWriteGlyph(x, y, font, code, bg, fg);
            offset = 0;
        }
    }

    return x - xorig;
}
