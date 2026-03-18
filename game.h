#ifndef GAME_H
#define GAME_H

#include <time.h>
#include <stdlib.h>

#define CELL_EMPTY 0
#define CELL_X 1
#define CELL_O 2

#define WON_NOT_YET 0
#define WON_X 1
#define WON_Y 2
#define WON_DRAW 3

typedef struct {
	char table[9];
	int difficulty;
	int currentPlayer;
} GameContext;

void NewGame(int difficulty);
char GetCell(int n);
void TrySetCell(int n);
char CheckWin();

void AITurnEasy();
void AITurnMedium();
void AITurnHard();

#endif