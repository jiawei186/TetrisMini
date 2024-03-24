#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses/ncurses.h>
#include "TetrisMini.h"

int tetrisOn;
int tetrisScore;

WINDOW *winTetris;

char currentTable[TETRISROW][TETRISCOL] = {0};

typedef struct
{
    char **blockDot;
    int width;
    int row, col;
} BLOCK;

BLOCK currentBlock;

const BLOCK blockSet[BLOCKNUM] = 
{
    {(char *[]){(char []){0, 1, 1}, (char []){1, 1, 0}, (char []){0, 0, 0}}, 3}, // S
    {(char *[]){(char []){1, 1, 0}, (char []){0, 1, 1}, (char []){0, 0, 0}}, 3}, // Z

    {(char *[]){(char []){0, 1, 0}, (char []){1, 1, 1}, (char []){0, 0, 0}}, 3}, // T
    {(char *[]){(char []){0, 0, 1}, (char []){1, 1, 1}, (char []){0, 0, 0}}, 3}, // L
    {(char *[]){(char []){1, 0, 0}, (char []){1, 1, 1}, (char []){0, 0, 0}}, 3}, // Flipped L

    {(char *[]){(char []){1, 1}, (char []){1, 1}}, 2}, // Square
    {(char *[]){(char []){1, 1, 1, 1}, (char []){0, 0, 0, 0}, (char []){0, 0, 0, 0}, (char []){0, 0, 0, 0}}, 4}, // Long bar
};

const char *gameInf[] =
{
    "Tetris Mini", 
    " ", 
    "Rows: ", 
    "Columns: ", 
    " ", 
    "Rotate: w", 
    "Move left: a", 
    "Move right: d", 
    "Move down: s", 
    " ", 
    "Exit: q", 
    " ", 
    "Score: ", 
};

BLOCK blockCopy(BLOCK bl)
{
    BLOCK newBlock = bl;
    newBlock.blockDot = (char **) malloc (newBlock.width * sizeof(char *));

    int i, j;
    for (i = 0; i < newBlock.width; i++)
    {
        newBlock.blockDot[i] = (char *) malloc (newBlock.width * sizeof(char));

        for (j = 0; j < newBlock.width; j++)
        {
            newBlock.blockDot[i][j] = bl.blockDot[i][j];
        }
    }

    return newBlock;
}

void blockDotDelete(BLOCK bl)
{
    int i;
    for (i = 0; i < bl.width; i++)
    {
        free(bl.blockDot[i]);
    }

    free(bl.blockDot);
}

int blockPositionCheck(BLOCK bl)
{
    int i, j;
    for (i = 0; i < bl.width; i++)
    {
        for (j = 0; j < bl.width; j++)
        {
            if ((bl.row + i >= TETRISROW || bl.col + j >= TETRISCOL || bl.col + j < 0))
            {
                if (bl.blockDot[i][j])
                {
                    return TETRISFALSE;
                }
            }
            else if (currentTable[bl.row + i][bl.col + j] && bl.blockDot[i][j])
            {
                return TETRISFALSE;
            }
        }
    }

    return TETRISTRUE;
}

void tableWrite()
{
    int i, j;
    for (i = 0; i < currentBlock.width; i++)
    {
        for (j = 0; j < currentBlock.width; j++)
        {
            if (currentBlock.blockDot[i][j])
            {
                currentTable[currentBlock.row + i][currentBlock.col + j] = 1;
            }
        }
    }
}

void rowRemove()
{
    int i, j, k, sum;
    int tetrisCount = 0;

    for (i = 0; i < TETRISROW; i++)
    {
        sum = 0;

        for (j = 0; j < TETRISCOL; j++)
        {
            sum += currentTable[i][j];
        }
        
        if (sum == TETRISCOL)
        {
            tetrisCount++;

            for (k = i; k > 0; k--)
            {
                for (j = 0; j < TETRISCOL; j++)
                {
                    currentTable[k][j] = currentTable[k - 1][j];
                }
            }
            
            for (j = 0; j < TETRISCOL; j++)
            {
                currentTable[0][j] = 0;
            }
        }
    }

    tetrisScore += tetrisCount * 100;
}

void tableBlockPrint()
{
    int gameInfVar[GAMEINFNUM] = {0};

    gameInfVar[2] = TETRISROW;
    gameInfVar[3] = TETRISCOL;
    gameInfVar[GAMEINFNUM - 1] = tetrisScore;

    char buffer[TETRISROW][TETRISCOL] = {0};

    int i, j;
    for (i = 0; i < currentBlock.width; i++)
    {
        for (j = 0; j < currentBlock.width; j++)
        {
            if (currentBlock.blockDot[i][j])
            {
                buffer[currentBlock.row + i][currentBlock.col + j] = 1;
            }
        }
    }

    clear();

    printw("\n");

    for (i = 0; i < TETRISROW; i++)
    {
        for (j = 0; j < TETRISCOL; j++)
        {
            printw("%c ", (currentTable[i][j] + buffer[i][j]) ? '#' : '.');
        }
        
        if (i < GAMEINFNUM)
        {
            if (gameInfVar[i] == 0)
            {
                printw("%s", gameInf[i]);

                if (i == (GAMEINFNUM - 1))
                {
                    printw("%d", gameInfVar[i]);
                }
            }
            else
            {
                printw("%s%d", gameInf[i], gameInfVar[i]);
            }
        }
        
        printw("\n");
    }
}

void randomBlock()
{
    BLOCK newBlock = blockCopy(blockSet[rand() % 7]);

    newBlock.row = 0;
    newBlock.col = rand() % (TETRISCOL - newBlock.width + 1);

    blockDotDelete(currentBlock);
    currentBlock = newBlock;

    if (!blockPositionCheck(currentBlock))
    {
        tetrisOn = TETRISFALSE;
    }
}

void blockRotate(BLOCK bl)
{
    BLOCK newBlock = blockCopy(bl);

    int i, j, k;

    for (i = 0; i < bl.width; i++)
    {
        for (j = 0, k = newBlock.width - 1; j < bl.width; j++, k--)
        {
            bl.blockDot[i][j] = newBlock.blockDot[k][i];
        }
    }

    blockDotDelete(newBlock);
}

void blockChange(int inputTetris)
{
    BLOCK temp = blockCopy(currentBlock);

    switch (inputTetris)
    {
        case 's':

            temp.row++;

            if (blockPositionCheck(temp))
            {
                currentBlock.row++;
            }
            else
            {
                tableWrite();
                rowRemove();
                randomBlock();
            }
            
            break;

        case 'd':

            temp.col++;

            if (blockPositionCheck(temp))
            {
                currentBlock.col++;
            }

            break;

        case 'a':

            temp.col--;

            if (blockPositionCheck(temp))
            {
                currentBlock.col--;
            }

            break;
            
        case 'w':

            blockRotate(temp);

            if (blockPositionCheck(temp))
            {
                blockRotate(currentBlock);
            }
            
            break;

        default:
            break;
    }

    blockDotDelete(temp);

    tableBlockPrint();
}

int gameCheck()
{
    if (tetrisOn)
    {
        return TETRISTRUE;
    }
    else
    {
        return TETRISFALSE;
    }
}

int gameInputCatch()
{
    int inputTetris = getch();

    if (inputTetris != ERR)
    {
        return inputTetris;
    }
    else
    {
        return TETRISFALSE;
    }
}

void tableInit()
{
    tetrisOn = TETRISTRUE;
    tetrisScore = 0;

    winTetris = initscr();
    timeout(1);

    srand(time(0));
    randomBlock();

    tableBlockPrint();
}

void tableClose()
{
    nodelay(winTetris, false);
    nocbreak();

    tableBlockPrint();
    printw("Press Enter to close the game\n");
    getch();

    blockDotDelete(currentBlock);
    
    endwin();
}
