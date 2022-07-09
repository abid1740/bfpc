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
#include "mpi.h"

extern char alphabets[];

int main(int argc, char *argv[])
{

    int myRank;       // Process rank.
    int processCount; // Total number of process
    int root = 0;     // The root process's rank, default to 0 at begining.

    MPI_Comm comm = MPI_COMM_WORLD;

    // Initializatoin of MPI Init
    MPI_Init(&argc, &argv);

    // Determing the number of processes in this world
    MPI_Comm_size(comm, &processCount);
    // Detemring the current process' id
    MPI_Comm_rank(comm, &myRank);

    int const LENGTH = 100;
    int i = 0;
    int letters = 0;

    int test[LENGTH];

    char *userEntry = NULL;
    unsigned char hashFromUser[SHA256_DIGEST_LENGTH];

    int generatePassword[LENGTH];

    int lenAlphabets = strlen(alphabets);

    bool found = false;

    // Process Length control
    int lengthControl[processCount];
    int lengthControllerIndex = 0;
    int assignmentLength = 0;

    if (myRank == root)
    {
        bool passwordHashed = false;
        do
        {
            printf("Please enter a password (max size is %d): ", LENGTH);
            fflush(stdout);
            scanf("%m[^\n]%*c", &userEntry);

            size_t length = strlen((const char *)userEntry);

            if (length < 100)
            {

                SHA256_CTX context;
                SHA256_Init(&context);
                SHA256_Update(&context, (unsigned char *)userEntry, length);
                SHA256_Final(hashFromUser, &context);

                printf("Your Hash is\n");
                for (i = 0; i < SHA256_DIGEST_LENGTH; i++)
                {
                    printf("%02x", hashFromUser[i]);
                }
                printf("\n");
                passwordHashed = true;
            }
            else
            {
                printf("Password can only be less than 100 characters long. \n");
            }
        } while (!passwordHashed);

        // now let's start the track keeping of lenght for each process
        for (lengthControllerIndex = 0; lengthControllerIndex < processCount; lengthControllerIndex++)
        {
            lengthControl[lengthControllerIndex] = lengthControllerIndex + 1;
        }
    }
    // now that we have the password Hashed, let's send that to the other threads.
    MPI_Bcast(&(hashFromUser[0]), SHA256_DIGEST_LENGTH, MPI_UNSIGNED_CHAR, root, comm);

    /* Debug Code
    if (myRank == 2)
    {
        printf("P%d:\n", myRank);
        printf("\n");
        for (i = 0; i < SHA256_DIGEST_LENGTH; i++)
        {
            printf("%02x", hashFromUser[i]);
        }
        printf("\n");
    }
    */

    // Now that all processes have the Hash, time to tell them their initial length assignment
    if (myRank == root)
    {
        for (i = 1; i < processCount; i++)
        {
            MPI_Send(&(lengthControl[i]), 1, MPI_INT, i, 0, comm);
        }
        assignmentLength = lengthControl[0];
    }
    else
    {
        MPI_Status status;
        MPI_Recv(&assignmentLength, 1, MPI_INT, root, 0, comm, &status);
    }

    printf("P%d, Assignment Length: %d\n", myRank, assignmentLength);

    // initializes the generatePassword Array
    for (i = 0; i < LENGTH; i++)
    {
        generatePassword[i] = -1;
    }

    found = nextCode(assignmentLength, assignmentLength, lenAlphabets, generatePassword, hashFromUser);

    printf("\n");

    if (found)
    {
        printf("P%d Found password\n", myRank);
        int length = 0;
        bool moreLetters = true;
        while (moreLetters)
        {
            if (generatePassword[length] == -1)
            {
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
    } else {
        printf("P%d found nothing, exiting...\n", myRank);
    }

    MPI_Finalize();
    return 0;
}