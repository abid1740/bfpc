
#include "project.h"

char alphabets[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 ";

bool nextCode(int currentPoint, int length, int maxNumber, int *array, unsigned char *encodedEntry)
{
    bool found = false;
    
    if (currentPoint > 0)
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
            if (currentPoint > 0)
            {
                found = nextCode(currentPoint - 1, length, maxNumber, array, encodedEntry);
            }

            if ((currentPoint - 1) == 0)
            {
                uint8_t generatedPassword[length + 1];
                uint8_t generatedPasswordCopy[length + 1];

                int j = 0;
                for (j = 0; j < length; j++)
                {
                    // Debug Code
                    // printf("%d ", array[j]);
                    generatedPassword[j] = alphabets[array[j]];
                }
                generatedPassword[length] = '\0';

                //printf("Testing: %s\n", generatedPassword);
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
                }
                // printf("\n");
            }
        }
        // printf("\n");
    }
    return found;
}