#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define TEST_RUN 0
#define LINE_LENGTH 49
#define ASCII_ALPHABET_START 65

#if( TEST_RUN == 1 )
    #define STEPS_TOTAL 6
    #define WORKERS_COUNT 2
    #define TIME_OFFSET 0
#else
    #define STEPS_TOTAL 26
    #define WORKERS_COUNT 5
    #define TIME_OFFSET 60
#endif

typedef enum 
{ 
    NOT_READY,
    READY, 
    ACTIVE, 
    DONE 
} stepStateType;

typedef struct step step;

struct step{
    step *dependencies[STEPS_TOTAL];
    int ID;

    stepStateType stepState;

    int start_time;
    int finish_time;
};

step * createNewStep(int stepID)
{
    step *newStep = malloc(sizeof(step));
    newStep->ID = stepID;

    newStep->stepState = NOT_READY;
    newStep->start_time = -1;
    newStep->finish_time = -1;

    memset(newStep->dependencies, 0, sizeof(step *) * STEPS_TOTAL);

    return(newStep);
}

void extractConditions(char *line, step **stepCollection)
{
    char ConditionChar[1];
    char DependencyChar[1];

    sscanf( line, "Step %c must be finished before step %c can begin.", ConditionChar, DependencyChar );

    int Condition_ID = (int)ConditionChar[0] - 65;
    int Dependency_ID = (int)DependencyChar[0] - 65;

    step *condition_step = stepCollection[Condition_ID];
    if(condition_step == NULL)
    {
        step * newCondition = createNewStep(Condition_ID);
        stepCollection[Condition_ID] = newCondition;

        condition_step = newCondition;
    }

    step *dependency_step = stepCollection[Dependency_ID];
    if(dependency_step == NULL)
    {
        step * newDependency = createNewStep(Dependency_ID);
        stepCollection[Dependency_ID] = newDependency;

        dependency_step = newDependency;
    }

    dependency_step->dependencies[Condition_ID] = condition_step;
}

void markReady(step **stepCollection)
{
    for (int i = 0; i < STEPS_TOTAL; i++)
    {
        int hasDependencies = 0;
        for (int j = 0; j < STEPS_TOTAL; j++)
        {
            if(stepCollection[i]->dependencies[j] != NULL)
            {
                if(stepCollection[i]->dependencies[j]->stepState != DONE)
                {
                    hasDependencies = 1;
                }
            }
        }

        if( (hasDependencies == 0) && 
        (stepCollection[i]->stepState != DONE) && 
        (stepCollection[i]->stepState != ACTIVE) )
        {
            stepCollection[i]->stepState = READY;
        }
    }
}

void setActive(step **stepCollection)
{

}

int countDoneFlags(step **stepCollection)
{
    int countDoneFlags = 0;

    for (int i = 0; i < STEPS_TOTAL; i++)
    {
        if( stepCollection[i]->stepState == DONE)
        {
            countDoneFlags++;
        }
    }

    return(countDoneFlags);
}

int evaluateGraph(step **stepCollection)
{
    int doneFlags = 0;
    int availableWorkers = WORKERS_COUNT;

    int currentTimestep = 0;

    while (doneFlags < STEPS_TOTAL)
    {
        markReady(stepCollection);

        bool graphChanged = false;
        int index = 0;


        while(graphChanged == false)
        {
            if( (currentTimestep == stepCollection[index]->finish_time) && 
            (stepCollection[index]->stepState == ACTIVE) )
            {
                stepCollection[index]->stepState = DONE;
                printf("%c", (stepCollection[index]->ID + 65) );
                availableWorkers++;
                graphChanged = true;
            }
            else if( (stepCollection[index]->stepState == READY) && (availableWorkers > 0) )
            {
                stepCollection[index]->start_time = currentTimestep;
                stepCollection[index]->finish_time = currentTimestep + index + 1 + TIME_OFFSET;
                stepCollection[index]->stepState = ACTIVE;
                availableWorkers--;
                graphChanged = true;
            }
            else
            {
                index++;
            }

            if( index == STEPS_TOTAL)
            {
                currentTimestep++;
                graphChanged = true;
            }
        }
        doneFlags = countDoneFlags(stepCollection);
    }

    return(currentTimestep);
}

int main(void)
{
    FILE *fp;
    char line[LINE_LENGTH];

    step *stepCollection[STEPS_TOTAL];
    memset(stepCollection, 0, sizeof(step *) * 26);

#if (TEST_RUN == 1)
    fp = fopen("D:\\Creativity\\Advent_of_Code\\AoC_2018\\07_C\\07_02\\test.txt", "r");
#else
    fp = fopen("D:\\Creativity\\Advent_of_Code\\AoC_2018\\07_C\\07_02\\input.txt", "r");
#endif

    if( fp == NULL)
    {
        printf("File could not be read!\n");
        exit(1);
    }

    while(fgets(line, sizeof(line), fp) != NULL)
    {
        extractConditions(&line[0], &stepCollection[0]);
    }

    int total_time = evaluateGraph(&stepCollection[0]);

    printf("\nDuration: %d\n", total_time);


    return(0);
}