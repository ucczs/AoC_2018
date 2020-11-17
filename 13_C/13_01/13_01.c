#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define TEST_RUN 0

#if( TEST_RUN == 1)
    #define CARTS_COUNT 2
    #define X_MAX 13
    #define Y_MAX 6
#else
    #define CARTS_COUNT 17
    #define X_MAX 150
    #define Y_MAX 150
#endif

typedef enum{
    TURN_LEFT,
    STRAIGHT,
    TURN_RIGHT
}intersec_action;

typedef enum{
    LEFT,
    UP,
    RIGHT,
    DOWN
}cart_direction;

typedef enum{
    VERTICAL,
    HORIZONTAL,
    CURVE_RIGHT,
    CURVE_LEFT,
    INTERSECTION,
    NONE
}track_type;

typedef struct{
    cart_direction direction;
    intersec_action next_action;
    bool cartMovedInThisTick;
}cart_type;

typedef struct{
    cart_type* cart;
    track_type trackType;
}mapElement_type;

typedef struct{
    bool collision_occured;
    int collision_x_pos;
    int collision_y_pos;
}collision_type;

typedef struct{
    mapElement_type map[Y_MAX][X_MAX];
    cart_type carts[CARTS_COUNT];
    collision_type collision;
    int tick_count;
}scene_type;


void readInLine(char*line, scene_type* scene)
{
    static int y_count = 0;
    static int cart_count = 0;

    if(y_count > Y_MAX) printf("\nERROR during read in of map! Y count too big!\n");
    if(cart_count > CARTS_COUNT) printf("\nERROR during read in of map! Cart count too big!\n");

    for (size_t i = 0; i < X_MAX; i++)
    {
        switch ( line[i] )
        {
        case ' ':
            scene->map[y_count][i].trackType = NONE;
            scene->map[y_count][i].cart = NULL;
            break;
        case '/':
            scene->map[y_count][i].trackType = CURVE_RIGHT;
            scene->map[y_count][i].cart = NULL;
            break;
        case '\\':
            scene->map[y_count][i].trackType = CURVE_LEFT;
            scene->map[y_count][i].cart = NULL;
            break;
        case '-':
            scene->map[y_count][i].trackType = HORIZONTAL;
            scene->map[y_count][i].cart = NULL;
            break;
        case '|':
            scene->map[y_count][i].trackType = VERTICAL;
            scene->map[y_count][i].cart = NULL;
            break;
        case '+':
            scene->map[y_count][i].trackType = INTERSECTION;
            scene->map[y_count][i].cart = NULL;
            break;
        case '<':
        case '>':
            scene->map[y_count][i].trackType = HORIZONTAL;
            scene->carts[cart_count].next_action = TURN_LEFT;

            if( line[i] == '>') scene->carts[cart_count].direction = RIGHT;
            else scene->carts[cart_count].direction = LEFT;
            
            scene->map[y_count][i].cart = &scene->carts[cart_count];

            cart_count++;
            break;
        case '^':
        case 'v':
            scene->map[y_count][i].trackType = VERTICAL;
            scene->carts[cart_count].next_action = TURN_LEFT;

            if( line[i] == 'v') scene->carts[cart_count].direction = DOWN;
            else scene->carts[cart_count].direction = UP;
            
            scene->map[y_count][i].cart = &scene->carts[cart_count];

            cart_count++;
            break;
        default:
            printf("\nERROR during read in of map! Undefined char read in.\n");
            break;
        }
    }

    y_count++;
}

void moveCart(scene_type* scene, int x_pos, int y_pos)
{
    cart_type *currentCart = scene->map[y_pos][x_pos].cart;
    currentCart->cartMovedInThisTick = true;

    switch ( currentCart->direction )
    {
    case LEFT:
        if( scene->map[y_pos][x_pos-1].cart == NULL)
        {
            scene->map[y_pos][x_pos-1].cart = currentCart;
            scene->map[y_pos][x_pos].cart = NULL;

            if( scene->map[y_pos][x_pos-1].trackType == CURVE_LEFT )
            {
                currentCart->direction = UP;
            }
            else if( scene->map[y_pos][x_pos-1].trackType == CURVE_RIGHT )
            {
                currentCart->direction = DOWN;
            }
            else if( scene->map[y_pos][x_pos-1].trackType == INTERSECTION )
            {
                switch ( currentCart->next_action )
                {
                case TURN_LEFT:
                    currentCart->next_action = STRAIGHT;
                    currentCart->direction = DOWN;
                    break;
                case STRAIGHT:
                    currentCart->next_action = TURN_RIGHT;
                    break;
                case TURN_RIGHT:
                    currentCart->next_action = TURN_LEFT;
                    currentCart->direction = UP;
                    break;
                default:
                    printf("Intersection invalid!\n");
                    break;
                }
            }
            else if( scene->map[y_pos][x_pos-1].trackType == HORIZONTAL )
            {
                // nothing to do
            }
            else printf("Error in moving cart left, issue in map!\n");
        }
        else
        {
            // collision
            scene->collision.collision_occured = true;
            scene->collision.collision_x_pos = x_pos-1;
            scene->collision.collision_y_pos = y_pos;
        }
        break;
    case UP:
        if( scene->map[y_pos-1][x_pos].cart == NULL)
        {
            scene->map[y_pos-1][x_pos].cart = currentCart;
            scene->map[y_pos][x_pos].cart = NULL;

            if( scene->map[y_pos-1][x_pos].trackType == CURVE_LEFT )
            {
                currentCart->direction = LEFT;
            }
            else if( scene->map[y_pos-1][x_pos].trackType == CURVE_RIGHT )
            {
                currentCart->direction = RIGHT;
            }
            else if( scene->map[y_pos-1][x_pos].trackType == INTERSECTION )
            {
                switch ( currentCart->next_action )
                {
                case TURN_LEFT:
                    currentCart->next_action = STRAIGHT;
                    currentCart->direction = LEFT;
                    break;
                case STRAIGHT:
                    currentCart->next_action = TURN_RIGHT;
                    break;
                case TURN_RIGHT:
                    currentCart->next_action = TURN_LEFT;
                    currentCart->direction = RIGHT;
                    break;
                default:
                    printf("Intersection invalid!\n");
                    break;
                }
            }
            else if( scene->map[y_pos-1][x_pos].trackType == VERTICAL )
            {
                // nothing to do
            }
            else printf("Error in moving cart up, issue in map!\n");
        }
        else
        {
            // collision
            scene->collision.collision_occured = true;
            scene->collision.collision_x_pos = x_pos;
            scene->collision.collision_y_pos = y_pos-1;
        }
        break;
    case RIGHT:
        if( scene->map[y_pos][x_pos+1].cart == NULL)
        {
            scene->map[y_pos][x_pos+1].cart = currentCart;
            scene->map[y_pos][x_pos].cart = NULL;

            if( scene->map[y_pos][x_pos+1].trackType == CURVE_LEFT )
            {
                currentCart->direction = DOWN;
            }
            else if( scene->map[y_pos][x_pos+1].trackType == CURVE_RIGHT )
            {
                currentCart->direction = UP;
            }
            else if( scene->map[y_pos][x_pos+1].trackType == INTERSECTION )
            {
                switch ( currentCart->next_action )
                {
                case TURN_LEFT:
                    currentCart->next_action = STRAIGHT;
                    currentCart->direction = UP;
                    break;
                case STRAIGHT:
                    currentCart->next_action = TURN_RIGHT;
                    break;
                case TURN_RIGHT:
                    currentCart->next_action = TURN_LEFT;
                    currentCart->direction = DOWN;
                    break;
                default:
                    printf("Intersection invalid!\n");
                    break;
                }
            }
            else if( scene->map[y_pos][x_pos+1].trackType == HORIZONTAL )
            {
                // nothing to do
            }
            else printf("Error in moving cart right, issue in map!\n");
        }
        else
        {
            // collision
            scene->collision.collision_occured = true;
            scene->collision.collision_x_pos = x_pos+1;
            scene->collision.collision_y_pos = y_pos;
        }
        break;
    case DOWN:
        if( scene->map[y_pos+1][x_pos].cart == NULL)
        {
            scene->map[y_pos+1][x_pos].cart = currentCart;
            scene->map[y_pos][x_pos].cart = NULL;

            if( scene->map[y_pos+1][x_pos].trackType == CURVE_LEFT )
            {
                currentCart->direction = RIGHT;
            }
            else if( scene->map[y_pos+1][x_pos].trackType == CURVE_RIGHT )
            {
                currentCart->direction = LEFT;
            }
            else if( scene->map[y_pos+1][x_pos].trackType == INTERSECTION )
            {
                switch ( currentCart->next_action )
                {
                case TURN_LEFT:
                    currentCart->next_action = STRAIGHT;
                    currentCart->direction = RIGHT;
                    break;
                case STRAIGHT:
                    currentCart->next_action = TURN_RIGHT;
                    break;
                case TURN_RIGHT:
                    currentCart->next_action = TURN_LEFT;
                    currentCart->direction = LEFT;
                    break;
                default:
                    printf("Intersection invalid!\n");
                    break;
                }
            }
            else if( scene->map[y_pos+1][x_pos].trackType == VERTICAL )
            {
                // nothing to do
            }
            else printf("Error in moving cart down, issue in map!\n");
        }
        else
        {
            // collision
            scene->collision.collision_occured = true;
            scene->collision.collision_x_pos = x_pos;
            scene->collision.collision_y_pos = y_pos+1;
        }
        break;
    
    default:
        break;
    }
}

void resetMoveState(scene_type* scene)
{
    for (int i = 0; i < CARTS_COUNT; i++)
    {
        scene->carts[i].cartMovedInThisTick = false;
    }
}

void performNextTick(scene_type* scene)
{
    resetMoveState(scene);

    for (int y = 0; y < Y_MAX; y++)
    {
        for (int x = 0; x < X_MAX; x++)
        {
            if( scene->map[y][x].cart != NULL )
            {
                if( scene->map[y][x].cart->cartMovedInThisTick == false)
                {
                    moveCart(scene, x, y);
                }
            }
        }
    }
}

void printMap(scene_type* scene)
{
    for (int y = 0; y < Y_MAX; y++)
    {
        for (int x = 0; x < X_MAX; x++)
        {
            if(scene->map[y][x].cart != NULL)
            {
                switch (scene->map[y][x].cart->direction)
                {
                case LEFT:
                    printf("<");
                    break;
                case UP:
                    printf("^");
                    break;
                case RIGHT:
                    printf(">");
                    break;
                case DOWN:
                    printf("v");
                    break;
                default:
                    break;
                }
            }
            else
            {
                switch ( scene->map[y][x].trackType )
                {
                case VERTICAL:
                    printf("|");
                    break;
                case HORIZONTAL:
                    printf("-");
                    break;
                case CURVE_RIGHT:
                    printf("/");
                    break;
                case CURVE_LEFT:
                    printf("\\");
                    break;
                case INTERSECTION:
                    printf("+");
                    break;
                case NONE:
                    printf(" ");
                    break;
                default:
                    break;
                }
            }
        }
        printf("\n");
    }
}

int main()
{
    FILE *fp;
    char line[X_MAX+2];
    scene_type scene;
    scene.collision.collision_occured = false;
    scene.tick_count = 0;

#if(TEST_RUN == 1)
    fp = fopen("D:\\Creativity\\Advent_of_Code\\AoC_2018\\13_C\\13_01\\test.txt", "r");
#else
    fp = fopen("D:\\Creativity\\Advent_of_Code\\AoC_2018\\13_C\\13_01\\input.txt", "r");
#endif

    if(fp == NULL)
    {
        printf("Input file cannot be read!\n");
    }

    while(fgets(&line, sizeof(line), fp) != NULL)
    {
        readInLine(&line, &scene);
    }

    //printMap(&scene);

    while(scene.collision.collision_occured == false)
    {
        scene.tick_count++;
        performNextTick(&scene);
        //printMap(&scene);
    }

    printf("Collision occured!\n");
    printf("x: %d, y: %d, tick count: %d\n", scene.collision.collision_x_pos, scene.collision.collision_y_pos, scene.tick_count);

    return(0);
}