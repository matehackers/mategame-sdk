/*
    Original code from http://web.archive.org/web/20120313055436/http://www.dgames.org/beep-sound-with-sdl/
*/
#ifndef __BEEPER__
#define __BEEPER__

#include <queue>
#include <cmath>

struct BeepObject
{
    double freq;
    int samplesLeft;
};

class Beeper
{
private:
    double v;
    std::queue<BeepObject> beeps;

public:
    Beeper();
    ~Beeper();
    void beep(double freq, int duration);
    void generateSamples(Sint16 *stream, int length);
    void wait();
};

extern Beeper *_audio_beeper;

#endif