#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define TEST_RUN 0
#define LINE_LENGTH 49
#define ASCII_ALPHABET_START 65

#if( TEST_RUN == 1)
    #define STEPS_TOTAL 6
#else
    #define STEPS_TOTAL 26
#endif

typedef struct step step;

struct step{
    step *dependencies[STEPS_TOTAL];

    int ID;
    int activeFlag;
    int doneFlag;
};

step * createNewStep(stepID)
{
    step *newStep = malloc(sizeof(step));
    newStep->ID = stepID;
    newStep->activeFlag = 0;
    newStep->doneFlag = 0;

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

void markActive(step **stepCollection)
{
    for (int i = 0; i < STEPS_TOTAL; i++)
    {
        int hasDependencies = 0;
        for (int j = 0; j < STEPS_TOTAL; j++)
        {
            if(stepCollection[i]->dependencies[j] != NULL)
            {
                if(stepCollection[i]->dependencies[j]->doneFlag == 0)
                {
                    hasDependencies = 1;
                }
            }
        }

        if( (hasDependencies == 0) && (stepCollection[i]->doneFlag == 0) )
        {
            stepCollection[i]->activeFlag = 1;
        }
    }
}

int countactiveFlags(step **stepCollection)
{
    int activeCounter = 0;

    for (int i = 0; i < STEPS_TOTAL; i++)
    {
        activeCounter += stepCollection[i]->activeFlag;
    }

    return activeCounter;
}

int countDoneFlags(step **stepCollection)
{
    int countDoneFlags = 0;

    for (int i = 0; i < STEPS_TOTAL; i++)
    {
        countDoneFlags += stepCollection[i]->doneFlag;
    }

    return(countDoneFlags);
}

void evaluateGraph(step **stepCollection)
{
    int doneFlags = 0;

    while (doneFlags < STEPS_TOTAL)
    {
        markActive(stepCollection);

        bool graphChanged = false;
        int index = 0;

        while(graphChanged == false)
        {
            if(stepCollection[index]->activeFlag == 1)
            {
                printf("%c", (stepCollection[index]->ID + 65) );
                stepCollection[index]->doneFlag = 1;
                stepCollection[index]->activeFlag = 0;
                graphChanged = true;
            }
            else
            {
                index++;
            }
        }
        doneFlags = countDoneFlags(stepCollection);
    }
}

int main(void)
{
    FILE *fp;
    char line[LINE_LENGTH];

    step *stepCollection[STEPS_TOTAL];
    memset(stepCollection, 0, sizeof(step *) * 26);

#if (TEST_RUN == 1)
    fp = fopen("D:\\Creativity\\Advent_of_Code\\AoC_2018\\07_C\\07_01\\test.txt", "r");
#else
    fp = fopen("D:\\Creativity\\Advent_of_Code\\AoC_2018\\07_C\\07_01\\input.txt", "r");
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

    evaluateGraph(&stepCollection[0]);


    return(0);
}