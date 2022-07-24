#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <openssl/sha.h>

//#include "project.h"

char alphabets[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 ";

bool nextCode(int startingPoint, int currentPoint, int length, int maxNumber, int *array, unsigned char *encodedEntry)
{
    bool found = false;

    if (currentPoint > startingPoint)
    {
        int i = 0;

        /* Debug Code
        printf("\n");
        for (i = 0; i < SHA256_DIGEST_LENGTH; i++)
        {
            printf("%02x", encodedEntry[i]);
        }
        printf("\n");
        */

        for (int i = 0; i < maxNumber && !found; i++)
        {
            array[currentPoint - 1] = i;
            if (currentPoint > startingPoint)
            {
                found = nextCode(startingPoint, currentPoint - 1, length, maxNumber, array, encodedEntry);
            }

            if ((currentPoint - 1) == startingPoint)
            {
                uint8_t generatedPassword[length + 1];
                uint8_t generatedPasswordCopy[length + 1];

                int j = 0;
                for (j = 0; j < length; j++)
                {
                    // Debug Code
                    printf("%d ", array[j]);
                    generatedPassword[j] = alphabets[array[j]];
                }
                generatedPassword[length] = '\0';

                printf("Testing: %s\n", generatedPassword);
                /*
                strcpy(generatedPasswordCopy, generatedPassword);

                unsigned char hashFromGenerated[SHA256_DIGEST_LENGTH];

                SHA256_CTX context;
                SHA256_Init(&context);
                SHA256_Update(&context, (unsigned char *)generatedPassword, length);
                SHA256_Final(hashFromGenerated, &context);

                bool same = true;
                for (j = 0; j < SHA256_DIGEST_LENGTH && same; j++)
                {
                    if (encodedEntry[j] != hashFromGenerated[j])
                    {
                        same = false;
                    }
                }
                if (same)
                {

                    // Found it!
                    printf("\n%.*s is the one! hashes are:\n", length, generatedPasswordCopy);

                    printf("\nSource: ");
                    for (j = 0; j < SHA256_DIGEST_LENGTH; j++)
                    {
                        printf("%02x", encodedEntry[j]);
                    }
                    printf("\n");

                    printf("\nGenerated: ");
                    for (j = 0; j < SHA256_DIGEST_LENGTH; j++)
                    {
                        printf("%02x", hashFromGenerated[j]);
                    }
                    printf("\n");

                    found = same;
                }*/
                printf("\n");
            }
        }
        printf("\n");
    }
    return found;
}

bool addOne(int currentPoint, int length, int maxNumber, int *array)
{
    bool maxReached = false;
    if (currentPoint >= 0)
    {
        if (array[currentPoint - 1] < maxNumber)
        {

            array[currentPoint - 1]++;

            if (array[currentPoint - 1] >= maxNumber)
            {
                if ((currentPoint - 1) >= 0)
                {
                    array[currentPoint - 1] = 0;
                    maxReached = addOne((currentPoint - 1), length, maxNumber, array);
                }
            }
        }
        else
        {
            maxReached = true;
        }
    }

    return maxReached;
}

int main()
{
    int startingPoint = 0;    
    int length = 1;
    int endingPoint = length;
    
    if (length > 3){
        endingPoint = length - 3;
    }    
    //int endingPoint = 1;

    int alphaCount = 5;

    int passwordMap[length];

    unsigned char hashFromUser[10];

    bool found = false;

    for (int i = 0; i < length; i++)
    {
        passwordMap[i] = 0;
    }

    int i = 0;
    int j = 0;
    do
    {

        // passwordMap[0] = i;
        // passwordMap[1] = j;

        //found = nextCode(startingPoint, endingPoint, length, alphaCount, passwordMap, hashFromUser);

        if (!found)
        {
            bool cycleDone = false;
            for (int i = 0; i < length; i++)
            {
                printf("%d ", passwordMap[i]);
            }
            printf("\n");

            if (length <= 3) {
                found = nextCode(startingPoint, endingPoint, length, alphaCount, passwordMap, hashFromUser);
            } else {
                found = nextCode(endingPoint, length, length, alphaCount, passwordMap, hashFromUser);
            }
            while(!cycleDone)
            {
                cycleDone = addOne(endingPoint, length, alphaCount, passwordMap);
                for (int i = 0; i < length; i++)
                {
                    printf("%d ", passwordMap[i]);
                }
                printf("\n");
                found = nextCode(endingPoint, length, length, alphaCount, passwordMap, hashFromUser);
            }

            found = true;
        }
        /*
        if (!found)
        {

            if (j >= (26 - 1))
            {
                j = 0;
                i++;
            }
            else
            {
                j++;
            }

            if (i >(26 - 1))
            {
                found = true;
            }
        }*/
    } while (!found);
}