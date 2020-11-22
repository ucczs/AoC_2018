#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define TEST_RUN 0

#if( TEST_RUN == 1)
    #define DESIRED_NUMBER_1 5
    #define DESIRED_NUMBER_2 1
    #define DESIRED_NUMBER_3 5
    #define DESIRED_NUMBER_4 8
    #define DESIRED_NUMBER_5 9
    #define DESIRED_NUMBER_6 -1
    #define DIGIT_COUNT_COMBINATION 5
    #define EXPECTED_RESULT 9
#elif( TEST_RUN == 2)
    #define DESIRED_NUMBER_1 0
    #define DESIRED_NUMBER_2 1
    #define DESIRED_NUMBER_3 2
    #define DESIRED_NUMBER_4 4
    #define DESIRED_NUMBER_5 5
    #define DESIRED_NUMBER_6 -1
    #define DIGIT_COUNT_COMBINATION 5
    #define EXPECTED_RESULT 5
#elif( TEST_RUN == 3)
    #define DESIRED_NUMBER_1 9
    #define DESIRED_NUMBER_2 2
    #define DESIRED_NUMBER_3 5
    #define DESIRED_NUMBER_4 1
    #define DESIRED_NUMBER_5 0
    #define DESIRED_NUMBER_6 -1
    #define DIGIT_COUNT_COMBINATION 5
    #define EXPECTED_RESULT 18
#elif( TEST_RUN == 4)
    #define DESIRED_NUMBER_1 5
    #define DESIRED_NUMBER_2 9
    #define DESIRED_NUMBER_3 4
    #define DESIRED_NUMBER_4 1
    #define DESIRED_NUMBER_5 4
    #define DESIRED_NUMBER_6 -1
    #define DIGIT_COUNT_COMBINATION 5
    #define EXPECTED_RESULT 2018
#else
    #define DESIRED_NUMBER_1 2
    #define DESIRED_NUMBER_2 8
    #define DESIRED_NUMBER_3 6
    #define DESIRED_NUMBER_4 0
    #define DESIRED_NUMBER_5 5
    #define DESIRED_NUMBER_6 1
    #define DIGIT_COUNT_COMBINATION 6
    #define EXPECTED_RESULT 0
#endif

typedef struct recipe recipe;

struct recipe{
    int score;
    int recipe_count;
    recipe* nextRecipe;
};

typedef struct {
    recipe *rootRecipe;
    recipe *lastRecipe;

    recipe *combinationCheckStart;

    recipe *firstElfsRecipe;
    recipe *secondElfsRecipe;

    int combinationStartRecipeCount;

    int currentNumberRecipe;
}recipeCollectionType;


recipe* createNewrecipe(int input1, int input2)
{
    int sum = input1 + input2;
    recipe* generatedRecipes;

    if (sum > 9)
    {
        generatedRecipes = (recipe*)calloc(2, sizeof(recipe));
        generatedRecipes[0].score = (int)(sum / 10);
        generatedRecipes[0].nextRecipe = &generatedRecipes[1];

        generatedRecipes[1].score = (sum % 10);
    }
    else
    {
        generatedRecipes = (recipe*)calloc(1, sizeof(recipe));
        generatedRecipes[0].score = (sum % 10);
    }

    return(generatedRecipes);
}

void moveElvesForward(recipeCollectionType *recipeCollection)
{
    int firstElfForwardSteps = recipeCollection->firstElfsRecipe->score + 1;
    int secondElfForwardSteps = recipeCollection->secondElfsRecipe->score + 1;

    for (size_t i = 0; i < firstElfForwardSteps; i++)
    {
        recipeCollection->firstElfsRecipe = recipeCollection->firstElfsRecipe->nextRecipe;
    }
    
    for (size_t i = 0; i < secondElfForwardSteps; i++)
    {
        recipeCollection->secondElfsRecipe = recipeCollection->secondElfsRecipe->nextRecipe;
    }
}

void performNextStep(recipeCollectionType *recipeCollection)
{
    int firstPartNewRecipe = recipeCollection->firstElfsRecipe->score;
    int secondPartNewRecipe = recipeCollection->secondElfsRecipe->score;

    recipe* generatedRecipe1 = createNewrecipe(firstPartNewRecipe, secondPartNewRecipe);

    recipeCollection->lastRecipe->nextRecipe = generatedRecipe1;
    recipeCollection->currentNumberRecipe++;
    generatedRecipe1->recipe_count = recipeCollection->currentNumberRecipe;
    recipeCollection->lastRecipe = generatedRecipe1;

    if( firstPartNewRecipe + secondPartNewRecipe > 9 )
    {
        recipe* generatedRecipe2 = &generatedRecipe1[1];
        recipeCollection->currentNumberRecipe++;
        generatedRecipe2->recipe_count = recipeCollection->currentNumberRecipe;
        recipeCollection->lastRecipe = generatedRecipe2;
    }

    recipeCollection->lastRecipe->nextRecipe = recipeCollection->rootRecipe;

    // adapt pointer to combinationStartRecipe
    if( recipeCollection->currentNumberRecipe == DIGIT_COUNT_COMBINATION)
    {
        recipeCollection->combinationCheckStart = recipeCollection->rootRecipe;
    }
    else if( (recipeCollection->currentNumberRecipe == DIGIT_COUNT_COMBINATION + 1) && (recipeCollection->combinationCheckStart == NULL))
    {
        recipeCollection->combinationCheckStart = recipeCollection->rootRecipe->nextRecipe;
    }
    else if( recipeCollection->currentNumberRecipe > DIGIT_COUNT_COMBINATION)
    {
        recipeCollection->combinationCheckStart = recipeCollection->combinationCheckStart->nextRecipe;
    }

    moveElvesForward(recipeCollection);
}

void printRecipes(recipeCollectionType *recipeCollection)
{
    recipe *currentRecipe = recipeCollection->rootRecipe;

    while ( currentRecipe != recipeCollection->lastRecipe)
    {
        printf("%d  ", currentRecipe->score);
        currentRecipe = currentRecipe->nextRecipe;
    }

    printf("%d  \n", recipeCollection->lastRecipe->score);
}


void printLastNRecipes(recipeCollectionType *recipeCollection, int lastX)
{
    recipe *recipeToPrint = recipeCollection->rootRecipe;
    
    for (int i = 0; i < recipeCollection->currentNumberRecipe - DIGIT_COUNT_COMBINATION; i++)
    {
        recipeToPrint = recipeToPrint->nextRecipe;
    }

    for (int i = 0; i < lastX; i++)
    {
        printf("%d", recipeToPrint->score);
        recipeToPrint = recipeToPrint->nextRecipe;
    }

    printf("\n");
}

bool checkForCombination(recipeCollectionType *recipeCollection)
{
    bool return_val;

    if( recipeCollection->currentNumberRecipe >= DIGIT_COUNT_COMBINATION)
    {
        int number_1 = recipeCollection->combinationCheckStart->score;
        int number_2 = recipeCollection->combinationCheckStart->nextRecipe->score;
        int number_3 = recipeCollection->combinationCheckStart->nextRecipe->nextRecipe->score;
        int number_4 = recipeCollection->combinationCheckStart->nextRecipe->nextRecipe->nextRecipe->score;
        int number_5 = recipeCollection->combinationCheckStart->nextRecipe->nextRecipe->nextRecipe->nextRecipe->score;
        int number_6 = recipeCollection->combinationCheckStart->nextRecipe->nextRecipe->nextRecipe->nextRecipe->nextRecipe->score;

        if( DESIRED_NUMBER_1 == number_1 &&
            DESIRED_NUMBER_2 == number_2 &&
            DESIRED_NUMBER_3 == number_3 &&
            DESIRED_NUMBER_4 == number_4 &&
            DESIRED_NUMBER_5 == number_5)
        {
            bool number_6 = true;
            if((DIGIT_COUNT_COMBINATION == 6) && (DESIRED_NUMBER_6 != number_6))
            {
                number_6 = false;
            }

            if( number_6 == true)
            {
                recipeCollection->combinationStartRecipeCount = recipeCollection->combinationCheckStart->recipe_count - 1;
                return_val = true;
            }
        }
    }
    else
    {
        return_val = false;
    }

    return(return_val);
}

int main()
{
    recipe rootRecipe;
    rootRecipe.recipe_count = 1;
    rootRecipe.score = 3;

    recipe secondRecipe;
    secondRecipe.recipe_count = 2;
    secondRecipe.score = 7;

    rootRecipe.nextRecipe = &secondRecipe;
    secondRecipe.nextRecipe = &rootRecipe;

    recipeCollectionType recipeCollection;
    recipeCollection.currentNumberRecipe = 2;

    recipeCollection.rootRecipe = &rootRecipe;
    recipeCollection.lastRecipe = &secondRecipe;

    recipeCollection.firstElfsRecipe = &rootRecipe;
    recipeCollection.secondElfsRecipe = &secondRecipe;

    recipeCollection.combinationCheckStart = NULL;

    while( !checkForCombination(&recipeCollection) )
    {
        performNextStep(&recipeCollection);
        //printRecipes(&recipeCollection);
    }
    
    printLastNRecipes(&recipeCollection, DIGIT_COUNT_COMBINATION);
    printf("%d -> Recipe count until combination\n", recipeCollection.combinationStartRecipeCount);
    printf("%d -> expected\n", EXPECTED_RESULT);

    return(0);
}
