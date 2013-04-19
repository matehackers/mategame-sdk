#include <SDL.h>
#include <SDL_audio.h>
#include <Controllers.h>
#include <beeper.h>

/* It's kind of inevitable to use bytes as array indexes
   when dealing with this level of programming, so just
   disable all the millions of warnings about this
*/
#pragma GCC diagnostic ignored "-Wchar-subscripts"

int _digitalPins[14];
int _analogPins[6];

bool _mockvision_running = true;

SDL_Thread *_input_thread;
SDL_Thread *_audio_thread;

long _millis = 0;

/* Some functions to simulate Arduino inputs, I plan on having
   a GUI for visualizing and adjusting those */
void pinMode(int p, int v)
{
}

void digitalWrite(int p, int v)
{
    _digitalPins[p] = v;
}

int digitalRead(int p)
{
    return _digitalPins[p];
}

int analogRead(int p)
{
    return _analogPins[p];
}

/* Straight from the Arduino code */
void randomSeed(unsigned int seed)
{
  if (seed != 0) {
    srandom(seed);
  }
}

long random(long howbig)
{
  if (howbig == 0) {
    return 0;
  }
  return random() % howbig;
}

long random(long howsmall, long howbig)
{
  if (howsmall >= howbig) {
    return howsmall;
  }
  long diff = howbig - howsmall;
  return random(diff) + howsmall;
}

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

long millis()
{
    return _millis;
}

char* strcpy_P(char* c, char* d)
{
    strcpy(c, d);
    return c;
}

void arduino_quit()
{
    SDL_PauseAudio(0);
    SDL_CloseAudio();
    SDL_WaitThread(_input_thread, NULL);
    SDL_WaitThread(_graphics_thread, NULL);
    SDL_Quit();
}

void receive_input(SDLKey key, bool keydown)
{
    switch (key)
    {
        case SDLK_LEFT:
            Controller.buttons[ButtonController::LEFT] = keydown;
        break;

        case SDLK_RIGHT:
            Controller.buttons[ButtonController::RIGHT] = keydown;
        break;

        case SDLK_UP:
            Controller.buttons[ButtonController::UP] = keydown;
        break;

        case SDLK_DOWN:
            Controller.buttons[ButtonController::DOWN] = keydown;
        break;

        case SDLK_SPACE:
            Controller.buttons[ButtonController::FIRE] = keydown;
        break;

        default:
        break;
    }
}

int input_loop(void *unused)
{
    while(_mockvision_running)
    {
        SDL_Event event;
        if (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                _mockvision_running = false;
            }

            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    _mockvision_running = false;
                }
                receive_input(event.key.keysym.sym, true);
            }

            if (event.type == SDL_KEYUP)
            {
                receive_input(event.key.keysym.sym, false);
            }
        }

        // Receive input every 100ms
        SDL_Delay(100);
    }

    printf("Exit input\n");
    return 0;
}

int audio_loop(void *unused)
{
    while(_mockvision_running)
    {
        _audio_beeper->wait();
    }

    printf("Exit audio\n");
    return 0;
}

int main()
{
    _input_thread = SDL_CreateThread(input_loop, NULL);
    _audio_thread = SDL_CreateThread(audio_loop, NULL);

    unsigned long int cur_ticks = SDL_GetTicks();

    if (_input_thread == NULL || _audio_thread == NULL)
    {
        fprintf(stderr, "Unable to create thread: %s\n", SDL_GetError());
        return 1;
    }

    setup();

    while (_mockvision_running)
    {
        if ( (SDL_GetTicks() - cur_ticks) > 1)
        {
            cur_ticks = SDL_GetTicks();
            _millis = cur_ticks;
            loop();
        }
    }

    arduino_quit();

    return 0;
}
