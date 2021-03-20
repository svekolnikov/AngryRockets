#include <stdio.h>
#include <stdlib.h>

#define R_MAX 2147483647
static long long unsigned int SEED = 0x1;

typedef struct {
    int pos_before;
    int boost_before;
    int r1;
    int r2;
    int pos_after;
    int boost_after;
}Player_t;

typedef struct Game_t{
    int round;
    int player;
    Player_t p_t[2];
}Game_t;

typedef enum{
    CELL_EMPTY,
    CELL_BOOST,
    CELL_BLOCK,
    CELL_PLAYER
}CellType;

void srnd(int seed);
int GetRandomNumber(int start, int end);
CellType PlaceToWay(CellType * p_way, int pos, CellType ctype);
void PrintStep(Game_t *p_info);

int main(void) {
    //*********************** Init *******************************
    int seed = 11;
    int cellsNumMin = 0;
    int cellsNumMax = 100;
    int cellsNum = 50;
    int blockNum = 15;
    int boostNum = 5;
    //scanf("%d %d %d %d",&seed,&cellsNum,&blockNum,&boostNum);
    //*********************** Validation *************************
    if (seed <= 0 || cellsNum > cellsNumMax || cellsNum < cellsNumMin || ((blockNum + boostNum) > cellsNum / 2))
        return -1;

    srnd(seed);

    Game_t game = {.round = 1, .player = 1, .p_t[0].pos_before = -1, .p_t[1].pos_before = -1};

    // Init way by CELL_EMPTY
    CellType *way = malloc (sizeof (CellType) * cellsNum);
    for (int i = 0; i < cellsNum; i++)
        way[i] = CELL_EMPTY;

	//*********************** Set random blocks *******************
	printf("BLOCK:%");
	int temp = blockNum;
	while (temp--)
        while(PlaceToWay(way, GetRandomNumber(1, cellsNum - 1), CELL_BLOCK) != CELL_BLOCK){};
	while(temp++<cellsNum)
        if(way[temp] == CELL_BLOCK) printf("%d ", temp);

    //*********************** Set random boosts *******************
	printf("\nBOOST:%");
    temp = boostNum;
	while (temp--)
        while(PlaceToWay(way, GetRandomNumber(1, cellsNum - 1), CELL_BOOST) != CELL_BOOST){};
	while(temp++<cellsNum)
        if(way[temp] == CELL_BOOST) printf("%d ", temp);
    printf("\n");
    //*************************************************************

    temp = 5;
    while(temp--)
    {
        PrintStep(&game);
    }


    while(1){}
	return 0;
}

//------------ functions ---------------
void srnd(int seed)
{
    SEED = seed;
}
int GetRandomNumber(int from, int to) {
    SEED = SEED * 16807 % R_MAX;
    return from + (int) SEED % (to - from + 1);
}
CellType PlaceToWay(CellType* way, int pos, CellType ctype)
{
    if (way[pos] == CELL_EMPTY)
    {
        way[pos] = ctype;
        return ctype;
    }
    else
        return way[pos];
}

void PrintStep(Game_t *i)
{
    int player = i->player;
    printf("%d", player);
    /*
    printf("[%d,%d] [%d,%d] [%d,%d] [%d,%d]\n",
           &i->round,
           &i->player,
           &i->p_t[i->player].pos_before,
           &i->p_t[i->player].boost_before,
           &i->p_t[i->player].r1,
           &i->p_t[i->player].r2,
           &i->p_t[i->player].pos_after,
           &i->p_t[i->player].boost_after); */
}






