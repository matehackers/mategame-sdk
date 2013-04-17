/*
    Arduino compatible x86 mock
*/
#ifndef __MOCKVISION_ARDUINO__
#define __MOCKVISION_ARDUINO__

#include <stdlib.h>
#include <stdio.h>
#include <SDL_thread.h>

#define INPUT 0
#define OUTPUT 1

void pinMode(int p, int v);
void digitalWrite(int p, int v);
int digitalRead(int p);
int analogRead(int p);

void randomSeed(int seed) { srand(seed); }
int random(int n, int m) { return rand() % (m - n) + n; }

extern SDL_Thread *_graphics_thread;
extern bool _mockvision_running;

extern int _digitalPins[14];
extern int _analogPins[6];

static int TCCR2B;

#define HIGH 1024
#define LOW 0

void loop();
void setup();

#endif /* __MOCKVISION_ARDUINO__ */
