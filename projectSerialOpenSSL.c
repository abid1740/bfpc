/***********************************************************************************************************************
 * Name:        Password Cracker (Serial)
 * Programmer:  Samson Chung
 * Date:        July 7th, 2022
 * Description: This is the serial version of our project's password Cracker using OpenSSL
 *
 *              Asks the user to enter a password
 *              Hash their password using SHA256
 *              timeStarted = now()
 *              do
 *                  generate a password from Alphabet String
 *                  hash generatedPassword to generatedHash
 *              while( hash != generatedHash)
 *              timeEnded = now()
 *
 *              display password from user, generated password, and time it took
 *
 **********************************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/bio.h>
#include <openssl/sha.h>

char alphabets[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 ";
// char alphabets[] = "ABCD";
// uint8_t iv[16] = {0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff};
// char key[] = "secret key 123";

void nextCode(int currentPoint, int length, int maxNumber, int *array, unsigned char *encodedEntry)
{
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

        for (int i = 0; i < maxNumber; i++)
        {
            array[currentPoint - 1] = i;
            if (currentPoint > 0)
            {
                nextCode(currentPoint - 1, length, maxNumber, array, encodedEntry);
            }

            if ((currentPoint - 1) == 0)
            {
                uint8_t generatedPassword[length + 1];
                uint8_t generatedPasswordCopy[length + 1];

                int j = 0;
                for (j = 0; j < length; j++)
                {
                    // Debug Code
                    //printf("%d ", array[j]);
                    generatedPassword[j] = alphabets[array[j]];
                }
                generatedPassword[length] = '\0';

                printf("Testing: %s\n", generatedPassword);
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
                    
                    exit(0);
                }
                // printf("\n");
            }
        }
        // printf("\n");
    }
}
int main(int argc, char *argv[])
{
    int const LENGTH = 100;
    int i = 0;
    int letters = 0;

    int test[LENGTH];

    char *userEntry = NULL;
    unsigned char hashFromUser[SHA256_DIGEST_LENGTH];

    int generatePassword[LENGTH];

    int lenAlphabets = strlen(alphabets);

    bool same = false;

    printf("Please enter a password (max size is %d): ", LENGTH);
    scanf("%m[^\n]%*c", &userEntry);

    size_t length = strlen((const char *)userEntry);

    if (length < 100)
    {

        SHA256_CTX context;
        SHA256_Init(&context);
        SHA256_Update(&context, (unsigned char *)userEntry, length);
        SHA256_Final(hashFromUser, &context);

        printf("\n");
        for (i = 0; i < SHA256_DIGEST_LENGTH; i++)
        {
            printf("%02x", hashFromUser[i]);
        }
        printf("\n");

        // initializes the generatePassword Array
        for (i = 0; i < LENGTH; i++)
        {
            generatePassword[i] = -1;
        }

        for (letters = 1; letters <= LENGTH; letters++)
        {
            nextCode(letters, letters, lenAlphabets, generatePassword, hashFromUser);
            printf("\n");
        }
        /*
        do
        {



            //AES_init_ctx_iv(&ctx, key, iv);
            //AES_CTR_xcrypt_buffer(&ctx, test, strlen((char *)test));


        } while(!same);
        */
        // printf("\nDEC: %s\n", (char *)test); // don't use this string as an input
    }
    return 0;
}