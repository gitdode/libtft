/* 
 * File:   types.h
 * Author: torsten.roemer@luniks.net
 *
 * Created on 17. September 2023, 20:33
 */

#ifndef TFTTYPES_H
#define TFTTYPES_H

#include <stdbool.h>
#include <stdint.h>

/* Width, height and color space of bitmaps and glyphs */
typedef uint16_t    width_t;
typedef uint16_t    height_t;
typedef uint8_t     space_t;

/* Width * height * bytes per pixel */
typedef uint32_t    bytes_t;

/* X and Y coordinates of the display */
typedef uint16_t    x_t;
typedef uint16_t    y_t;

/* Char code (like UTF-8 code point) */
typedef uint8_t     code_t;

/* Number of glyphs of a font */
typedef uint8_t     length_t;

#endif /* TFTTYPES_H */

