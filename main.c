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
    int rSum;
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
        .seed = 10,
        .n = 15,
        .blockNum = 3,
        .boostNum = 3,

        .nMin = 0,
        .nMax = 100,
        .round = 1,
        .player = 1,
        .hotspot = 0,
        .p_t[0].pos_before = -1,
        .p_t[1].pos_before = -1
    };
    scanf("%d %d %d %d",&game.seed,&game.n,&game.blockNum,&game.boostNum);

    srnd(game.seed);

    game.way = (int *)malloc(game.n * 2 * sizeof(int));
    memset(game.way, CELL_EMPTY, game.n*sizeof(*game.way));
    for (int r = 0; r <  game.n; r++)
    {
        *(game.way + r*2) = CELL_EMPTY; //here cells for players, blocks and boosts
        *(game.way + r*2 + 1) = 0;      //here quantity of visits exact cell
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

//*****************************************************************
void srnd(int seed)
{
    SEED = seed;
}
int GetRandomNumber(int from, int to)
{
    SEED = SEED * 16807 % R_MAX;
    return from + (int) SEED % (to - from + 1);
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
            r = GetRandomNumber(1, g->n-1);
        }
        while(g->way[r*2]!=CELL_EMPTY);

        g->way[r*2] = f;
    }

	for(int i=0; i<g->n; i++)
        if(g->way[i*2] == f) printf("%d ", i);
}

//*****************************************************************
void SetTwoRandom(Game_t *g)
{
    g->p_t[g->player-1].r1 = GetRandomNumber(1,6);
    g->p_t[g->player-1].r2 = GetRandomNumber(1,6);
    g->p_t[g->player-1].rSum = g->p_t[g->player-1].r1 + g->p_t[g->player-1].r2;
}
int GetR1(Game_t *g)
{
    return g->p_t[g->player-1].r1;
}
int GetR2(Game_t *g)
{
    return g->p_t[g->player-1].r2;
}
int GetRSum(Game_t *g)
{
    return g->p_t[g->player-1].rSum;
}
int GetMaxRandom(Game_t *g)
{
    if(GetR1(g) > GetR2(g))
        return GetR1(g);

    return GetR2(g);
}
int CompareR1R2With(Game_t *g, int num)
{
    if((GetR1(g) == GetR2(g)) && GetR1(g) == num)
        return 1;
    return 0;
}

//*****************************************************************
int FindPlayerOnWay(Game_t *g, CellType player)
{
    for(int i=0; i<g->n; i++)
    {
        if(g->way[i*2] == player)
            return i;
    }
    return -1;
}
void PlacePlayerToWayPosition(Game_t *g, CellType player, int pos)
{
    ResetPlayerOnWay(g,player);
    g->way[pos*2] = player;
    g->way[pos*2 +1]++;
}
int IsNextPlayerAhead(Game_t *g)
{
    int curPlayerPosition = FindPlayerOnWay(g,g->player);
    int nextPlayerPosition = FindPlayerOnWay(g,3 - g->player);
    if(nextPlayerPosition > curPlayerPosition)
        return 1;
    return 0;
}
void ResetPlayerOnWay(Game_t *g, CellType player)
{
    for(int i=0; i<g->n; i++)
    {
        if(g->way[i*2] == player)
        {
            g->way[i*2] = CELL_EMPTY;
        }
    }
}
int ReplacePlayersOnWay(Game_t *g)
{
    int wayPos1 = FindPlayerOnWay(g,CELL_PLAYER1);
    int wayPos2 = FindPlayerOnWay(g,CELL_PLAYER2);

    if(wayPos1 == -1 || wayPos2 == -1)
        return -1;

    ResetPlayerOnWay(g,CELL_PLAYER1);
    ResetPlayerOnWay(g,CELL_PLAYER2);

    PlacePlayerToWayPosition(g,CELL_PLAYER1,wayPos2);
    PlacePlayerToWayPosition(g,CELL_PLAYER2,wayPos1);
}

//*****************************************************************
void SetPosBefore(Game_t *g, CellType player, int pos)
{
    g->p_t[player-1].pos_before = pos;
}
int GetPosBefore(Game_t *g, CellType player)
{
    return g->p_t[player-1].pos_before;
}
void SetPosAfter(Game_t *g, CellType player, int pos)
{
    g->p_t[player-1].pos_after = pos;
}
int GetPosAfter(Game_t *g, CellType player)
{
    return g->p_t[player-1].pos_after;
}

//*****************************************************************
void SetBoostsBefore(Game_t *g,CellType player, int boosts)
{
    g->p_t[player-1].boost_before = boosts;
}
int GetBoostsBefore(Game_t *g,CellType player)
{
    return g->p_t[player-1].boost_before;
}
void SetBoostsAfter(Game_t *g,CellType player, int boosts)
{
    g->p_t[player-1].boost_after = boosts;
}
int GetBoostsAfter(Game_t *g,CellType player)
{
    return g->p_t[player-1].boost_after;
}

//*****************************************************************
CellType GetCell(Game_t *g, int pos)
{
    return g->way[pos*2];
}
void ClearCell(Game_t *g,int pos)
{
    g->way[pos*2] = CELL_EMPTY;
}
//*****************************************************************
void DoStep(Game_t *g)
{
    SetPosBefore(g, g->player, FindPlayerOnWay(g, g->player));
    SetBoostsBefore(g, g->player, GetBoostsAfter(g, g->player));

    CellType oppositePlayer = 3 - g->player;

    //Step size
    int d = 0;

    if(GetPosBefore(g,g->player) == -1) // Player outside
    {
        if( GetRSum(g) > 7)
        {
            d = GetRSum(g) - 7 + GetBoostsBefore(g, g->player);

            CellType nextCell = GetCell(g,d);

            if(nextCell == CELL_BLOCK)
            {
                SetBoostsAfter(g,g->player,0);
                ClearCell(g,d);
                ResetPlayerOnWay(g,g->player);
                SetPosAfter(g,g->player, FindPlayerOnWay(g,g->player));
                return;
            }
            else if(nextCell == CELL_BOOST)
                SetBoostsAfter(g,g->player,GetBoostsBefore(g,g->player)+1);
            else if(nextCell == oppositePlayer)
                ResetPlayerOnWay(g,oppositePlayer);

            PlacePlayerToWayPosition(g,g->player,d);
        }
    }
    else // Player on the field
    {
        if(((CompareR1R2With(g,6) == 1 && IsNextPlayerAhead(g) == 1) ||
           (CompareR1R2With(g,1) == 1 && IsNextPlayerAhead(g) == 0)) && FindPlayerOnWay(g,oppositePlayer) > -1)
        {
            ReplacePlayersOnWay(g);
            SetPosAfter(g,g->player, FindPlayerOnWay(g,g->player));
            return;
        }

        d = FindPlayerOnWay(g,g->player) + GetMaxRandom(g) + GetBoostsBefore(g, g->player);
        if(d > g->n-1)
        {
            SetPosAfter(g,g->player, d);
            return;
        }


        CellType nextCell = GetCell(g,d);
        if(nextCell == CELL_BLOCK)
        {
            if(GetBoostsBefore(g,g->player) == 0)
            {
                ResetPlayerOnWay(g,g->player);
                SetPosAfter(g,g->player, FindPlayerOnWay(g,g->player));
                ClearCell(g,d);
                return;
            }
            else
            {
                SetBoostsAfter(g,g->player,GetBoostsBefore(g,g->player)-1);
            }
        }
        else if(nextCell == CELL_BOOST)
            SetBoostsAfter(g,g->player,GetBoostsBefore(g,g->player)+1);
        else if(nextCell == oppositePlayer)
            ResetPlayerOnWay(g,oppositePlayer);

        PlacePlayerToWayPosition(g,g->player,d);
    }
    SetPosAfter(g,g->player, FindPlayerOnWay(g,g->player));
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
    int hotspot = 0;
    for (int r = 1; r <  g->n; r++)
    {
       if(g->way[r*2+1] > hotspot)
            hotspot = g->way[r*2+1];
    }
    g->hotspot = hotspot;
}
void PrintStatistic(Game_t *g)
{
    int winner = 0;
    if(g->p_t[CELL_PLAYER1-1].pos_after>g->n-1)
        winner = CELL_PLAYER1;
    else if(g->p_t[CELL_PLAYER2-1].pos_after>g->n-1)
        winner = CELL_PLAYER2;

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
    while(1)
    {
        SetTwoRandom(&game);//ok
        DoStep(&game); //ok
        PrintStep(&game);//ok
        if(game.p_t[game.player-1].pos_after>game.n-1)break;
        IncreaseRound(&game);//ok
        TogglePlayer(&game);//ok
    }
    //************************ Finish ******************************
    PrintStatistic(&game);
	return 0;
}
















