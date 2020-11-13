#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define TEST_RUN 0

#if( TEST_RUN == 1 )
    #define COORDINATE_X_MIN 32
    #define COORDINATE_Y_MIN 44
    #define SIZE_GRID 5
    #define SERIAL_NO 18
    #define PRINT_MAP 1
#elif ( TEST_RUN == 2 )
    #define COORDINATE_X_MIN 20
    #define COORDINATE_Y_MIN 60
    #define SIZE_GRID 5
    #define SERIAL_NO 42
    #define PRINT_MAP 1
#elif( TEST_RUN == 3 )
    #define COORDINATE_X_MIN 1
    #define COORDINATE_Y_MIN 1
    #define SIZE_GRID 281
    #define SERIAL_NO 18
    #define PRINT_MAP 0
#elif ( TEST_RUN == 4 )
    #define COORDINATE_X_MIN 1
    #define COORDINATE_Y_MIN 1
    #define SIZE_GRID 265
    #define SERIAL_NO 42
    #define PRINT_MAP 0
#else
    #define COORDINATE_X_MIN 1
    #define COORDINATE_Y_MIN 1
    #define SIZE_GRID 300
    #define SERIAL_NO 2187
    #define PRINT_MAP 0
#endif

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

typedef struct {
    int x_pos;
    int y_pos;
    
    int power_level;

    int totalSquarePower;
    int squareSize;
}CoordinateType;


typedef struct{
    int x_min;
    int x_max;

    int y_min;
    int y_max;

    int serial_number;

    CoordinateType gridFuelLevel[SIZE_GRID * SIZE_GRID];
}GridType;


static void printFuelLevels(GridType *grid)
{
    printf("Fuel levels of coordinates:\n");
    for (int j = COORDINATE_Y_MIN; j < (COORDINATE_Y_MIN + SIZE_GRID); j++)
    {
        for (int i = COORDINATE_X_MIN; i < (COORDINATE_X_MIN + SIZE_GRID); i++)
        {
            int current_idx = (i-COORDINATE_X_MIN) + ((j-COORDINATE_Y_MIN) * SIZE_GRID);
            CoordinateType *currentCoordinate = &grid->gridFuelLevel[current_idx];
            printf("%02d ", currentCoordinate->power_level);
        }
        printf("\n");
    }
    printf("\n");
}

static void printSquarePower(GridType *grid)
{
    printf("Best Square energy:\n");
    for (int j = COORDINATE_Y_MIN; j < (COORDINATE_Y_MIN + SIZE_GRID); j++)
    {
        for (int i = COORDINATE_X_MIN; i < (COORDINATE_X_MIN + SIZE_GRID); i++)
        {
            int current_idx = (i-COORDINATE_X_MIN) + ((j-COORDINATE_Y_MIN) * SIZE_GRID);
            CoordinateType *currentCoordinate = &grid->gridFuelLevel[current_idx];
            printf("%02d ", currentCoordinate->totalSquarePower);
        }
        printf("\n");
    }
    printf("\n");
}

static void printSquareSize(GridType *grid)
{
    printf("Best Square sizes:\n");
    for (int j = COORDINATE_Y_MIN; j < (COORDINATE_Y_MIN + SIZE_GRID); j++)
    {
        for (int i = COORDINATE_X_MIN; i < (COORDINATE_X_MIN + SIZE_GRID); i++)
        {
            int current_idx = (i-COORDINATE_X_MIN) + ((j-COORDINATE_Y_MIN) * SIZE_GRID);
            CoordinateType *currentCoordinate = &grid->gridFuelLevel[current_idx];
            printf("%02d ", currentCoordinate->squareSize);
        }
        printf("\n");
    }
    printf("\n");
}

void createGrid(GridType *grid)
{
    grid->serial_number = SERIAL_NO;
    grid->x_min = COORDINATE_X_MIN;
    grid->y_min = COORDINATE_Y_MIN;

    grid->x_max = grid->x_min + (SIZE_GRID - 1);
    grid->y_max = grid->y_min + (SIZE_GRID - 1);

    for (int j = COORDINATE_Y_MIN; j < (COORDINATE_Y_MIN + SIZE_GRID); j++)
    {
        for (int i = COORDINATE_X_MIN; i < (COORDINATE_X_MIN + SIZE_GRID); i++)
        {
            int current_idx = (i-COORDINATE_X_MIN) + ((j-COORDINATE_Y_MIN) * SIZE_GRID);
            CoordinateType *currentCoordinate = &grid->gridFuelLevel[current_idx];
            currentCoordinate->x_pos = i;
            currentCoordinate->y_pos = j;
            currentCoordinate->totalSquarePower = 0;

            int rack_ID = currentCoordinate->x_pos + 10;
            int powerLevel2 = rack_ID * currentCoordinate->y_pos;
            int powerLevel3 = powerLevel2 + grid->serial_number;
            int powerLevel4 = powerLevel3 * rack_ID;
            int powerLevel5 = powerLevel4 % 1000;
            int powerLevel6 = (int)powerLevel5 / 100;
            currentCoordinate->power_level = powerLevel6 - 5;
        }
    }
}

static void calculateAllPossibleSquareEnergy(GridType *grid, int j, int i)
{
    int current_idx = (i-COORDINATE_X_MIN) + ((j-COORDINATE_Y_MIN) * SIZE_GRID);
    CoordinateType *currentCoordinate = &grid->gridFuelLevel[current_idx];

    int biggest_energy = -6;
    int biggest_energy_squareSize = 0;

    int maxSizeSquareCurrentPos = MIN((COORDINATE_Y_MIN + SIZE_GRID - j), (COORDINATE_X_MIN + SIZE_GRID - i));

    for (int squareSize = 1; squareSize <= maxSizeSquareCurrentPos; squareSize++)
    {
        int current_energy = 0;

        if( i < (COORDINATE_X_MIN + SIZE_GRID - (squareSize - 1) ) && j < (COORDINATE_Y_MIN + SIZE_GRID - (squareSize - 1)) )
        {
            int current_square_idx = current_idx;

            for (int y_square = 0; y_square < squareSize; y_square++)
            {
                for (int x_square = 0; x_square < squareSize; x_square++)
                {
                    current_energy += grid->gridFuelLevel[current_square_idx].power_level;
                    current_square_idx += 1;
                }
                current_square_idx += SIZE_GRID - squareSize;
            }
        }

        if(current_energy > biggest_energy)
        {
            biggest_energy = current_energy;
            biggest_energy_squareSize = squareSize;
        }
    }
    currentCoordinate->totalSquarePower = biggest_energy;
    currentCoordinate->squareSize = biggest_energy_squareSize;
}

static void calculateEnergySquares(GridType *grid)
{
    for (int j = COORDINATE_Y_MIN; j < (COORDINATE_Y_MIN + SIZE_GRID); j++)
    {
        for (int i = COORDINATE_X_MIN; i < (COORDINATE_X_MIN + SIZE_GRID); i++)
        {
            calculateAllPossibleSquareEnergy(grid, j, i);
        }
    }
}

static void findMaxSquare(GridType *grid)
{
    int max_x = 0;
    int max_y = 0;
    int maxEnergy = 0;
    int squareSize = 0;

    for (int i = 0; i < SIZE_GRID * SIZE_GRID; i++)
    {
        if(grid->gridFuelLevel[i].totalSquarePower > maxEnergy)
        {
            maxEnergy = grid->gridFuelLevel[i].totalSquarePower;
            max_x = grid->gridFuelLevel[i].x_pos;
            max_y = grid->gridFuelLevel[i].y_pos;
            squareSize = grid->gridFuelLevel[i].squareSize;
        }
    }
    
    printf("Max Energy: %d\n", maxEnergy);
    printf("Position: %d, %d\n", max_x, max_y);
    printf("Square size: %d\n", squareSize);
}

int main()
{
    GridType grid;

    createGrid(&grid);

    if( PRINT_MAP == 1)
    {
        printFuelLevels(&grid);
    }

    calculateEnergySquares(&grid);
    
    if( PRINT_MAP == 1)
    {
        printSquarePower(&grid);
        printSquareSize(&grid);
    }

    findMaxSquare(&grid);

    return(0);
}
