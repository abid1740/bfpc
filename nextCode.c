#include <stdio.h>

void nextCode(int currentPoint, int length, int maxNumber, int *array)
{
    if (length > 0)
    {
        for (int i = 0; i < maxNumber; i++)
        {
            array[currentPoint] = i;
            if (currentPoint > 0)
            {
                nextCode(currentPoint - 1, length, maxNumber, array);
            }
            
            if (currentPoint == 0)
            {
                for (int j = 0; j < length; j++)
                {
                    printf("%d ", array[j]);
                }
                printf("\n");
            }
        }
        printf("\n");
    }
}

int main()
{
    int length = 3;

    int maxNumber = 5;

    int arrayOfNumbers[length];

    int i = 0;

    /*
    for (i = 0; i < maxNumber; i++){
        arrayOfNumbers[i] = i;
        for (int j = 0; j < maxNumber; j++){
            arrayOfNumbers[j] = j;
            for (int k = 0; k < maxNumber; k++) {
                arrayOfNumbers[k] = k;

                printf("%d %d %d\n", arrayOfNumbers[i], arrayOfNumbers[j], arrayOfNumbers[k]);
            }
        }
    }
    */
    /*
    for (i = 0; i <= length; i++)
    {
        nextCode(i, i, maxNumber, arrayOfNumbers);
    }
    */
    nextCode(2, 2, maxNumber, arrayOfNumbers);

    return 0;
}