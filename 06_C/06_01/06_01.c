#include <stdio.h>
#include <stdlib.h>

#define TEST_RUN 0
#define MAX_LINE_LENGHT 10

#if(TEST_RUN == 1)
    #define MAX_MAP_XDIM 10
    #define MAX_MAP_YDIM 11
    #define NUMB_COORDINATES 6
#else
    #define MAX_MAP_XDIM 360
    #define MAX_MAP_YDIM 339
    #define NUMB_COORDINATES 50
#endif

typedef struct{
    int xCoordinate;
    int yCoordinate;
    int ID;
}Coordinates;

Coordinates readInCoordiantes(char *line)
{
    static int ID_cnt = 0;

    Coordinates newCoordinate;
    newCoordinate.xCoordinate = 0;
    newCoordinate.yCoordinate = 0;

    int xFlag = 1;
    int yFlag = 0;

    for (int i = 0; i < MAX_LINE_LENGHT; i++)
    {
        if(line[i] == ',')
        {
            xFlag = 0;
            yFlag = 1;
        }
        else if(line[i] == ' ')
        {
            continue;
        }
        else if(line[i] == '\000' || line[i] == '\n')
        {
            yFlag = 0;
        }
        else if(xFlag == 1)
        {
            newCoordinate.xCoordinate *= 10;
            newCoordinate.xCoordinate += (int)(line[i] - '0');
        }
        else if(yFlag == 1)
        {
            newCoordinate.yCoordinate *= 10;
            newCoordinate.yCoordinate += (int)(line[i] - '0');
        }
        else
            printf("Something wrong with reading in new Coordinates.");
    }

    newCoordinate.ID = ID_cnt;
    ID_cnt++;

    return(newCoordinate);
}

int findNearestCoordinate(int xpos, int ypos, Coordinates CoordinateList[NUMB_COORDINATES])
{
    int min_distance = 10000;
    int nearestID = -1;

    for (int i = 0; i < NUMB_COORDINATES; i++)
    {
        int distance = abs(CoordinateList[i].xCoordinate - xpos) + abs(CoordinateList[i].yCoordinate - ypos);
        if(distance < min_distance)
        {
            min_distance = distance;
            nearestID = CoordinateList[i].ID;
        }
        else if (distance == min_distance)
        {
            nearestID = -1;
        }
    }

    return(nearestID);
}

void createMap(int map[MAX_MAP_YDIM][MAX_MAP_XDIM], Coordinates CoordinateList[NUMB_COORDINATES])
{
    for (int y = 0; y < MAX_MAP_YDIM; y++)
    {
        for (int x = 0; x < MAX_MAP_XDIM; x++)
        {
            int min_distance = 1000;
            map[y][x] = findNearestCoordinate(x, y, CoordinateList);
        }
    }
}

void printMap(int map[MAX_MAP_YDIM][MAX_MAP_XDIM])
{
    for (int y = 0; y < MAX_MAP_YDIM; y++)
    {
        for (int x = 0; x < MAX_MAP_XDIM; x++)
        {
            if(map[y][x] >= 0)
                printf("+%02d ", map[y][x]);
            else
                printf("%03d ", map[y][x]);

        }
        printf("\n");
    }
}

int infiniteCheck(int IDtoCheck, int map[MAX_MAP_YDIM][MAX_MAP_XDIM])
{
    int IDInfinite = 0;

    for (int x = 0; x < MAX_MAP_XDIM; x++)
    {
        if( IDtoCheck == map[0][x] || IDtoCheck == map[ MAX_MAP_YDIM-1 ][x] )
        {
            IDInfinite = 1;
        }
    }

    for (int y = 0; y < MAX_MAP_YDIM; y++)
    {
        if( IDtoCheck == map[y][0] || IDtoCheck == map[y][MAX_MAP_XDIM-1] )
        {
            IDInfinite = 1;
        }
    }
    return IDInfinite;
}

int countAreaOfID(int map[MAX_MAP_YDIM][MAX_MAP_XDIM], int ID)
{
    int areaCount = 0;

    for (int y = 0; y < MAX_MAP_YDIM; y++)
    {
        for (int x = 0; x < MAX_MAP_XDIM; x++)
        {
            if(map[y][x] == ID)
                areaCount++;
        }
    }
    return(areaCount);
}

int getBiggestFiniteArea(int map[MAX_MAP_YDIM][MAX_MAP_XDIM], Coordinates CoordinateList[NUMB_COORDINATES])
{
    int biggestArea = 0;

    for (int i = 0; i < NUMB_COORDINATES; i++)
    {
        if( infiniteCheck(CoordinateList[i].ID, map) == 0 )
        {
            int currentArea = countAreaOfID(map, CoordinateList[i].ID);

            if(currentArea > biggestArea)
                biggestArea = currentArea;
        }
    }
    return(biggestArea);
}

int main(void)
{
    FILE *fp;
    char line[MAX_LINE_LENGHT];
    Coordinates CoordinateList[NUMB_COORDINATES];
    int MapArea[MAX_MAP_YDIM][MAX_MAP_XDIM];

#if (TEST_RUN == 1)
    fp = fopen("D:\\Creativity\\Advent_of_Code\\AoC_2018\\06_C\\06_01\\test.txt", "r");
#else
    fp = fopen("D:\\Creativity\\Advent_of_Code\\AoC_2018\\06_C\\06_01\\input.txt", "r");
#endif

    if(fp == NULL)
    {
        printf("File could not be open!");
        exit(1);
    }

    int cooridnate_cnt = 0;
    while(fgets(line, sizeof(line), fp) != NULL)
    {
        CoordinateList[cooridnate_cnt] = readInCoordiantes(&line[0]);
        cooridnate_cnt++;
    }

    createMap(MapArea, CoordinateList);
    //printMap(MapArea);

    int biggestArea = getBiggestFiniteArea(MapArea, CoordinateList);
    printf("Biggest finite area: %d", biggestArea);

    return(0);
}