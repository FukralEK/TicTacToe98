#include "game.h"

static GameContext ctx;

void NewGame(int difficulty)
{
	int i;
	for (i = 0; i < 9; i++)
	{
		ctx.table[i] = CELL_EMPTY;
	}
	ctx.difficulty = difficulty;
	ctx.currentPlayer = 0;

	srand(time(NULL));
}

char GetCell(int n)
{
	return ctx.table[n];
}

void TrySetCell(int n)
{
	if (ctx.table[n] != CELL_EMPTY)
	{
		return;
	}
	ctx.table[n] = ctx.currentPlayer ? CELL_O : CELL_X;
	if (ctx.difficulty == 0)
	{
		ctx.currentPlayer = !ctx.currentPlayer;
		return;
	}
	switch(ctx.difficulty)
	{
	case 1:
		AITurnEasy();
		break;
	case 2:
		AITurnMedium();
		break;
	case 3:
		AITurnHard();
		break;
	}
	
}
char CheckWin()
{
    int i;
    char *t = ctx.table;

    // Check rows
    for(i = 0; i < 3; i++)
    {
        if(t[i*3] != 0 && t[i*3] == t[i*3+1] && t[i*3+1] == t[i*3+2])
            return t[i*3];
    }

    // Check columns
    for(i = 0; i < 3; i++)
    {
        if(t[i] != 0 && t[i] == t[i+3] && t[i+3] == t[i+6])
            return t[i];
    }

    // Check diagonals
    if(t[0] != 0 && t[0] == t[4] && t[4] == t[8])
        return t[0];
    if(t[2] != 0 && t[2] == t[4] && t[4] == t[6])
        return t[2];

    // Check for draw
    for(i = 0; i < 9; i++)
        if(t[i] == 0)
            return WON_NOT_YET;

    return WON_DRAW;
}

void AITurnEasy()
{
	int i;
	while(1)
	{
		i = rand() % 9;
		
		if (ctx.table[i] == CELL_EMPTY)
		{
			ctx.table[i] = CELL_O;
			return;
		}
	}
}
void AITurnMedium()
{
	int i;
	while(1)
	{
		i = rand() % 9;
		
		if (ctx.table[i] == CELL_EMPTY)
		{
			ctx.table[i] = CELL_O;
			return;
		}
	}
}
void AITurnHard()
{
	int i;
	while(1)
	{
		i = rand() % 9;
		
		if (ctx.table[i] == CELL_EMPTY)
		{
			ctx.table[i] = CELL_O;
			return;
		}
	}
}