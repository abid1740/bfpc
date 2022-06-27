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

void nextCode(int level, int size, char * generated)
{
    int i = 0;
    for (i = 0; i < size ; i++)
    {
        generated[level] = i + '0';
        
        if (level > 0)
        {

            nextCode(level - 1, size, generated);
        }

        printf("%s\n", generated);
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    int const LENGTH = 100;
    int i = 0;
    int letters = 0;

    char key[] = "secret key 123";
    char test[LENGTH];
    char userEntry[LENGTH];

    // char alphabets[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 ";
    char alphabets[] = "ABCDE";

    int generatePassword[LENGTH];

    int lenAlphabets = strlen(alphabets);

    bool same = false;

    uint8_t iv[16] = {0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff};

    printf("Please enter a password (max size is %d): ", LENGTH);
    scanf("%99[^\n]", userEntry);

    printf("Length: %lu", strlen((char *)userEntry));

    struct AES_ctx ctx;
    // encode and decode requires a pair of the same calls,
    // AES_init_ctx_iv(&ctx, key, iv);
    // AES_CTR_xcrypt_buffer(&ctx, userEntry, strlen((char *)userEntry));
    printf("\nENC: %s", (char *)userEntry); // don't use this string as an input

    // initializes the generatePassword Array
    for (i = 0; i < LENGTH; i++)
    {
        generatePassword[i] = -1;
    }

    letters = 1;

    for (letters = 1; letters <= 5; letters++)
    {
        nextCode(letters, lenAlphabets, test);
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