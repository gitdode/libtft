# libtft

## About

Static avr-libc library providing basic support for TFT LCDs such as:

* [Adafruit 1.8" Color TFT LCD 160x128 ST7735R](https://www.adafruit.com/product/358)
* [Adafruit 2.0" Color IPS TFT 320x240 ST7789](https://www.adafruit.com/product/4311)
* [Adafruit 2.8" Color TFT LCD with Cap Touch 320x240 ILI9341](https://www.adafruit.com/product/2090)

Currently implemented features:

* Mostly complete UTF-8 set (code points U+0000 to U+00FF) of Unifont font (1-Bit monochrome)
* Mostly complete UTF-8 set (code points U+0000 to U+00FF) of Hack font with antialiasing (4-Bit greyscale)
* Write text, draw bitmaps

![IMG_20250711_234547](https://github.com/user-attachments/assets/8837384c-b532-4fae-8a88-431e301c79c6)

## Usage

1. Include `libtft.h`, `font.h`, `types.h` and `libtft.a` in the project
2. Include `hack.h` and/or `unifont.h`, depending on what font to use.
2. Implement the `_tft*` functions in `libtft.h` in the application
(this is to make the library CPU frequency independent)

## Fonts

To include a font, add i.e. `dejavu.h`:

```C
    #ifndef DEJAVU_H
    #define DEJAVU_H

    #include "font.h"

    /**
     * DejaVu font.
     */
    extern const __flash Font dejaVuFont;

    #endif /* DEJAVU_H */
```

and the matching implementation `dejavu.c`:

```C
    #include "dejavu.h"
    #include "utils.h"

    #define HEIGHT 56

    static const __flash uint8_t SPACE[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        ...
    }

    ...

    const __flash Glyph glyphs[] = {
        {0x0020, 24, SPACE},
        {0x0021, 16, EXCLAMATION_MARK},
        {0x0023, 32, NUMBER_SIGN},
        {0x0025, 32, PERCENT_SIGN},
        ...
    }

    const __flash Font dejaVuFont = {glyphs, array_length(glyphs), HEIGHT, SPACE_MONO1};
```

and write some text in DejaVu to the display:

```C
    const __flash Font *dejaVu = &dejaVuFont;
    tftWriteString(x, y, dejaVu, "123", WHITE, crc ? BLACK : RED);
```

## Bitmaps

To display bitmaps, add i.e. `bitmaps.c`:

```C
    #include "font.h"

    const __flash uint8_t TUX_DATA[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x03, 0xff, 0x80, 0x00, 0x00,
        0x00, 0x00, 0x07, 0xff, 0xc0, 0x00, 0x00,
        0x00, 0x00, 0x07, 0xff, 0xe0, 0x00, 0x00,
        ...
    }

    const __flash Bitmap bitmaps[] = {
        {56, 64, SPACE_MONO1, TUX_DATA}
    };
```

and write Tux to the display:

```C
    tftWriteBitmap(0, 176, 0, 0xffff, 0x0000);
```
