#ifndef __INVADERS_H__
#define __INVADERS_H__

/* Prototypes */
void initVars();
bool titleScreen();
bool displayHighScores(byte file);
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

void playTone(unsigned int frequency, unsigned long duration_ms);
void playTone(unsigned int frequency, unsigned long duration_ms, byte priority);

void handleMysteryShip();

void gameOver();
void enterInitials();
void newLevel();
void tick();
bool getInput();
void enterHighScore(byte file);

bool pollFireButton(int n);

void damage(byte x, byte y);
void destroyCannon();

void spaceInvaders();

bool columnEmpty(byte c);
bool rowEmpty(byte r);

#endif