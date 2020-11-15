#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define TEST_RUN 1

#if( TEST_RUN == 1)
    #define LENGHT_LINE_MAX 42
    #define RULE_COUNT 14
#else
    #define LENGHT_LINE_MAX 117
    #define RULE_COUNT 32
#endif

#define INITIAL_LENGTH_POS 15
#define RULE_POS_OUTCOME 10
#define MAX_GENERATION 20

typedef struct PotType PotType;

struct PotType{
    bool potHasPlant_curr;
    bool potHasPlant_nextGen;
    int potPosition;
    PotType* nextPot;
    PotType* previousPot;

    bool twoPrevState;
    bool prevState;
    bool nextState;
    bool twoNextState;
};

typedef struct{
    bool twoPrevState;
    bool prevState;
    bool currState;
    bool nextState;
    bool twoNextState;

    bool resultState;
}BreedRule;

PotType* getInitialState(char *line)
{
    int potNumber = 0;

    bool firstPotFound = false;
    PotType *firstPot;

    PotType *prevPot = NULL;

    for (int i = INITIAL_LENGTH_POS; i < LENGHT_LINE_MAX; i++)
    {
        PotType* newPot = (PotType*)calloc(1, sizeof(PotType));
        newPot->nextPot = NULL;
        newPot->previousPot = prevPot;
        newPot->potPosition = potNumber;
        potNumber++;

        if( prevPot != NULL)
        {
            prevPot->nextPot = newPot;
        }

        if(line[i] == '#')
        {
            newPot->potHasPlant_curr = true;
        }
        else if( line[i] == '.')
        {
            newPot->potHasPlant_curr = false;
        }
        else
        {
            printf("Error when reading in initial state!\n");
        }

        if( firstPotFound == false)
        {
            firstPot = newPot;
            firstPotFound = true;
        }

        prevPot = newPot;
    }
    
    return(firstPot);
}

void extractBreedRule(char *line, BreedRule *ruleCollection)
{
    static int ruleCount = 0;

    if( line[0] == '.' )
        ruleCollection[ruleCount].twoPrevState = false;
    else
        ruleCollection[ruleCount].twoPrevState = true;

    if( line[1] == '.' )
        ruleCollection[ruleCount].prevState = false;
    else
        ruleCollection[ruleCount].prevState = true;
    
    if( line[2] == '.' )
        ruleCollection[ruleCount].currState = false;
    else
        ruleCollection[ruleCount].currState = true;

    if( line[3] == '.' )
        ruleCollection[ruleCount].nextState = false;
    else
        ruleCollection[ruleCount].nextState = true;

    if( line[4] == '.' )
        ruleCollection[ruleCount].twoNextState = false;
    else
        ruleCollection[ruleCount].twoNextState = true;

    // result of the surrounding
    if( line[9] == '.' )
        ruleCollection[ruleCount].resultState = false;
    else
        ruleCollection[ruleCount].resultState = true;

    ruleCount++;
}

PotType* updateNeighbourInfos(PotType *firstPot)
{
    PotType *currentPot = firstPot;
    PotType *returnPot = firstPot;

    // add new element at the beginning in case there is a plant
    if( currentPot->potHasPlant_curr == true )
    {
        PotType* newPot = (PotType*)calloc(1, sizeof(PotType));
        newPot->nextPot = currentPot;
        newPot->previousPot = NULL;
        newPot->potPosition = currentPot->potPosition - 1;
        newPot->potHasPlant_curr = false;
        currentPot->previousPot = newPot;

        currentPot = newPot;
        returnPot = newPot;
    }

    do
    {
        // set all previous states
        if(currentPot->previousPot == NULL)
        {
            currentPot->prevState = false;
            currentPot->twoPrevState = false;
        }
        else
        {
            currentPot->prevState = currentPot->previousPot->potHasPlant_curr;
            currentPot->twoPrevState = currentPot->previousPot->prevState;
        }

        // set all next states
        if(currentPot->nextPot == NULL)
        {
            currentPot->nextState = false;
            currentPot->twoNextState = false;
        }
        else
        {
            currentPot->nextState = currentPot->nextPot->potHasPlant_curr;

            if( currentPot->nextPot->nextPot == NULL)
            {
                currentPot->twoNextState = false;
            }
            else
            {
                currentPot->twoNextState = currentPot->nextPot->nextPot->potHasPlant_curr;
            }
        }

        if( currentPot->nextPot == NULL )
        {
            if( currentPot->potHasPlant_curr == false
             && currentPot->prevState        == false
             && currentPot->twoPrevState     == false)
             {
                currentPot = currentPot->nextPot;
             }
             else
             {
                PotType* newPot = (PotType*)calloc(1, sizeof(PotType));
                newPot->previousPot = currentPot;
                newPot->nextPot = NULL;
                newPot->potPosition = currentPot->potPosition + 1;
                newPot->potHasPlant_curr = false;
                newPot->prevState = currentPot->potHasPlant_curr;
                newPot->twoPrevState = currentPot->prevState;
                currentPot->nextPot = newPot;

                currentPot = newPot;
             }
        }
        else
        {
            currentPot = currentPot->nextPot;
        }

    } while ( currentPot != NULL );

    return(returnPot);
}

bool compareRuleToState(PotType *currentPot, BreedRule *rule)
{
    bool return_val = false;

    if(    currentPot->potHasPlant_curr == rule->currState 
        && currentPot->twoPrevState     == rule->twoPrevState
        && currentPot->prevState        == rule->prevState
        && currentPot->nextState        == rule->nextState
        && currentPot->twoNextState     == rule->twoNextState)
    {
        return_val = true;
    }

    return(return_val);
}

void setNextGenStateToCurrentState(PotType *firstPot)
{
    PotType *currentPot = firstPot;
    
    while ( currentPot != NULL )
    {
        currentPot->potHasPlant_curr = currentPot->potHasPlant_nextGen;
        currentPot = currentPot->nextPot;
    }
}

void nextGenerationStep(PotType *firstPot, BreedRule *ruleCollection)
{
    PotType *currentPot = firstPot;

    do
    {
        bool rule_found = false;

        for (int i = 0; i < RULE_COUNT; i++)
        {
            if( compareRuleToState(currentPot, &ruleCollection[i] ) == true )
            {
                currentPot->potHasPlant_nextGen = ruleCollection[i].resultState;
                rule_found = true;
            }
        }

        if( rule_found == false)
        {
            currentPot->potHasPlant_nextGen = false;
        }

        currentPot = currentPot->nextPot;

    } while ( currentPot != NULL );

    setNextGenStateToCurrentState(firstPot);
}

void printPots(PotType *firstPot)
{
    PotType *currentPot = firstPot;
    
    while ( currentPot != NULL )
    {
        if( currentPot->potHasPlant_curr == true)
        {
            if( currentPot->potPosition == 0 )
                printf("X");
            else 
                printf("#");
        }
        else
        {
            if( currentPot->potPosition == 0 )
                printf("o");
            else
                printf(".");
        }

        currentPot = currentPot->nextPot;
    }
    printf("\n");
}

int calculateFinalResult(PotType *firstPot)
{
    PotType *currentPot = firstPot;
    int result = 0;

    while ( currentPot != NULL )
    {
        if( currentPot->potHasPlant_curr == true )
        {
            result += currentPot->potPosition;
        }
        currentPot = currentPot->nextPot;
    }

    return(result);
}

int main()
{
    FILE *fp;
    char line[LENGHT_LINE_MAX];
    PotType *firstPot = NULL;

    BreedRule ruleCollection[RULE_COUNT];

#if( TEST_RUN == 1 )
    fp = fopen("D:\\Creativity\\Advent_of_Code\\AoC_2018\\12_C\\12_01\\test.txt", "r");
#else
    fp = fopen("D:\\Creativity\\Advent_of_Code\\AoC_2018\\12_C\\12_01\\input.txt", "r");
#endif

    if( fp == NULL)
    {
        printf("Input file cannot be read!\n");
    }

    while(fgets(line, sizeof(line), fp) != NULL)
    {
        if(line[0] == 'i')
        {
            firstPot = getInitialState(&line[0]);
        }
        else if( (line[0] == '.') || (line[0] == '#') )
        {
            extractBreedRule(&line[0], &ruleCollection[0]);
        }
    }

    for (int i = 0; i < MAX_GENERATION; i++)
    {
        printf("%02d: ", i);
        printPots(firstPot);

        firstPot = updateNeighbourInfos(firstPot);
        nextGenerationStep(firstPot, &ruleCollection[0]);
    }

    printPots(firstPot);

    int result = calculateFinalResult(firstPot);
    printf("Sum of all pot numbers with plants: %d\n", result);

    return(0);
}