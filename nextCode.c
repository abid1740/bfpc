#include <stdio.h>
#include <string.h>

char alphabets[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

void nextCode(int currentPoint, int length, int maxNumber, int *array)
{
    if (currentPoint > 0)
    {
        for (int i = 0; i < maxNumber; i++)
        {
            array[currentPoint-1] = i;
            if (currentPoint > 0)
            {
                nextCode(currentPoint - 1, length, maxNumber, array);
            }
            
            if ((currentPoint-1) == 0)
            {
                char generatedPassword[length];

                for (int j = 0; j < length; j++)
                {
                    //printf("%d ", array[j]);
                    generatedPassword[j] = alphabets[array[j]];
                }
                printf("\n%.*s", length, generatedPassword );
                printf("\n");
            }
        }
        printf("\n");
    }
}

int main()
{
    int length = 50;

    int maxNumber = 26;

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
    nextCode(4, 4, sizeof(alphabets), arrayOfNumbers);

    return 0;
}