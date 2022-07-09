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

int main()
{
    int startingPoint = 2;
    int endingPoint = 6;
    int length = 6;

    int passwordMap[length];

    unsigned char hashFromUser[10];

    bool found = false;

    for (int i = 0; i < length; i++)
    {
        passwordMap[i] = -1;
    }

    int i = 0;
    int j = 0;
    do
    {
        passwordMap[0] = i;
        passwordMap[1] = j;

        found = nextCode(startingPoint, endingPoint, length, 26, passwordMap, hashFromUser);

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
        }
    } while (!found);
}