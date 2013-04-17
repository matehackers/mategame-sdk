/*
 Copyright (c) 2010 Myles Metzer

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following
 conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.
*/

/*
 This library provides a simple method for outputting data to a tv
 from a frame buffer stored in sram.  This implementation is done
 completly by interupt and will return give as much cpu time to the
 application as possible.
*/

#include <avr/pgmspace.h>

#include "TVout.h"
#include "video_gen.h"
#include <SDL/SDL.h>
#include <beeper.h>

PROGMEM const unsigned char ascii3x5[] ={
#include "ascii3x5.h"
};

PROGMEM const unsigned char ascii5x7[] = {
#include "ascii5x7.h"
};

PROGMEM const unsigned char ascii8x8[] = {
#include "ascii8x8.h"
};

PROGMEM const unsigned char bitmap[] = {
#include "bitmap.h"
};

/* call this to start video output with the default resolution.
 * returns 4 if not enough memory.
 */
char TVout::begin(uint8_t mode) {

    display.vscale = 1;
    display.vscale_const = 1;
    display.vres = 96;
    display.hres = 128/8;
    render_line = &render_line6c;

    display.screen = (unsigned char*)malloc(display.hres * display.vres * sizeof(unsigned char));
    if (display.screen == NULL)
        return 4;

    // Clear screen
    memset(display.screen, 0, display.hres * display.vres * sizeof(unsigned char));

    return 0;
}

/* call this to start video output with a specified resolution.
 * mode selects NTSC or PAL
 * x is the horizontal resolution MUST BE DIVISABLE BY 8 (max 248)
 * y is the vertical resolution (max 255)
 */
char TVout::begin(uint8_t mode, uint8_t x, uint8_t y) {

    display.hres = x/8;
    display.vres = y;

    display.screen = (unsigned char*)malloc(display.hres * display.vres * sizeof(unsigned char));

    if (display.screen == NULL)
        return 4;

    if ( display.hres >= 13 && display.hres <= 16 )
        render_line = &render_line6c;
    else if ( display.hres >= 17 && display.hres <= 19)
        render_line = &render_line5c;
    else
        return 5;

    // Clear screen
    memset(display.screen, 0, display.hres * display.vres * sizeof(unsigned char));

    return 0;
}

/* pauses rendering but keeps outputting a HSYNC this gives all CPU time to
 * user code
*/
void TVout::pause() {
    // TODO
}

/* Resume rendering the screen
*/
void TVout::resume() {
    // TODO
}

/* Clears the screen
*/
void TVout::fill(uint8_t color) {
    switch(color) {
        case 0:
            for (int i = 0; i < (display.hres)*display.vres; i++)
                display.screen[i] = 0;
            break;
        case 1:
            for (int i = 0; i < (display.hres)*display.vres; i++)
                display.screen[i] = 0xFF;
            break;
        case 2:
            for (int i = 0; i < display.hres*display.vres; i++)
                display.screen[i] = ~display.screen[i];
            break;
    }
}

/* Gets the Horizontal resolution of the screen
*/
unsigned char TVout::hres() {
    return display.hres*8;
}

/* Gets the Vertical resolution of the screen
*/
unsigned char TVout::vres() {
    return display.vres;
}

/* Return the number of characters that will fit on a line
*/
char TVout::char_line() {
    return ((display.hres*8)/font);
}

/* Delay for x frames
 * for NTSC 1 second = 60 frames
 * for PAL 1 second = 50 frames
 */
void TVout::delay(unsigned int x) {
    SDL_Delay(x*10);
}

/* plot one point 
 * at x,y with color 1=white 0=black 2=invert
 */
void TVout::set_pixel(uint8_t x, uint8_t y, char c) {
    if (x >= display.hres*8 || y >= display.vres)
        return;
    sp(x,y,c);
}

/* Returns the value of pixel (x,y)
 * 0 if pixel is black
 * !0 if pixel is white
 * Thank you gijs on the arduino.cc forum for the non obviouse fix.
*/
unsigned char TVout::get_pixel(uint8_t x, uint8_t y) {
    if (x >= display.hres*8 || y >= display.vres)
        return 0;
    return ((display.screen[(x/8) +(y*display.hres)] >> ((~x) & 7)) &0x01);
}

/* draw a line
 * x1,y1 to x2,y2
 * with color 1 = white, 0=black, 2=invert
 */
void TVout::draw_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, char c) {
    if (x0 > display.hres*8 || y0 > display.vres || x1 > display.hres*8 || y1 > display.vres)
        return;

    int e;
    signed int dx,dy,j, temp;
    signed char s1,s2, xchange;
    signed int x,y;

    x = x0;
    y = y0;

    //take absolute value
    if (x1 < x0) {
        dx = x0 - x1;
        s1 = -1;
    }
    else if (x1 == x0) {
        dx = 0;
        s1 = 0;
    }
    else {
        dx = x1 - x0;
        s1 = 1;
    }

    if (y1 < y0) {
        dy = y0 - y1;
        s2 = -1;
    }
    else if (y1 == y0) {
        dy = 0;
        s2 = 0;
    }
    else {
        dy = y1 - y0;
        s2 = 1;
    }

    xchange = 0;

    if (dy>dx) {
        temp = dx;
        dx = dy;
        dy = temp;
        xchange = 1;
    }

    e = ((int)dy<<1) - dx;

    for (j=0; j<=dx; j++) {
        sp(x,y,c);

        if (e>=0) {
            if (xchange==1) x = x + s1;
            else y = y + s2;
            e = e - ((int)dx<<1);
        }

        if (xchange==1)
            y = y + s2;
        else
            x = x + s1;

        e = e + ((int)dy<<1);
    }
}

/* draw a box
 * x1,y1 to x2,y2
 * with color 1 = white, 0=black, 2=invert
 * with fill 0 = black fill, 1 = white fill, 2 = invert fill, 3 = no fill
 * with radius for rounded box
 * safe draw or not 1 = safe
 * Added by Andy Crook 2010
 */
void TVout::draw_box(unsigned char x0, unsigned char y0,
               unsigned char x1, unsigned char y1, char c, char d, char e, char f)
{
    /* x0,y0 = start position from top left    x1,y1 = width and height
    *  c is box line colour, d is fill colour, e is radius for rounded rectangles (0 radius = straight box)
    *  if the radius is bigger than x0 or y0 then dont render, if the position + length + radius is bigger than the resolution then dont render
    */
    if (f == 0 ) { // if safe sp is off
        if (x0 >= display.hres*8 || y0 >= display.vres || x0 + x1 + e >= display.hres*8 || y0 + y1 + e >= display.vres)
            return;
    }

    unsigned char x,y;

    if (e == 0) { // ordinary box
        if (d < 3) { // fill the box with the appropriate colour if the fill is 0, 1 or 2. if 3 or higher, no fill
            for (y = y0+1; y < y0+y1-1; y++) {
                for (x = x0+1; x < x0+x1; x++)
                    sp_safe(x,y,d,f);
            }
        }
        // Now draw the box
        for (y = y0; y < y0+y1; y++) {
            sp_safe(x0,y,c,f);   // left hand line
            sp_safe(x0+x1,y,c,f); // right hand line
        }
        for (x = x0+1; x < x0 + x1; x++) {
            sp_safe(x,y0,c,f); // top line
            sp_safe(x,y0+y1-1,c,f); // bottom line
        }
    }
    if (e>0) {   // rounded rectangle! Split a circle into the 4 corners and draw a box
    // subtract the radius of the corners from the straight box we want to draw

        if (e+e > y1) // we cant draw a rounded box with a total radius bigger than the dimensions
            return;
        if (e+e > x1)
            return;

        x0=x0+e;
        y0=y0+e;
        y1=y1-e-e;
        x1=x1-e-e;
        // draw the box

        if (d < 3) {// fill the box with the appropriate colour if the fill is 0, 1 or 2. if 3 or higher, no fill
            for (y = y0-e; y < y0+y1+e; y++) {
                for (x = x0; x < x0+x1+1; x++)
                    sp_safe(x,y,d,f);
            }
            for (y = y0; y < y0+y1+1; y++) {
                for (x = x0-e; x < x0+1; x++)
                    sp_safe(x,y,d,f);
                for (x = x0+x1; x < x0+x1+e+1; x++)
                    sp_safe(x,y,d,f);
            }
        }

        for (y = y0; y < y0+y1; y++) {
            sp_safe(x0-e,y+1,c,f);        // First do the left hand line
            sp_safe(x0+x1+e,y+1,c,f);    // Now do the right hand line
        }

        for (x = x0+1; x < x0 + x1+1; x++) {
            sp_safe(x,y0-e,c,f);        // First do the top line
            sp_safe(x,y0+y1+e,c,f);        // Now do the bottom line
        }

        int radius = e;
        int ff = 1 - radius;
        int dx = 1;
        int dy = -2 * radius;
        int x = 0;
        int y = radius;
        int sty1 = 0;

        // draw  quarter circles at each corner of the box
        ff = 1 - radius;
        dx = 1;
        dy = -2 * radius;
        x = 0;
        y = radius;

        if (d<3) {
            // if filled, use endpoints to draw lines to center x
            for (sty1=0; sty1<radius; sty1++) {
                sp_safe(x0,y0+sty1+y1,d,f);
                sp_safe(x0,y0-sty1,d,f);
                sp_safe(x0+sty1+x1,y0,d,f);
                sp_safe(x0-sty1,y0,d,f);
            }
        }
        sp_safe(x0, y0 + radius+y1,c,f);
        sp_safe(x0, y0 - radius,c,f);
        sp_safe(x0 + radius+x1, y0,c,f);
        sp_safe(x0 - radius, y0,c,f);
        while(x < y) {
            if(ff >= 0) {
                y--;
                dy += 2;
                ff += dy;
            }
            x++;
            dx += 2;
            ff += dx;
            if (d<3) {
                // if filled, use endpoints to draw lines to center x
                for (sty1=0; sty1<y; sty1++) {
                    sp_safe(x0+x+x1,y0+sty1+y1,d,f);
                    sp_safe(x0-x,y0+sty1+y1,d,f);
                    sp_safe(x0+x+x1,y0-sty1,d,f);
                    sp_safe(x0-x,y0-sty1,d,f);
                    sp_safe(x0+sty1+x1,y0+x+y1,d,f);
                    sp_safe(x0-sty1,y0+x+y1,d,f);
                    sp_safe(x0+sty1+x1,y0-x,d,f);
                    sp_safe(x0-sty1,y0-x,d,f);
                }
            }
            sp_safe(x0 + x+x1, y0 + y+y1,c,f);
            sp_safe(x0 - x, y0 + y+y1,c,f);
            sp_safe(x0 + x+x1, y0 - y,c,f);
            sp_safe(x0 - x, y0 - y,c,f);
            sp_safe(x0 + y+x1, y0 + x+y1,c,f);
            sp_safe(x0 - y, y0 + x+y1,c,f);
            sp_safe(x0 + y+x1, y0 - x,c,f);
            sp_safe(x0 - y, y0 - x,c,f);
        }
        if (d<3) { // redraw circle
            // draw circle
            ff = 1 - radius;
            dx = 1;
            dy = -2 * radius;
            x = 0;
            y = radius;

            sp_safe(x0, y0 + radius+y1,c,f);
            sp_safe(x0, y0 - radius,c,f);
            sp_safe(x0 + radius+x1, y0,c,f);
            sp_safe(x0 - radius, y0,c,f);

            while(x < y) {

                if(ff >= 0) {
                    y--;
                    dy += 2;
                    ff += dy;
                }
                x++;
                dx += 2;
                ff += dx;
                sp_safe(x0 + x+x1, y0 + y+y1,c,f);
                sp_safe(x0 - x, y0 + y+y1,c,f);
                sp_safe(x0 + x+x1, y0 - y,c,f);
                sp_safe(x0 - x, y0 - y,c,f);
                sp_safe(x0 + y+x1, y0 + x+y1,c,f);
                sp_safe(x0 - y, y0 + x+y1,c,f);
                sp_safe(x0 + y+x1, y0 - x,c,f);
                sp_safe(x0 - y, y0 - x,c,f);
            }
        }
    }
}

/* draw a circle
 * x0,y0 around radius
 * with color 1 = white, 0=black, 2=invert
 * with fill 0 = black fill, 1 = white fill, 2 = invert fill, 3 = no fill
 * safe draw or not 1 = safe
 * Added by Andy Crook 2010
 */
void TVout::draw_circle(unsigned char x0, unsigned char y0,
                unsigned char radius, char c, char d, char h)
{
    if (h == 0) {
        if (x0+radius >= display.hres*8 || y0+radius >= display.vres || radius >= x0 || radius >= y0)
            return;
    }
    int f = 1 - radius;
    int dx = 1;
    int dy = -2 * radius;
    int x = 0;
    int y = radius;
    int sty1 = 0;

    // draw circle
    f = 1 - radius;
    dx = 1;
    dy = -2 * radius;
    x = 0;
    y = radius;

    if (d<3) { // if filled, use endpoints to draw lines to center x
        for (sty1=0; sty1<radius; sty1++) {
            sp_safe(x0,y0+sty1,d,h);
            sp_safe(x0,y0-sty1,d,h);
            sp_safe(x0+sty1,y0,d,h);
            sp_safe(x0-sty1,y0,d,h);
        }
    }
    sp_safe(x0, y0 + radius,c,h);
    sp_safe(x0, y0 - radius,c,h);
    sp_safe(x0 + radius, y0,c,h);
    sp_safe(x0 - radius, y0,c,h);
    while(x < y) {
        if(f >= 0) {
            y--;
            dy += 2;
            f += dy;
        }
        x++;
        dx += 2;
        f += dx;
        if (d<3) {
            // if filled, use endpoints to draw lines to center x
            for (sty1=0; sty1<y; sty1++) {
                sp_safe(x0+x,y0+sty1,d,h);
                sp_safe(x0-x,y0+sty1,d,h);
                sp_safe(x0+x,y0-sty1,d,h);
                sp_safe(x0-x,y0-sty1,d,h);
                sp_safe(x0+sty1,y0+x,d,h);
                sp_safe(x0-sty1,y0+x,d,h);
                sp_safe(x0+sty1,y0-x,d,h);
                sp_safe(x0-sty1,y0-x,d,h);
            }
        }
        sp_safe(x0 + x, y0 + y,c,h);
        sp_safe(x0 - x, y0 + y,c,h);
        sp_safe(x0 + x, y0 - y,c,h);
        sp_safe(x0 - x, y0 - y,c,h);
        sp_safe(x0 + y, y0 + x,c,h);
        sp_safe(x0 - y, y0 + x,c,h);
        sp_safe(x0 + y, y0 - x,c,h);
        sp_safe(x0 - y, y0 - x,c,h);
    }
    if (d<3) {  // redraw circle

    // draw circle
        f = 1 - radius;
        dx = 1;
        dy = -2 * radius;
        x = 0;
        y = radius;

        sp_safe(x0, y0 + radius,c,h);
        sp_safe(x0, y0 - radius,c,h);
        sp_safe(x0 + radius, y0,c,h);
        sp_safe(x0 - radius, y0,c,h);

        while(x < y) {
            if(f >= 0) {
                y--;
                dy += 2;
                f += dy;
            }
            x++;
            dx += 2;
            f += dx;
            sp_safe(x0 + x, y0 + y,c,h);
            sp_safe(x0 - x, y0 + y,c,h);
            sp_safe(x0 + x, y0 - y,c,h);
            sp_safe(x0 - x, y0 - y,c,h);
            sp_safe(x0 + y, y0 + x,c,h);
            sp_safe(x0 - y, y0 + x,c,h);
            sp_safe(x0 + y, y0 - x,c,h);
            sp_safe(x0 - y, y0 - x,c,h);
        }
    }
}


/* very simple bitmap placeing function, bitmap must be full screen and
 * called bitmap, this will be expanded.
 */
void TVout::fs_bitmap() {
    uint8_t byte;
    for (int i = 0; i < display.hres*display.vres; i++) {
        byte = pgm_read_byte((uint32_t)(bitmap) + i);
        display.screen[i] = byte;
        if (byte != display.screen[i]) {
            print_str(0,80,"Copy Err");
            while(1);
        }
    }
}

void TVout::select_font(uint8_t f) {
    font = f;
}

/*
 * print an 8x8 char c at x,y
 * x must be a multiple of 8
 */
void TVout::print_char(uint8_t x, uint8_t y, char c) {
    uint8_t y_pos;
    uint8_t j;

    if (font == _3X5) {

        // since this is not a complete font set fix the character
        if (c < 39)
            return;
        else if (c > 'z')
            c-=(26+39);
        else if (c > '`')
            c-=(97-65+39);
        else
            c-=39;

        uint8_t mask;
        if (x & 0x03) {
            if (x == (x & 0xF8))
                mask = 0x0F;
            else
                mask = 0xF0;
            for (char i = 0; i < 5; i++) {
                j = *(ascii3x5 + c*5 +i);
                display.screen[x+(y*display.hres)] = (display.screen[x+(y*display.hres)] & mask) | (j & ~mask);
                y++;
            }
        }
        else {

            for (char i=0;i<5;i++) {
                y_pos = y + i;

                j = *(ascii3x5 + c*5 + i);

                sp(x,   y_pos, (j & 0x80)==0x80);
                sp(x+1, y_pos, (j & 0x40)==0x40);
                sp(x+2, y_pos, (j & 0x20)==0x20);
                sp(x+3, y_pos, (j & 0x10)==0x10);
            }
        }
    }
    else if (font == _5X7) {
        
        // since there is nothing in the first 32 characters of the 5x7 font fix the offset c
        if (c < ' ')
            return;
        else
            c -= 32;
        for (char i=0;i<7;i++) {
            y_pos = y + i;

            j = *(ascii5x7 + c*7 + i);

            sp(x,   y_pos, (j & 0x80)==0x80);
            sp(x+1, y_pos, (j & 0x40)==0x40);
            sp(x+2, y_pos, (j & 0x20)==0x20);
            sp(x+3, y_pos, (j & 0x10)==0x10);
            sp(x+4, y_pos, (j & 0x08)==0x08);
        }
    }
    else if (font == _8X8) {
        if (x & 0x07) {
            for (char i=0;i<8;i++) {
                y_pos = y + i;

                j = *(ascii8x8 + c*8 + i);

                sp(x,   y_pos, (j & 0x80)==0x80);
                sp(x+1, y_pos, (j & 0x40)==0x40);
                sp(x+2, y_pos, (j & 0x20)==0x20);
                sp(x+3, y_pos, (j & 0x10)==0x10);
                sp(x+4, y_pos, (j & 0x08)==0x08);
                sp(x+5, y_pos, (j & 0x04)==0x04);
                sp(x+6, y_pos, (j & 0x02)==0x02);
                sp(x+7, y_pos, (j & 0x01)==0x01);
            }
        }
        else {
            x = x/8;
            for (char i = 0; i < 8; i++) {
                j = pgm_read_byte(((uint32_t)(ascii8x8)) + c*8 + i);
                display.screen[x+y*display.hres] = j;
                y++;
            }
        }
    }
}

/* print a null terminated string at x,y
*/
void TVout::print_str(uint8_t x, uint8_t y, const char *str) {
    // printf("TRACE: %s(x: %2d, y: %2d, str: \"%s\")\n", __FUNCTION__, x, y, str);
    if (y >= display.vres)
        return;
    for (char i=0; str[i]!=0; i++) { 
        if (x > (display.hres*8+font))
            return;
        print_char(x,y,str[i]);
        x += font;
    }
}

void TVout::tone(unsigned int frequency) {
  tone(frequency, 100);
}

void TVout::tone(unsigned int frequency, unsigned long duration_ms) {
    _audio_beeper->beep(frequency, duration_ms);
}

void TVout::noTone() {
    // TODO
}

void TVout::set_vbi_hook(void (*func)()) {
}


void TVout::render_setup(uint8_t mode) {
}

/* Inline version of set_pixel that does not perform a bounds check
*/
void sp(uint8_t x, uint8_t y, char c) {
    if (c==1)
        display.screen[(x/8) + (y*display.hres)] |= 0x80 >> (x&7);
    else if (c==0)
        display.screen[(x/8) + (y*display.hres)] &= ~0x80 >> (x&7);
    else
        display.screen[(x/8) + (y*display.hres)] ^= 0x80 >> (x&7);
}

// Inline version of set_pixel that does perform a bounds check
// Added by Andy Crook 2010
void sp_safe(unsigned char x, unsigned char y, char c, char d) {
    if (d==0) { // if d is zero then just do without any bounds check
        if (c==1)
            display.screen[(x/8) + (y*display.hres)] |= 0x80 >> (x&7);
        else if (c==0)
            display.screen[(x/8) + (y*display.hres)] &= ~0x80 >> (x&7);
        else
            display.screen[(x/8) + (y*display.hres)] ^= 0x80 >> (x&7);
    }
    else {  // d is 1, so check every pixel for boundary
        if (x < display.hres*8 && y < display.vres) {  // if x is in bounds AND if y is in bounds then proceed with pixel write
            if (c==1)
                display.screen[(x/8) + (y*display.hres)] |= 0x80 >> (x&7);
            else if (c==0)
                display.screen[(x/8) + (y*display.hres)] &= ~0x80 >> (x&7);
            else
                display.screen[(x/8) + (y*display.hres)] ^= 0x80 >> (x&7);
        }
    }
}
