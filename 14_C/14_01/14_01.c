#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define TEST_RUN 0

#if( TEST_RUN == 1)
    #define RECIPE_COUNT 9
    #define EXPECTED_RESULT 5158916779
#elif( TEST_RUN == 2)
    #define RECIPE_COUNT 5
    #define EXPECTED_RESULT 124515891
#elif( TEST_RUN == 3)
    #define RECIPE_COUNT 18
    #define EXPECTED_RESULT 9251071085
#elif( TEST_RUN == 4)
    #define RECIPE_COUNT 2018
    #define EXPECTED_RESULT 5941429882
#else
    #define RECIPE_COUNT 286051
    #define EXPECTED_RESULT 0
#endif

#define RESULT_RECIPE_COUNT 10

typedef struct recipe recipe;

struct recipe{
    int score;
    int recipe_count;
    recipe* nextRecipe;
};

typedef struct {
    recipe *rootRecipe;
    recipe *lastRecipe;

    recipe *firstElfsRecipe;
    recipe *secondElfsRecipe;

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
    
    for (int i = 0; i < RECIPE_COUNT; i++)
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

    while( recipeCollection.currentNumberRecipe < RECIPE_COUNT + RESULT_RECIPE_COUNT )
    {
        performNextStep(&recipeCollection);
        //printRecipes(&recipeCollection);
    }
    
    printLastNRecipes(&recipeCollection, RESULT_RECIPE_COUNT);
    printf("%lld -> expected", EXPECTED_RESULT);

    return(0);
}
