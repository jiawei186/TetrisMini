#define TETRISTRUE 1
#define TETRISFALSE 0

#define TETRISROW 22
#define TETRISCOL 17

#define BLOCKNUM 7
#define GAMEINFNUM 13

void tableInit();
void tableClose();

int gameCheck();
int gameInputCatch();

void blockChange(int inputTetris);
