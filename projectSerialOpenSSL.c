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



#include "project.h"

extern char alphabets[];

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

    bool found = false;

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

        for (letters = 1; letters <= LENGTH && !found; letters++)
        {
            printf(".");
            fflush(stdout);
            found = nextCode(letters, letters, lenAlphabets, generatePassword, hashFromUser);
            
        }
        printf("\n");
        /*
        do
        {



            //AES_init_ctx_iv(&ctx, key, iv);
            //AES_CTR_xcrypt_buffer(&ctx, test, strlen((char *)test));


        } while(!same);
        */
        // printf("\nDEC: %s\n", (char *)test); // don't use this string as an input

        if (found)
        {
            int length = 0;
            bool moreLetters = true;
            while (moreLetters){
                if (generatePassword[length] == -1) {
                    moreLetters = false;
                }
                length++;
            }
            uint8_t generatedDisplayPassword[length + 1];
            int j = 0;
            for (j = 0; j < length; j++)
            {
                // Debug Code
                // printf("%d ", array[j]);
                generatedDisplayPassword[j] = alphabets[generatePassword[j]];
            }
            generatedDisplayPassword[length] = '\0';

            printf("Password is: %s\n", generatedDisplayPassword);
        }
    }
    return 0;
}