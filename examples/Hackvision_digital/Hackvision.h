#ifndef __HACKVISION_H__
#define __HACKVISION_H__

/* Prototypes */

/* Space Invaders stuff */
void initVars();
boolean titleScreen();
boolean displayHighScores(byte file);
void drawBunkers();
void drawBunker(byte x, byte y);
void drawCannon(byte x, byte y, byte color);
void drawScoreLine();
void drawRemainingLives();

void advanceInvaders();
void drawLaser();
void drawBitmap(byte x, byte y, unsigned int bitmapIndex);
void drawInvaders();
void drawBitmap();
void dropBomb();
void drawBombs();

void handleMysteryShip();

void playTone(unsigned int frequency, unsigned long duration_ms);
void playTone(unsigned int frequency, unsigned long duration_ms, byte priority);


void gameOver();
void enterInitials();
void newLevel();
void tick();
boolean getInput();
void enterHighScore(byte file);

boolean pollFireButton(int n);

void damage(byte x, byte y);
void destroyCannon();

byte menu(byte nChoices, byte *choices);
void spaceInvaders();

boolean columnEmpty(byte c);
boolean rowEmpty(byte r);

void initSpaceInvaders(boolean start);

/* Pong stuff */
void initPong();
void hitSound();
void bounceSound();
void scoreSound();

void pong();

void moveBall();

void drawPaddles();
void  drawPaddle(int x, int y);
void drawNet();

#endif