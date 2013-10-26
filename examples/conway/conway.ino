/*
  Conway's Game of Life
*/

#include <avr/pgmspace.h>
#include <TVout.h>
#include <video_gen.h>
#include <EEPROM.h>
#include <Controllers.h>

#include <fontALL.h>

// #include "conway.h"

#define MATRIX_ROWS 64
#define MATRIX_COLUMNS 48

#define W 128
#define H 96

#define SIZE_X W/MATRIX_ROWS
#define SIZE_Y H/MATRIX_COLUMNS

#define ALIVE 1
#define DEAD 0

bool matrix[MATRIX_ROWS][MATRIX_COLUMNS];
bool matrix_buffer[MATRIX_ROWS][MATRIX_COLUMNS];

void initVars();

TVout tv;

// Allow the overall speed of the game to be adjusted.
// Higher number (like 1.5) slow the game down.  Lower numbers (like 0.6) speed it up.
float speedAdjust = 1;

void initGame(bool start)
{
  tv.fill(0);
  tv.select_font(font4x6);

  initVars();
}

void initVars()
{
  // Random numbers inside the matrix
  for(int i=0; i < MATRIX_ROWS; i++)
    for(int j=0; j < MATRIX_COLUMNS; j++)
      matrix[i][j] = random(0, 10) == 0;
}

void nextGeneration()
{
  int pos[8][2] ={ {-1, 1}, {0, 1}, {1, 1},
                   {-1, 0},         {1, 0},
                   {-1,-1}, {0, -1},{1,-1}
                 };

  for(int i=0; i < MATRIX_ROWS; i++)
    for(int j=0; j < MATRIX_COLUMNS; j++)
    {
      int n_sides = 0;
      for(int k=0; k < 8; k++)
      {
        int xk = i + pos[k][0], yk = j + pos[k][1];

        if ((xk > 0) && (xk < MATRIX_ROWS) && (yk > 0) && (yk < MATRIX_COLUMNS))
          n_sides += matrix[xk][yk];
      }

      if (matrix[i][j] == DEAD && n_sides == 3)
      {
        matrix_buffer[i][j] = ALIVE;
      }
      else if (matrix[i][j] == ALIVE && (n_sides < 2  || n_sides > 3))
      {
        matrix_buffer[i][j] = DEAD;
      }
    }

    for(int i=0; i < MATRIX_ROWS; i++)
      for(int j=0; j < MATRIX_COLUMNS; j++)
        matrix[i][j] = matrix_buffer[i][j];
}

void setup() {
  // If pin 12 is pulled LOW, then the PAL jumper is shorted.
  pinMode(12, INPUT);
  digitalWrite(12, HIGH);

  if (digitalRead(12) == LOW) {
    tv.begin(_PAL, W, H);
    // Since PAL processing is faster, we need to slow the game play down.
    speedAdjust = 1.4;
  } else {
    tv.begin(_NTSC, W, H);
  }

  randomSeed(analogRead(0));

  tv.delay_frame(30);

  initGame(false);
}

void drawMatrix()
{
  for(int i=0; i < MATRIX_ROWS; i++)
    for(int j=0; j < MATRIX_COLUMNS; j++)
      tv.draw_rect(i*SIZE_X, j*SIZE_Y, SIZE_X, SIZE_Y, matrix[i][j] == ALIVE, matrix[i][j] == ALIVE);
}

// void debugMatrix()
// {
//   for(int i=0; i < MATRIX_ROWS; i++)
//   {
//     for(int j=0; j < MATRIX_COLUMNS; j++)
//     {
//       printf("%d ", matrix[i][j]);
//     }
//     printf("\n");
//   }
// }

void loop()
{
  drawMatrix();
  nextGeneration();

  tv.delay_frame(10);
}
