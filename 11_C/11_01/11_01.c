#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define TEST_RUN 0

#if( TEST_RUN == 1 )
    #define COORDINATE_X_MIN 32
    #define COORDINATE_Y_MIN 44
    #define SIZE_GRID_X 5
    #define SIZE_GRID_Y 5
    #define SERIAL_NO 18
    #define PRINT_MAP 1
#elif ( TEST_RUN == 2 )
    #define COORDINATE_X_MIN 20
    #define COORDINATE_Y_MIN 60
    #define SIZE_GRID_X 5
    #define SIZE_GRID_Y 5
    #define SERIAL_NO 42
    #define PRINT_MAP 1
#else
    #define COORDINATE_X_MIN 1
    #define COORDINATE_Y_MIN 1
    #define SIZE_GRID_X 300
    #define SIZE_GRID_Y 300
    #define SERIAL_NO 2187
    #define PRINT_MAP 0
#endif

#define ENERGY_SQUARE_SIZE 3


typedef struct {
    int x_pos;
    int y_pos;
    
    int rack_ID;
    int power_level;

    int totalSquarePower;
}CoordinateType;


typedef struct{
    int x_min;
    int x_max;

    int y_min;
    int y_max;

    int serial_number;

    CoordinateType gridFuelLevel[SIZE_GRID_X * SIZE_GRID_Y];
}GridType;


static void printFuelLevels(GridType *grid)
{
    for (int j = COORDINATE_Y_MIN; j < (COORDINATE_Y_MIN + SIZE_GRID_Y); j++)
    {
        for (int i = COORDINATE_X_MIN; i < (COORDINATE_X_MIN + SIZE_GRID_X); i++)
        {
            int current_idx = (i-COORDINATE_X_MIN) + ((j-COORDINATE_Y_MIN) * SIZE_GRID_Y);
            CoordinateType *currentCoordinate = &grid->gridFuelLevel[current_idx];
            printf("%02d ", currentCoordinate->power_level);
        }
        printf("\n");
    }
    printf("\n");
}

static void printSquarePower(GridType *grid)
{
    for (int j = COORDINATE_Y_MIN; j < (COORDINATE_Y_MIN + SIZE_GRID_Y); j++)
    {
        for (int i = COORDINATE_X_MIN; i < (COORDINATE_X_MIN + SIZE_GRID_X); i++)
        {
            int current_idx = (i-COORDINATE_X_MIN) + ((j-COORDINATE_Y_MIN) * SIZE_GRID_Y);
            CoordinateType *currentCoordinate = &grid->gridFuelLevel[current_idx];
            printf("%02d ", currentCoordinate->totalSquarePower);
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

    grid->x_max = grid->x_min + (SIZE_GRID_X - 1);
    grid->y_max = grid->y_min + (SIZE_GRID_Y - 1);

    for (int j = COORDINATE_Y_MIN; j < (COORDINATE_Y_MIN + SIZE_GRID_Y); j++)
    {
        for (int i = COORDINATE_X_MIN; i < (COORDINATE_X_MIN + SIZE_GRID_X); i++)
        {
            int current_idx = (i-COORDINATE_X_MIN) + ((j-COORDINATE_Y_MIN) * SIZE_GRID_Y);
            CoordinateType *currentCoordinate = &grid->gridFuelLevel[current_idx];
            currentCoordinate->x_pos = i;
            currentCoordinate->y_pos = j;
            currentCoordinate->totalSquarePower = 0;

            currentCoordinate->rack_ID = currentCoordinate->x_pos + 10;
            int powerLevel2 = currentCoordinate->rack_ID * currentCoordinate->y_pos;
            int powerLevel3 = powerLevel2 + grid->serial_number;
            int powerLevel4 = powerLevel3 * currentCoordinate->rack_ID;
            int powerLevel5 = powerLevel4 % 1000;
            int powerLevel6 = (int)powerLevel5 / 100;
            currentCoordinate->power_level = powerLevel6 - 5;
        }
    }
}

static void calculateEnergySquares(GridType *grid)
{
    for (int j = COORDINATE_Y_MIN; j < (COORDINATE_Y_MIN + SIZE_GRID_Y); j++)
    {
        for (int i = COORDINATE_X_MIN; i < (COORDINATE_X_MIN + SIZE_GRID_X); i++)
        {
            int current_idx = ((i-COORDINATE_X_MIN) * SIZE_GRID_X) + (j-COORDINATE_Y_MIN);
            CoordinateType *currentCoordinate = &grid->gridFuelLevel[current_idx];

            if( i < (COORDINATE_X_MIN + SIZE_GRID_X - 2) && j < (COORDINATE_Y_MIN + SIZE_GRID_Y - 2) )
            {
                int current_square_idx = current_idx;

                for (int y_square = 0; y_square < (ENERGY_SQUARE_SIZE); y_square++)
                {
                    for (int x_square = 0; x_square < (ENERGY_SQUARE_SIZE); x_square++)
                    {
                        currentCoordinate->totalSquarePower += grid->gridFuelLevel[current_square_idx].power_level;
                        current_square_idx += 1;
                    }
                    current_square_idx += SIZE_GRID_X - ENERGY_SQUARE_SIZE;
                }
            }
        }
    }
}

static void findMaxSquare(GridType *grid)
{
    int max_x = 0;
    int max_y = 0;
    int maxEnergy = 0;

    for (int i = 0; i < SIZE_GRID_X * SIZE_GRID_Y; i++)
    {
        if(grid->gridFuelLevel[i].totalSquarePower > maxEnergy)
        {
            maxEnergy = grid->gridFuelLevel[i].totalSquarePower;
            max_x = grid->gridFuelLevel[i].x_pos;
            max_y = grid->gridFuelLevel[i].y_pos;
        }
    }
    
    printf("Max Energy: %d\n", maxEnergy);
    printf("Position: %d, %d\n", max_x, max_y);

}

int main()
{
    GridType grid;

    createGrid(&grid);

    if( PRINT_MAP == 1)
    {
        printFuelLevels(&grid);
        printf("\n\n");
    }


    calculateEnergySquares(&grid);
    
    if( PRINT_MAP == 1)
    {
        printSquarePower(&grid);
    }

    findMaxSquare(&grid);

    return(0);
}
