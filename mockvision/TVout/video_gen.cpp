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

// #include <avr/interrupt.h>
// #include <avr/io.h>

#include "video_gen.h"

#include <SDL.h>
#include <SDL_thread.h>
#include <beeper.h>

#define WAIT_FOR_FRAMES 1000/30.0

SDL_Surface *mock_screen;
SDL_Thread *_graphics_thread;
Beeper *_audio_beeper;

extern bool _mockvision_running;

TVout_vid display;

int draw_loop(void *unused)
{
    while(_mockvision_running)
    {
        SDL_Rect rect;
        int i, j;

        for (i = 0; i < display.hres*8; i++)
        {
            for (j = 0; j < display.vres; j++)
            {
                int x = i * SCREEN_W/(display.hres*8);
                int y = j * SCREEN_H/display.vres;

                Uint32 color;

                if ((display.screen[(i/8) + (j*display.hres)] >> ((~i) & 7)) &0x01)
                    color = SDL_MapRGB(mock_screen->format, 255, 255, 255);
                else
                    color = SDL_MapRGB(mock_screen->format, 0, 0, 0);

                rect.x = x;
                rect.y = y;
                rect.w = SCREEN_W/(display.hres*8);
                rect.h = SCREEN_H/display.vres;

                SDL_FillRect(mock_screen, &rect, color);
            }
        }

        SDL_Flip(mock_screen);

        SDL_Delay(WAIT_FOR_FRAMES);
    }

    printf("Exit graphics\n");
    return 0;
}

TVout_vid::TVout_vid() {
    int bpp = 0, flags = 0;

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    atexit(SDL_Quit);

    mock_screen = SDL_SetVideoMode(SCREEN_W, SCREEN_H, bpp, flags);
    SDL_WM_SetCaption("Mockvision", "");

    _graphics_thread = SDL_CreateThread(draw_loop, NULL);

    _audio_beeper = new Beeper();

    if (_graphics_thread == NULL) {
        fprintf(stderr, "Unable to create thread: %s\n", SDL_GetError());
        return;
    }
}

volatile long remainingToneVsyncs;

void empty() {
}

int renderLine;
void (*render_line)();			//remove me
void (*line_handler)();			//remove me
void (*hbi_hook)() = &empty;
void (*vbi_hook)() = &empty;

void render_setup(uint8_t mode, uint8_t x, uint8_t y, uint8_t *scrnptr) {
	display.screen = scrnptr;
	display.hres = x;
	display.vres = y;
	display.frames = 0;
}

void blank_line() {
}

void active_line() {
}

void vsync_line() {
}


void inline wait_until(uint8_t time) {
}

void render_line6c() {
}

void render_line5c() {
}

void render_line4c() {
}

void render_line3c() {
}
