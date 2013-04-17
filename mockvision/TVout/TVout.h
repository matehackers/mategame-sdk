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

Limitations:
-	Currently only works with NTSC. PAl support would require modifying
	the timings in video_gen.h as well as the OCR1A ISR to allow for a 16bit
	line counter.
-	each frame only consists of 256 scanlines vs 262 for the fake progressive
	signal this is supposed to generate (works fine on my tv).
- virtical sync does not match the specs at all.


current hardware setup:
Pin9: Sync line:  -->|--/\/\/\--o
				Diode 1Kohm   |
Pin8: Video line: -->|--/\/\/\--o---------------- RCA tip
				Diode 330ohm  |
								o--/--/\/\/\--o-- RCA GND
								Switch  75ohm |
											V
											GND
											
Audio connected to arduino pin 10, hard coded for now
*/
#ifndef TVOUT_H
#define TVOUT_H

#include <stdint.h>

// macros for readability when selecting mode.
#define _PAL					1
#define _NTSC					0

#define _3X5					4
#define	_5X7					6
#define _8X8					8

// combatibility macros
#define start_render(mode)			begin(mode)
#define pause_render()				pause()
#define resume_render()				resume()
#define	horz_res()					hres()
#define vert_res()					vres()
#define delay_frame(x)				delay(x)

// Macros for clearer usage
#define clear_screen()				fill(0)
#define invert(color)				fill(2)

class TVout {
public:
	uint8_t * screen;

	uint8_t audio_freq;
	uint8_t audio_dur;

	char begin(uint8_t mode);
	char begin(uint8_t mode, uint8_t x, uint8_t y);
	void pause();
	void resume();
	void fill(uint8_t color);
	unsigned char hres();
	unsigned char vres();
	char char_line();
	void delay(unsigned int x);

	//basic rendering functions
	void set_pixel(uint8_t x, uint8_t y, char c);
	unsigned char get_pixel(uint8_t x, uint8_t y);
	void draw_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, char c);
	void draw_box(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, char c, char d, char e, char f); 
	void draw_circle(uint8_t x0, uint8_t y0, uint8_t radius, char c, char d, char h);
	void fs_bitmap();

	//printing functions
	void select_font(uint8_t f);
	void print_char(uint8_t x, uint8_t y, char c);
	void print_str(uint8_t x, uint8_t y, const char *str);
	void tone(unsigned int frequency, unsigned long duration_ms);
	void tone(unsigned int frequency);
	void noTone();
	void set_vbi_hook(void (*func)());
private:
	uint8_t font;
	void render_setup(uint8_t mode);
};

void sp(unsigned char x, unsigned char y, char c);
void sp_safe(unsigned char x, unsigned char y, char c, char d); 
#endif
