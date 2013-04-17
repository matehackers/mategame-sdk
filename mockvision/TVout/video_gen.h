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

#ifndef VIDEO_GEN_H
#define VIDEO_GEN_H

#include <inttypes.h>

#define     SCREEN_W    800
#define     SCREEN_H    600

class TVout_vid {

public:
	TVout_vid();

	volatile int scanLine;
	int start_render;
	int stop_render;
	int lines_frame;
	uint8_t vres;
	uint8_t hres;
	uint8_t output_delay;
	char vscale_const;
	char vscale;
	char vsync_end;
	uint8_t * screen;
};

struct SDL_Surface;
struct SDL_Thread;
struct Beeper;

extern TVout_vid display;
extern bool _mockvision_running;
extern SDL_Thread *_graphics_thread;

//tone generation properties
extern volatile long remainingToneVsyncs;

extern void (*render_line)();
extern void (*line_handler)();
extern void (*vbi_hook)();

void blank_line();
void active_line();
void vsync_line();

// 6cycles functions
void render_line6c();
void render_line5c();
void wait_until(uint8_t time);
#endif
