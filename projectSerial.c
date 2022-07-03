/***********************************************************************************************************************
 * Name:        Password Cracker (Serial)
 * Programmer:  Samson Chung
 * Date:        Jun 27th, 2022
 * Description: This is the serial version of our project's password Cracker
 *
 *              Loads Key from file into memory
 *              Asks the user to enter a password
 *              encrypts the password using AES to hash
 *              timeStarted = now()
 *              do
 *                  generate a password from Alphabet String
 *                  encrypts generatedPassword to generatedHash
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
#include "external/tiny-AES-c/aes.h"

char alphabets[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 ";
//char alphabets[] = "ABCD";
uint8_t iv[16] = {0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff};
char key[] = "secret key 123";
    

void nextCode(int currentPoint, int length, int maxNumber, int *array, uint8_t * encodedEntry)
{
    if (currentPoint > 0)
    {
        for (int i = 0; i < maxNumber; i++)
        {
            array[currentPoint-1] = i;
            if (currentPoint > 0)
            {
                nextCode(currentPoint - 1, length, maxNumber, array, encodedEntry);
            }
            
            if ((currentPoint-1) == 0)
            {
                uint8_t generatedPassword[length+1];
                uint8_t generatedPasswordCopy[length+1];

                for (int j = 0; j < length; j++)
                {
                    //printf("%d ", array[j]);
                    generatedPassword[j] = alphabets[array[j]];
                }
                generatedPassword[length] = '\0';

                printf("Testing: %s\n",  generatedPassword );
                strcpy(generatedPasswordCopy, generatedPassword);

                struct AES_ctx ctx;
                AES_init_ctx_iv(&ctx, key, iv);
                AES_CTR_xcrypt_buffer(&ctx, generatedPassword, strlen((char *)generatedPassword));

                if (strcmp(generatedPassword, encodedEntry) == 0) {
                    //Found it!
                    printf("\n%.*s is the one!\n", length, generatedPasswordCopy );
                    exit(0);
                }
                //printf("\n");
            }
        }
        //printf("\n");
    }
}
int main(int argc, char *argv[])
{
    int const LENGTH = 100;
    int i = 0;
    int letters = 0;

    int test[LENGTH];
    uint8_t userEntry[LENGTH];

    int generatePassword[LENGTH];

    int lenAlphabets = strlen(alphabets);

    bool same = false;

    printf("Please enter a password (max size is %d): ", LENGTH);
    scanf("%99[^\n]", userEntry);

    printf("Length: %lu", strlen((char *)userEntry));

    struct AES_ctx ctx;
    // encode and decode requires a pair of the same calls,
    AES_init_ctx_iv(&ctx, key, iv);
    AES_CTR_xcrypt_buffer(&ctx, userEntry, strlen((char *)userEntry));
    printf("\nENC: %s\n", (char *)userEntry); // don't use this string as an input

    // initializes the generatePassword Array
    for (i = 0; i < LENGTH; i++)
    {
        generatePassword[i] = -1;
    }

    letters = 1;

    for (letters = 1; letters <= 50; letters++)
    {
        nextCode(letters, letters, lenAlphabets, generatePassword, userEntry);
        printf("\n");
    }
    /*
    do
    {



        //AES_init_ctx_iv(&ctx, key, iv);
        //AES_CTR_xcrypt_buffer(&ctx, test, strlen((char *)test));


    } while(!same);
    */
    printf("\nDEC: %s\n", (char *)test); // don't use this string as an input

    return 0;
}