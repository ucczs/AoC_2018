#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <Windows.h>

#define TEST_RUN 0

#if(TEST_RUN == 1)
    #define STAR_COUNT 31
    #define LINE_LENGTH 37
    #define NUMBER_LENGTH 2
    #define STEPS_TOTAL 4
#else
    #define STAR_COUNT 343
    #define LINE_LENGTH 45
    #define NUMBER_LENGTH 6
    #define STEPS_TOTAL 20000000
#endif

#define MAP_PRINT_OFFSET 3

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

typedef struct{
    int x_pos;
    int y_pos;

    int x_velo;
    int y_velo;
}StarType;

typedef struct{
    StarType starCollection[STAR_COUNT];

    int grid_minXVal;
    int grid_maxXVal;
    int grid_minYVal;
    int grid_maxYVal;
    int step_number;
}NightSkyType;

static void extractDataFromLine(char *line, StarType* starCollection)
{
    sscanf(line, "position=<%d, %d> velocity=<%d, %d>", 
    &starCollection->x_pos, 
    &starCollection->y_pos, 
    &starCollection->x_velo, 
    &starCollection->y_velo);
}

static void setMinMaxGrid(NightSkyType *nightSky)
{
    nightSky->grid_maxXVal = -50000;
    nightSky->grid_maxYVal = -50000;
    nightSky->grid_minXVal = 50000;
    nightSky->grid_minXVal = 50000;

    for (int i = 0; i < STAR_COUNT; i++)
    {
        nightSky->grid_maxXVal = MAX(nightSky->grid_maxXVal, nightSky->starCollection[i].x_pos);
        nightSky->grid_maxYVal = MAX(nightSky->grid_maxYVal, nightSky->starCollection[i].y_pos);

        nightSky->grid_minXVal = MIN(nightSky->grid_minXVal, nightSky->starCollection[i].x_pos);
        nightSky->grid_minYVal = MIN(nightSky->grid_minYVal, nightSky->starCollection[i].y_pos);
    }
}

static boolean checkLineForStar(NightSkyType *nightSky, int line_index)
{
    bool starPresentInLine = false;

    for (int j = nightSky->grid_minXVal-1; j <= nightSky->grid_maxXVal+1; j++)
    {
        for (int k = 0; k < STAR_COUNT; k++)
        {
            if( nightSky->starCollection[k].x_pos == j 
                && nightSky->starCollection[k].y_pos == line_index)
                {
                    starPresentInLine = true;
                }
        }
    }

    return(starPresentInLine);
}

static void printMap(NightSkyType *nightSky)
{
    printf("%d --------------- %d ------------- %d\n", nightSky->step_number, nightSky->step_number, nightSky->step_number);


    bool StarPresent = false;


    for (int i = nightSky->grid_minYVal-1; i <= nightSky->grid_maxYVal+1; i++)
    {
        if( checkLineForStar(nightSky, i))
        {
            for (int j = nightSky->grid_minXVal-1; j <= nightSky->grid_maxXVal+1; j++)
            {
                StarPresent = false;

                for (int k = 0; k < STAR_COUNT; k++)
                {
                    if( nightSky->starCollection[k].x_pos == j 
                    && nightSky->starCollection[k].y_pos == i)
                    {
                        StarPresent = true;
                    }
                }

                if( StarPresent == true)
                {
                    printf("#");
                }
                else
                {
                    printf(".");
                }
            }
            printf("\n");
        }
    }
    
    printf("\n\n");
    
}

static void performOneTimeStep(NightSkyType *nightSky)
{
    for (int i = 0; i < STAR_COUNT; i++)
    {
        nightSky->starCollection[i].x_pos += nightSky->starCollection[i].x_velo;
        nightSky->starCollection[i].y_pos += nightSky->starCollection[i].y_velo;
    }
}

static boolean starsTouchingCheck(NightSkyType *nightSky)
{
    boolean return_val = false;

    StarType *focusStar = &nightSky->starCollection[1];

    for (int i = 2; i < STAR_COUNT; i++)
    {
        if( (focusStar->x_pos+1) == nightSky->starCollection[i].x_pos 
         || (focusStar->x_pos-1) == nightSky->starCollection[i].x_pos
         || (focusStar->y_pos+1) == nightSky->starCollection[i].y_pos
         || (focusStar->y_pos-1) == nightSky->starCollection[i].y_pos)
         {
            if( (focusStar->x_pos+2) == nightSky->starCollection[i].x_pos 
            || (focusStar->x_pos-2) == nightSky->starCollection[i].x_pos
            || (focusStar->y_pos+2) == nightSky->starCollection[i].y_pos
            || (focusStar->y_pos-2) == nightSky->starCollection[i].y_pos)
                return_val = true;
         }
    }

    return(return_val);
}

int main()
{
    FILE *fp;
    char line[LINE_LENGTH];

    NightSkyType nightSky;

    nightSky.grid_maxXVal = 0;
    nightSky.grid_maxYVal = 0;
    nightSky.grid_minXVal = 0;
    nightSky.grid_minXVal = 0;
    nightSky.step_number = 0;

#if (TEST_RUN == 1)
    fp = fopen("D:\\Creativity\\Advent_of_Code\\AoC_2018\\10_C\\10_01\\test.txt", "r");
#else
    fp = fopen("D:\\Creativity\\Advent_of_Code\\AoC_2018\\10_C\\10_01\\input.txt", "r");
#endif

    if( fp == NULL)
    {
        printf("File could not be read!\n");
        exit(1);
    }

    int star_counter = 0;
    while(fgets(line, sizeof(line), fp) != NULL)
    {
        extractDataFromLine(&line[0], &nightSky.starCollection[star_counter] );
        star_counter++;
    }

    while(1)
    {
        nightSky.step_number++;
        
        setMinMaxGrid(&nightSky);

        // find heuristically situations when the stars are close together
        // in order to define the below used range
        //if( starsTouchingCheck(&nightSky) )
        //{
        //    printMap(&nightSky);
        //}

        if( nightSky.step_number > 10650 && nightSky.step_number < 10660)
            printMap(&nightSky);

        performOneTimeStep(&nightSky.starCollection[0]);
    }
    

    return(0);
}