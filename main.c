#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define R_MAX 2147483647
static long long unsigned int SEED = 0x1;

typedef enum{
    CELL_EMPTY,
    CELL_BOOST,
    CELL_BLOCK,
    CELL_PLAYER1,
    CELL_PLAYER2
}CellType;

typedef struct {
    int pos_before;
    int boost_before;
    int r1;
    int r2;
    int pos_after;
    int boost_after;
}Player_t;

typedef struct Game_t{
    int seed;
    int nMin;
    int nMax;
    int n;
    int blockNum;
    int boostNum;
    int round;
    int player;
    int hotspot;
    int *way;
    Player_t p_t[2];
}Game_t;

Game_t Initialization();
int ValidateData(Game_t *g);
void srnd(int seed);
int GetRandomNumber(int start, int end);
void IncreaseRound(Game_t *g);
void TogglePlayer(Game_t *g);
void SetTwoRandom(Game_t *g);
void PlaceFeatureToWay(Game_t *g, CellType f);
void PrintStep(Game_t *g);
void PrintStatistic(Game_t *g);

int main(void) {
    //*********************** Init *******************************
    Game_t game = Initialization();
    //*********************** Validation **************************
    if (ValidateData(&game))
        return -1;
	//*********************** Set random blocks ********************
	printf("BLOCK:%");
	PlaceFeatureToWay(&game,CELL_BLOCK);
    //*********************** Set random boosts ********************
	printf("\nBOOST:%");
    PlaceFeatureToWay(&game,CELL_BOOST);
    printf("\n");
    //************************ Game process ************************
    while(game.round != 27)
    {
        SetTwoRandom(&game);

        PrintStep(&game);
        IncreaseRound(&game);
        TogglePlayer(&game);
    }
    //************************ Finish ******************************
    game.way[game.n-1] = CELL_PLAYER2;
    PrintStatistic(&game);

    while(1){}
	return 0;
}

//************************ functions *******************************
Game_t Initialization()
{
    Game_t game = {
        .seed = 11,
        .nMin = 0,
        .nMax = 100,
        .n = 50,
        .blockNum = 15,
        .boostNum = 5,
        .round = 1,
        .player = 1,
        .hotspot = 3,
        .p_t[0].pos_before = -1,
        .p_t[1].pos_before = -1
    };
    //scanf("%d %d %d %d",&seed,&cellsNum,&blockNum,&boostNum);

    srnd(game.seed);

    game.way = malloc (sizeof (int) * game.n);
    memset(game.way, CELL_EMPTY, game.n*sizeof(*game.way));

    for(int i=0; i<game.n; i++)
    {
        printf("[%d,%d]\n", game.way[i],0);
    }
    return game;
}
int ValidateData(Game_t *g)
{
    if(g->seed <= 0   ||
       g->n > g->nMax ||
       g->n < g->nMin ||
       ((g->blockNum + g->boostNum) > g->n / 2)
    )
        return 1;

    return 0;
}
void srnd(int seed)
{
    SEED = seed;
}
int GetRandomNumber(int from, int to)
{
    SEED = SEED * 16807 % R_MAX;
    return from + (int) SEED % (to - from + 1);
}
void SetTwoRandom(Game_t *g)
{
    g->p_t[g->player-1].r1 = GetRandomNumber(1, 6);
    g->p_t[g->player-1].r2 = GetRandomNumber(1, 6);
}
void PlaceFeatureToWay(Game_t *g, CellType f)
{
    int featuresNum = 0;
    if(f == CELL_BLOCK)
        featuresNum = g->blockNum;
    else if(f == CELL_BOOST)
        featuresNum = g ->boostNum;

    for(int i=0; i<featuresNum; i++)
    {
        int r = 0;
        do
        {
            r = GetRandomNumber(1, g->n - 1);
        }
        while(g->way[r]!=CELL_EMPTY);
            g->way[r] = f;
    }

	for(int i=0; i<g->n; i++)
        if(g->way[i] == f) printf("%d ", i);
}
void PrintStep(Game_t *i)
{
    printf("[%d,%d] [%d,%d] [%d,%d] [%d,%d]\n",
           i->round,
           i->player,
           i->p_t[i->player-1].pos_before,
           i->p_t[i->player-1].boost_before,
           i->p_t[i->player-1].r1,
           i->p_t[i->player-1].r2,
           i->p_t[i->player-1].pos_after,
           i->p_t[i->player-1].boost_after);
}
void IncreaseRound(Game_t *g)
{
    g->round++;
}
void TogglePlayer(Game_t *g)
{
    g->player = 3 - g->player;
}
void PrintStatistic(Game_t *g)
{
    int winner = 0;
    if(g->way[g->n-1] == CELL_PLAYER1) winner = 1;
    else if(g->way[g->n-1] == CELL_PLAYER2) winner = 2;

    printf("WINNER:%d\n",winner);
    printf("HOTSPOT:%d",g->hotspot);
}

















