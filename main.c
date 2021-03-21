#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//*********************** Defines and Global variables ************
#define R_MAX 2147483647
static long long unsigned int SEED = 0x1;
//*********************** Data types ******************************
typedef enum{
    CELL_PLAYER1 = 1,
    CELL_PLAYER2,
    CELL_EMPTY,
    CELL_BOOST,
    CELL_BLOCK
}CellType;

typedef struct {
    int pos_before;
    int boost_before;
    int r1;
    int r2;
    int pos_after;
    int boost_after;
    int boosts;
}Player_t;

typedef struct Game_t{
    int seed;
    int nMin;
    int nMax;
    int n;
    int blockNum;
    int boostNum;
    int round;
    CellType player;
    int hotspot;
    int *way;
    Player_t p_t[2];
}Game_t;

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
        .hotspot = 0,
        .p_t[0].pos_before = -1,
        .p_t[1].pos_before = -1
    };
    //scanf("%d %d %d %d",&seed,&cellsNum,&blockNum,&boostNum);

    srnd(game.seed);

    game.way = (int *)malloc(game.n * 2 * sizeof(int));
    memset(game.way, CELL_EMPTY, game.n*sizeof(*game.way));
    for (int r = 0; r <  game.n; r++)
    {
        *(game.way + r*2) = CELL_EMPTY; //here cells for players, blocks and boosts
        *(game.way + r*2 + 1) = 0;      //here quantity of visits exact cell
    }

    //for (int r = 0; r <  game.n; r++)
        //for (int c = 0; c < 2; c++)
         //printf("%d ", *(game.way + r*2 + c));

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
    {
        printf("BLOCK:%");
        featuresNum = g->blockNum;
    }
    else if(f == CELL_BOOST)
    {
        printf("\nBOOST:%");
        featuresNum = g ->boostNum;
    }
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
int FindPlayer(Game_t *g, CellType player)
{
    for(int i=0; i<g->n; i++)
    {
        if(g->way[i*2] == player)
            return i;
    }
    return -1;
}
void ResetPlayer(Game_t *g, CellType player)
{
    for(int i=0; i<g->n; i++)
    {
        if(g->way[i*2] == player)
        {
            g->way[i*2] = CELL_EMPTY;
        }
    }
}
void ReplacePlayers(Game_t *g)
{
    int pos1 = GetPosBefore(g,CELL_PLAYER1);
    int pos2 = GetPosBefore(g,CELL_PLAYER2);
    ResetPlayer(g,CELL_PLAYER1);
    ResetPlayer(g,CELL_PLAYER2);
    SetPosBefore(g,CELL_PLAYER1,pos2);
    SetPosBefore(g,CELL_PLAYER2,pos1);
}
void SetPosBefore(Game_t *g,CellType player, int pos)
{
    g->p_t[player-1].pos_before = pos;
}
int GetPosBefore(Game_t *g, CellType player)
{
    return g->p_t[player-1].pos_before;
}
void SetBoostsBefore(Game_t *g)
{

}
void PlacePlayerPosition(Game_t *g, int pos)
{

}
void DoStep(Game_t *g)
{
    //Possition and boosts before
    SetPosBefore(g, g->player, FindPlayer(g, g->player));
    SetBoostsBefore(g);

    ReplacePlayers(g);

    //g->p_t[g->player-1].boosts_before = g->way[g->p_t[g->player-1].pos_before*2 +1];


    if((g->p_t[g->player].r1 == g->p_t[g->player].r2) && g->p_t[g->player].r1 == 6)
    {
        ReplacePlayers(g);
    }
    if((g->p_t[g->player].r1 == g->p_t[g->player].r2) && g->p_t[g->player].r1 == 1)
    {
        ReplacePlayers(g);
    }
    int increment = g->p_t[g->player-1].r1 + g->p_t[g->player-1].r1 - 7;

    // pos1 = FindPlayer(g, CELL_PLAYER1);
    //int pos2 = FindPlayer(g, CELL_PLAYER2);

    //ResetPlayer(g);
    //g->way[(pos1+increment)*2] = CELL_PLAYER1;
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
void FindHotspot(Game_t *g)
{
    int maxCellScore = 0;
    int hotspot = -1;
    for (int r = 0; r <  g->n; r++)
    {
       printf("%d ", g->way[r*2+1]);
       if(g->way[r*2+1] > maxCellScore)
       {
           maxCellScore = g->way[r*2+1];
           hotspot = r;
       }
    }
    g->hotspot = hotspot;
}
void PrintStatistic(Game_t *g)
{
    int winner = 0;
    if(g->way[g->n-1] == CELL_PLAYER1) winner = 1;
    else if(g->way[g->n-1] == CELL_PLAYER2) winner = 2;
    printf("WINNER:%d\n",winner);

    FindHotspot(g);
    printf("HOTSPOT:%d",g->hotspot);
}

int main(void) {
    //*********************** Initialization ***********************
    Game_t game = Initialization();
    //*********************** Validation ***************************
    if (ValidateData(&game)) return -1;
	//*********************** Set features *************************
	PlaceFeatureToWay(&game,CELL_BLOCK);
    PlaceFeatureToWay(&game,CELL_BOOST);
    printf("\n");
    //************************ Game process ************************

    while(game.round<27)
    {
        SetTwoRandom(&game);//ok
        DoStep(&game);
        PrintStep(&game);//ok
        IncreaseRound(&game);//ok
        TogglePlayer(&game);//ok
    }
    //************************ Finish ******************************
    PrintStatistic(&game);

    while(1){}
	return 0;
}
















