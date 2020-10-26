#include <stdio.h>
#include <stdlib.h>

#define TEST_RUN 0
#define MAX_LINE_LENGTH 45
#define RELEVANT_MINUTES 60

typedef struct{
    int day;
    int month;
    int year;
    int hour;
    int minute;
}actionDate;

typedef enum {
    ShiftStart = 1,
    FallAsleep = 2,
    WakeUp = 3
} actionType;

typedef struct{
    actionDate date;
    int guardID;
    actionType action;
} actionSet;

typedef struct{
    actionSet *array;
    int size;
    int used;
} actionSetList;

typedef struct{
    int guardID;
    int minutesOfSleep;
    int minuteStatistic[RELEVANT_MINUTES];
} guardStatistic;

typedef struct{
    guardStatistic *array;
    int size;
    int used;
} guardStatisticList;

void init_guardStatisticList(guardStatisticList *a, int initialSize)
{
    a->array = (actionSet *) malloc(initialSize * sizeof(guardStatistic));
    a->used = 0;
    a->size = initialSize;
}

void insertGuardStatisticToList(guardStatisticList *a, guardStatistic element)
{
    if (a->used == a->size)
    {
        a->size *= 2;
        a->array = realloc(a->array, a->size * sizeof(guardStatistic));
    }
    a->array[a->used++] = element;
}

void init_actionSetList(actionSetList *a, int initialSize)
{
    a->array = (actionSet *) malloc(initialSize * sizeof(actionSet));
    a->used = 0;
    a->size = initialSize;
}

void insertActionSetToList(actionSetList *a, actionSet element)
{
    if (a->used == a->size)
    {
        a->size *= 2;
        a->array = realloc(a->array, a->size * sizeof(actionSet));
    }
    a->array[a->used++] = element;
}

int getNumberInChar(char *line, int start_idx, int end_idx)
{
    int number = 0;
    for (int i = start_idx; i <= end_idx; i++)
    {
        number *= 10;
        number += (line[i] - '0');
    }
    return(number);
}

void extractDateFromLine(char *line, actionSet *actionSet)
{
    actionSet->date.year = getNumberInChar(line, 1, 4);
    actionSet->date.month = getNumberInChar(line, 6, 7);
    actionSet->date.day = getNumberInChar(line, 9, 10);
    actionSet->date.hour = getNumberInChar(line, 12, 13);
    actionSet->date.minute = getNumberInChar(line, 15, 16);
}

int extractGuardID(char *line)
{
    int line_idx = 26;
    int result = 0;

    while (line[line_idx] != ' ')
    {
        result *= 10;
        result += (line[line_idx] - '0');
        line_idx++;
    }
    return(result);
}

void extractActionFromLine(char *line, actionSet *actionSet)
{
    if( line[19] == 'f')
    {
        actionSet->action = FallAsleep;
    }
    else if( line[19] == 'w')
    {
        actionSet->action = WakeUp;
    }
    else if( line[19] == 'G')
    {
        actionSet->action = ShiftStart;
        actionSet->guardID = extractGuardID(line);
    }
    else
    {
        printf("Error during Action extracting!\n");
    }
}

void processInputLine(char *line, actionSetList *actionSetList)
{
    actionSet newActionSet;

    newActionSet.guardID = 0;
    extractDateFromLine(line, &newActionSet);
    extractActionFromLine(line, &newActionSet);

    insertActionSetToList(actionSetList, newActionSet);
}

int comperator_fct(const void *v1, const void *v2)
{
    const actionSet *p1 = (actionSet *)v1;
    const actionSet *p2 = (actionSet *)v2;
    if (p1->date.month < p2->date.month)
    {
        return -1;
    }
    else if (p1->date.month > p2->date.month)
    {
        return +1;
    }
    else
    {
        if(p1->date.day < p2->date.day)
        {
            return -1;
        }
        else if(p1->date.day > p2->date.day)
        {
            return +1;
        }
        else
        {
            if(p1->date.hour < p2->date.hour)
            {
                return -1;
            }
            else if(p1->date.hour > p2->date.hour)
            {
                return +1;
            }
            else
            {
                if(p1->date.minute < p2->date.minute)
                {
                    return -1;
                }
                else if(p1->date.minute > p2->date.minute)
                {
                    return +1;
                }
                else
                {
                    return 0;
                }
            }
        }
    }

    return 0;
}

void sortActionListByDate(actionSetList *actionSetList)
{
    qsort(actionSetList->array, actionSetList->used, sizeof(actionSet), comperator_fct);
}

guardStatistic* getGuard(guardStatisticList *guardList, int guardID)
{
    guardStatistic* foundGuard = NULL;

    for (int i = 0; i < guardList->used; i++)
    {
        if (guardList->array[i].guardID == guardID)
        {
            foundGuard = &(guardList->array[i]);
        }
    }
    return(foundGuard);
}

void fillOutSleepingMinutes(guardStatistic *sleepingGuard, int startMin, int StopMin)
{
    for (int i = startMin; i < StopMin; i++)
    {
        sleepingGuard->minuteStatistic[i]++;
    }
}

void calculateTotalSleepingMinutes(guardStatistic *sleepingGuard)
{
    int sleepingSum = 0;
    for (int i = 0; i < RELEVANT_MINUTES; i++)
    {
        sleepingSum += sleepingGuard->minuteStatistic[i];
    }

    sleepingGuard->minutesOfSleep = sleepingSum;
}

void getGuardStatistic(actionSetList *actionList, guardStatisticList *guardList)
{
    guardStatistic* foundGuard = NULL;

    for (int i = 0; i < actionList->used; i++)
    {
        actionSet currentAction = actionList->array[i];
        foundGuard = getGuard(guardList, currentAction.guardID);

        // create guard if it doesn't exist
        if( foundGuard == NULL)
        {
            guardStatistic newGuard;
            memset( newGuard.minuteStatistic, 0, sizeof(newGuard.minuteStatistic) );
            newGuard.guardID = currentAction.guardID;
            insertGuardStatisticToList(guardList, newGuard);

            foundGuard = getGuard(guardList, currentAction.guardID);
        }

        if(currentAction.action == FallAsleep)
        {
            int startSleepingMin = currentAction.date.minute;
            int stopSleepingMin = actionList->array[i+1].date.minute;
            fillOutSleepingMinutes(foundGuard, startSleepingMin, stopSleepingMin);
        }
    }

    for (int i = 0; i < guardList->used; i++)
    {
        calculateTotalSleepingMinutes( &(guardList->array[i]) );
    }
    
}

void addGuardIDForEachAction(actionSetList *actionSetList)
{
    int currentGuard = 0;

    for (int i = 0; i < actionSetList->used; i++)
    {
        if(actionSetList->array[i].action == ShiftStart)
        {
            currentGuard = actionSetList->array[i].guardID;
        }
        else
        {
            actionSetList->array[i].guardID = currentGuard;
        }
    }
}

int getHighSleepingMinute(guardStatistic *sleepingGuard)
{
    int highSleepingMin = 0;
    int minutesSleepInThatMinute = 0;

    for (int i = 0; i < RELEVANT_MINUTES; i++)
    {
        if( sleepingGuard->minuteStatistic[i] > minutesSleepInThatMinute)
        {
            highSleepingMin = i;
            minutesSleepInThatMinute = sleepingGuard->minuteStatistic[i];
        }
    }
    return(highSleepingMin);
}

void findWorstGuard(guardStatisticList *guardList, int *maxSleepGuardID, int *HighSleepingMinute)
{
    int maxSleep = 0;

    for (int i = 0; i < guardList->used; i++)
    {
        if(guardList->array[i].minutesOfSleep > maxSleep)
        {
            maxSleep = guardList->array[i].minutesOfSleep;
            *maxSleepGuardID = guardList->array[i].guardID;
        }
    }

    guardStatistic *foundGuard = getGuard(guardList, *maxSleepGuardID);
    *HighSleepingMinute = getHighSleepingMinute(foundGuard);
}

int main(void)
{
    FILE *fp;
    char line[MAX_LINE_LENGTH];

#if (TEST_RUN == 1)
    fopen("D:\\Creativity\\Advent_of_Code\\AoC_2018\\04_C\\04_01\\test.txt", "r");
#else
    fp = fopen("D:\\Creativity\\Advent_of_Code\\AoC_2018\\04_C\\04_01\\input.txt", "r");
#endif

    if(fp == NULL)
    {
        printf("File could not be open!");
        exit(1);
    }

    actionSetList actionList;
    init_actionSetList(&actionList, 100);

    while(fgets(line, sizeof(line), fp) != NULL)
    {
        processInputLine(&line[0], &actionList);
    }

    sortActionListByDate(&actionList);
    addGuardIDForEachAction(&actionList);

    guardStatisticList guardList;
    init_guardStatisticList(&guardList, 20);
    getGuardStatistic(&actionList, &guardList);

    int mostSleepyGuardID;
    int highSleepingMinute;
    findWorstGuard(&guardList, &mostSleepyGuardID, &highSleepingMinute);

    int sum = mostSleepyGuardID * highSleepingMinute;

    printf("Sum: %d\n", sum);

    return(0);
}