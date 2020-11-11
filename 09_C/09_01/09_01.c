#define TEST_RUN 0

#if(TEST_RUN == 1)
    #define PLAYER_COUNT 9
    #define MARBLE_TOTAL_COUNT 25
    #define EXPECTED_RESULT 32
#elif(TEST_RUN == 2)
    #define PLAYER_COUNT 10
    #define MARBLE_TOTAL_COUNT 1618
    #define EXPECTED_RESULT 8317
#elif(TEST_RUN == 3)
    #define PLAYER_COUNT 13
    #define MARBLE_TOTAL_COUNT 7999
    #define EXPECTED_RESULT 146373
#elif(TEST_RUN == 4)
    #define PLAYER_COUNT 17
    #define MARBLE_TOTAL_COUNT 1104
    #define EXPECTED_RESULT 2764
#elif(TEST_RUN == 5)
    #define PLAYER_COUNT 21
    #define MARBLE_TOTAL_COUNT 6111
    #define EXPECTED_RESULT 54718
#elif(TEST_RUN == 6)
    #define PLAYER_COUNT 30
    #define MARBLE_TOTAL_COUNT 5807
    #define EXPECTED_RESULT 37305
#else
    #define PLAYER_COUNT 411
    #define MARBLE_TOTAL_COUNT 71058
    #define EXPECTED_RESULT 0
#endif

#define SPECIAL_ACTION_COUNT 23
#define ADD_COUNT 2
#define REMOVE_COUNT 7

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct 
{
    int* circleMap;
    int marbles_inGame;
    int marbles_onBoard;
    int idx_currentMarble;
}MapType;

typedef struct
{
    int* scorePlayers;
    int idx_activePlayer;
}PlayersCollectionType;

// prototypes
void printMap(MapType *map);
static void insertElement(int* array, int idx, int value, int arrayLength);
static void removeElement(int* array, int idx, int arrayLength);
static void performNormalMove(MapType* map);
static void performSpecialMove(MapType* map, PlayersCollectionType* playersCollection);
static void performNextMove(MapType* map, PlayersCollectionType* playersCollection);
void getHighScore(PlayersCollectionType *playersCollection, int *highscore, int *highscore_player);
void printMap(MapType *map);


static void insertElement(int* array, int idx, int value, int arrayLength)
{
    for (int i = arrayLength; i > idx; i--)
    {
        array[i] = array[i-1];
    }

    array[idx] = value;
}

static void removeElement(int* array, int idx, int arrayLength)
{
    for (int i = idx; i < arrayLength-1; i++)
    {
        array[i] = array[i+1];
    }
}

static void performNormalMove(MapType* map)
{
    map->idx_currentMarble += ADD_COUNT;

    if(map->idx_currentMarble > map->marbles_onBoard)
    {
        map->idx_currentMarble -= map->marbles_onBoard;
    }

    insertElement(map->circleMap, map->idx_currentMarble, map->marbles_inGame-1, MARBLE_TOTAL_COUNT);
    map->marbles_onBoard++;
}

static void performSpecialMove(MapType* map, PlayersCollectionType* playersCollection)
{
    playersCollection->scorePlayers[playersCollection->idx_activePlayer] += map->marbles_inGame-1;

    map->idx_currentMarble -= REMOVE_COUNT;
    if(map->idx_currentMarble < 0)
    {
        map->idx_currentMarble += map->marbles_onBoard;
    }

    map->marbles_onBoard--;
    playersCollection->scorePlayers[playersCollection->idx_activePlayer] +=map->circleMap[map->idx_currentMarble];
    removeElement(map->circleMap, map->idx_currentMarble, MARBLE_TOTAL_COUNT);
}

static void performNextMove(MapType* map, PlayersCollectionType* playersCollection)
{
    map->marbles_inGame++;

    if( ((map->marbles_inGame - 1) % SPECIAL_ACTION_COUNT) == 0)
    {
        performSpecialMove(map, playersCollection);
    }
    else
    {
        performNormalMove(map);
    }

    // next players turn
    if(playersCollection->idx_activePlayer == (PLAYER_COUNT-1))
    {
        playersCollection->idx_activePlayer = 0;
    }
    else
    {
        playersCollection->idx_activePlayer++;
    }
}

void getHighScore(PlayersCollectionType *playersCollection, int *highscore, int *highscore_player)
{
    *highscore = 0;

    for (int i = 0; i < PLAYER_COUNT; i++)
    {
        if(*highscore < playersCollection->scorePlayers[i])
        {
            *highscore = playersCollection->scorePlayers[i];
            *highscore_player = i;
        }
    }

    return(highscore);
}

void printMap(MapType *map)
{
    for (int i = 0; i < map->marbles_onBoard; i++)
    {
        printf("%02d ", map->circleMap[i]);
    }

    printf("\n");
}

void printPlayerStat(PlayersCollectionType *playersCollection)
{
    for (int i = 0; i < PLAYER_COUNT; i++)
    {
        printf("Player no %02d: %d\n", i, playersCollection->scorePlayers[i]);
    }
    
}

int main()
{
    PlayersCollectionType playersCollection;
    int* scorePlayers = (int*)calloc(PLAYER_COUNT, sizeof(int));
    playersCollection.scorePlayers = scorePlayers;
    playersCollection.idx_activePlayer = 0;

    MapType map;
    int* circleMap = (int*)calloc(MARBLE_TOTAL_COUNT, sizeof(int));
    map.circleMap = circleMap;
    map.circleMap[0] = 0;
    map.marbles_inGame = 1;
    map.marbles_onBoard = 1;
    map.idx_currentMarble = 0;

    while (map.marbles_inGame < MARBLE_TOTAL_COUNT)
    {
        performNextMove(&map, &playersCollection);
    }

    int highscore = 0;
    int highscorePlayer = 0;
    
    getHighScore(&playersCollection, &highscore, &highscorePlayer);

    printf("Calculated high score: %d\n", highscore);
    printf("Best Player: %d\n", highscorePlayer+1);
    printf("Expected result: %d\n", EXPECTED_RESULT);

    //printPlayerStat(&playersCollection);
    //printMap(&map);

    return(0);
}
