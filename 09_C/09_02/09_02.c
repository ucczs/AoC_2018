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
    #define MARBLE_TOTAL_COUNT 7105800
    #define EXPECTED_RESULT 0
#endif

#define SPECIAL_ACTION_COUNT 23
#define ADD_COUNT 2
#define REMOVE_COUNT 7

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h> 

typedef struct CircleElementType CircleElementType;

struct CircleElementType
{
    unsigned long long value;
    CircleElementType* nextElement;
    CircleElementType* previousElement;
};


typedef struct 
{
    CircleElementType* currentMarble;
    unsigned long long marbles_inGame;
    unsigned long long marbles_onBoard;
}MapType;

typedef struct
{
    unsigned long long int* scorePlayers;
    int idx_activePlayer;
}PlayersCollectionType;


static void performNormalMove(MapType* map)
{
    CircleElementType* newElement = (CircleElementType*)calloc(1, sizeof(CircleElementType));
    newElement->value = map->marbles_inGame-1;

    CircleElementType* ElementAfterCurrent = map->currentMarble->nextElement;
    CircleElementType* ElementTwoAfterCurrent = map->currentMarble->nextElement->nextElement;

    // redirect current pointers of next and nextButOne element
    ElementAfterCurrent->nextElement = newElement;
    ElementTwoAfterCurrent->previousElement = newElement;

    // set pointers of new element correctly
    newElement->nextElement = ElementTwoAfterCurrent;
    newElement->previousElement = ElementAfterCurrent;
    
    map->currentMarble = newElement;
    map->marbles_onBoard++;
}

static void performSpecialMove(MapType* map, PlayersCollectionType* playersCollection)
{
    playersCollection->scorePlayers[playersCollection->idx_activePlayer] += map->marbles_inGame-1;

    for (int i = 0; i < REMOVE_COUNT; i++)
    {
        map->currentMarble = map->currentMarble->previousElement;
    }

    map->marbles_onBoard--;
    playersCollection->scorePlayers[playersCollection->idx_activePlayer] += map->currentMarble->value;

    // remove marble
    CircleElementType *marbleToRemove = map->currentMarble;
    map->currentMarble->previousElement->nextElement = map->currentMarble->nextElement;
    map->currentMarble->nextElement->previousElement = map->currentMarble->previousElement;

    map->currentMarble = map->currentMarble->nextElement;

    free(marbleToRemove);

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

static void getHighScore(PlayersCollectionType *playersCollection, unsigned long long int*highscore, int *highscore_player)
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
}

void printMap(MapType *map)
{
    for (int i = 0; i < map->marbles_onBoard; i++)
    {
        printf("%02d ", map->currentMarble->value);
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
    unsigned long long int* scorePlayers = (unsigned long long int*)calloc(PLAYER_COUNT, sizeof(unsigned long long int));
    playersCollection.scorePlayers = scorePlayers;
    playersCollection.idx_activePlayer = 0;

    MapType map;
    CircleElementType* circleMap = (CircleElementType*)calloc(1, sizeof(CircleElementType));
    map.currentMarble = circleMap;
    map.currentMarble->value = 0;
    map.currentMarble->nextElement = map.currentMarble;
    map.currentMarble->previousElement = map.currentMarble;

    map.marbles_inGame = 1;
    map.marbles_onBoard = 1;

    clock_t startTime; 
    clock_t currentTime;

    startTime = clock();


    while (map.marbles_inGame < MARBLE_TOTAL_COUNT)
    {
        performNextMove(&map, &playersCollection);

        if(map.marbles_inGame % 100000 == 0)
        {
            currentTime = clock() - startTime;
            double time_taken = ((double)currentTime)/CLOCKS_PER_SEC;
            printf("Marble count: %d, %.2f\n", map.marbles_inGame, time_taken);
        }
    }

    unsigned long long int highscore = 0;
    int highscorePlayer = 0;
    
    getHighScore(&playersCollection, &highscore, &highscorePlayer);

    printf("Calculated high score: %lld\n", highscore);
    printf("Best Player: %d\n", highscorePlayer+1);
    printf("Expected result: %d\n", EXPECTED_RESULT);

    //printPlayerStat(&playersCollection);
    //printMap(&map);

    return(0);
}
