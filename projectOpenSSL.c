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
    int threadIDFoundPassword = 0; // Used to determine the new Thread that found the password

    MPI_Comm comm = MPI_COMM_WORLD;

    // Initializatoin of MPI Init
    MPI_Init(&argc, &argv);

    // Determing the number of processes in this world
    MPI_Comm_size(comm, &processCount);
    // Detemring the current process' id
    MPI_Comm_rank(comm, &myRank);

    int const LENGTH = 100;
    int i = 0;
    int j = 0;
    int letters = 0;

    int test[LENGTH];

    char *userEntry = NULL;
    unsigned char hashFromUser[SHA256_DIGEST_LENGTH];

    int generateMasterPasswordSeed[LENGTH];
    int generatePassword[LENGTH];

    int lenAlphabets = strlen(alphabets);
    //lenAlphabets = 5; //Used for shorten Testing
    //lenAlphabets = 26;
    //printf("alpha: %d\n", lenAlphabets);

    bool found = false;

    // Process Length control
    int lengthControl[processCount];
    int lengthControllerIndex = 0;
    int assignmentLength = 0;

    int currentLength = 1;

    // time Measurements
    double startTime = 0.0;
    double endTime = 0.0;

    // Use to store the tasks being assigned to each process at the root
    int assignmentTasks[processCount][LENGTH];

    if (myRank == root)
    {
        bool passwordHashed = false;
        do
        {
            printf("P%d: Please enter a password (max size is %d): ", root, LENGTH);
            fflush(stdout);
            scanf("%m[^\n]%*c", &userEntry);

            size_t length = strlen((const char *)userEntry);

            if (length < 100)
            {

                SHA256_CTX context;
                SHA256_Init(&context);
                SHA256_Update(&context, (unsigned char *)userEntry, length);
                SHA256_Final(hashFromUser, &context);

                printf("P%d: Your Hash is\n", root);
                for (i = 0; i < SHA256_DIGEST_LENGTH; i++)
                {
                    printf("%02x", hashFromUser[i]);
                }
                printf("\n");
                passwordHashed = true;
            }
            else
            {
                printf("P%d: Password can only be less than 100 characters long. \n", root);
            }
        } while (!passwordHashed);

        
        /* we don't need this?
        // now let's start the track keeping of lenght for each process
        for (lengthControllerIndex = 0; lengthControllerIndex < processCount; lengthControllerIndex++)
        {
            lengthControl[lengthControllerIndex] = lengthControllerIndex + 1;
        }
        */
    }
    // now that we have the password Hashed, let's send that to the other threads.
    MPI_Bcast(&(hashFromUser[0]), SHA256_DIGEST_LENGTH, MPI_UNSIGNED_CHAR, root, comm);
    startTime = MPI_Wtime();

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
        // initializes the generatePassword Array
        for (i = 0; i < LENGTH; i++)
        {
            generateMasterPasswordSeed[i] = 0;
            generatePassword[i] = 0; // Taking care of the first password for root
        }

        // determine the length we need to send
        currentLength = 1;

        for (i = 0; i < processCount; i++)
        {
            // if the length to send it > 3, we'll need to break it up
            if (currentLength <= 3) {
                // store data to root's marker and increase currentLength
                for (j = 0; j < LENGTH; j++){
                    assignmentTasks[i][j] = generateMasterPasswordSeed[j];
                }
                lengthControl[i] = currentLength;
                currentLength++;
            } else {
                for (j = 0; j < LENGTH; j++){
                    assignmentTasks[i][j] = generateMasterPasswordSeed[j];
                }

                int endingPoint = currentLength;                
                if (currentLength > 3){
                    endingPoint = currentLength - 3;
                }

                bool cycleDone = addOne(endingPoint, currentLength, lenAlphabets, generateMasterPasswordSeed);
                lengthControl[i] = currentLength;

                if (cycleDone){
                    currentLength++;
                    printf("P%d: **************************************** + 1 currentLenght: %d\n", root, currentLength);
                }
            }

            if (i != root){
                MPI_Send(&(lengthControl[i]), 1, MPI_INT, i, 0, comm);
                MPI_Send(&(assignmentTasks[i][0]), 100, MPI_INT, i, 0, comm);
            }
        }
        // assigning assignmentLength for root
        assignmentLength = lengthControl[0];
    }
    
    while(!found){
        
        if (myRank != root){
            MPI_Status status;
            
            MPI_Recv(&assignmentLength, 1, MPI_INT, root, 0, comm, &status);
            MPI_Recv(&generatePassword, 100, MPI_INT, root, 0, comm, &status);
        }

        /* //Debug code
        printf("P%d, Assignment Length: %d\n", myRank, assignmentLength);

        for (i = 0; i < assignmentLength; i++){
            printf("%d ", generatePassword[i]);
        }
        printf("\n");   
        */

        int assignmentStart = assignmentLength - 3;
        int assignmentEnd = assignmentLength;

        found = nextCode(assignmentStart, assignmentEnd, assignmentLength, lenAlphabets, generatePassword, hashFromUser);

        // now that we have finish the group here, let's return found to root
        if (myRank != root) {
            MPI_Send(&found, 1, MPI_C_BOOL, root, 0, comm);
        } else {
            MPI_Status status;

            int endingPoint = currentLength;                
            if (currentLength > 3){
                endingPoint = currentLength - 3;
            }

            for (i = 1; i < processCount; i++) {
                bool childFoundCode = false;
                MPI_Recv(&childFoundCode, 1, MPI_C_BOOL, MPI_ANY_SOURCE, 0, comm, &status);

                int doneWorkerID = status.MPI_SOURCE;
                if (childFoundCode) {
                    // we have code found, prepare to transfer root access to status.source
                    found = true;
                    threadIDFoundPassword = doneWorkerID;

                } else {
                    // let's continue by setting the new settings for this process's task
                    for (j = 0; j < LENGTH; j++){
                        assignmentTasks[doneWorkerID][j] = generateMasterPasswordSeed[j];
                    }

                    
                    bool cycleDone = addOne(endingPoint, currentLength, lenAlphabets, generateMasterPasswordSeed);
                    lengthControl[doneWorkerID] = currentLength;

                    if (cycleDone){
                        currentLength++;
                        printf("P%d: **************************************** + 1 currentLenght: %d\n", root, currentLength);

                        endingPoint = currentLength;                
                        if (currentLength > 3){
                            endingPoint = currentLength - 3;
                        }
                    }

                    
                    MPI_Send(&(lengthControl[i]), 1, MPI_INT, doneWorkerID, 0, comm);
                    MPI_Send(&(assignmentTasks[i][0]), 100, MPI_INT, doneWorkerID, 0, comm);

                }
            }

            /*
            // now we set the root's next task!
            for (j = 0; j < LENGTH; j++){
                generatePassword[j] = generateMasterPasswordSeed[j];
                assignmentTasks[0][j] = generateMasterPasswordSeed[j];
            }

            bool cycleDone = addOne(endingPoint, currentLength, lenAlphabets, generateMasterPasswordSeed);
            assignmentLength = currentLength;
            lengthControl[0] = currentLength;

            if (cycleDone){
                currentLength++;
            }
            */
            

            
        }
        MPI_Bcast(&(found), 1, MPI_C_BOOL, root, comm);
    }
    endTime = MPI_Wtime();

    MPI_Barrier(comm);

    // Transfer threadID that found the password
    MPI_Bcast(&(threadIDFoundPassword), 1, MPI_INT, root, comm);

    
    // transfer root to the new threadID
    root = threadIDFoundPassword;
    
    //printf("New Root for T%d is now T%d\n", myRank, root);

    if (myRank == root){
        printf("P%d: is now the new Root!", root);
        if (found)
        {
            printf("P%d Found password\n", myRank);

            /*
            int length = 0;
            bool moreLetters = true;
            while (moreLetters)
            {
                if (generatePassword[length] == -1)
                {
                    moreLetters = false;
                }
                length++;
            }*/
            
            uint8_t generatedDisplayPassword[assignmentLength + 1];
            for (j = 0; j < assignmentLength; j++)
            {
                // Debug Code
                //printf("%d ", generatePassword[j]);
                generatedDisplayPassword[j] = alphabets[generatePassword[j]];
            }
            generatedDisplayPassword[assignmentLength] = '\0';
            
            printf("P%d: Password is: \"%s\"\n", root, generatedDisplayPassword);
            printf("P%d: Total time used: %lfs\n", root, endTime-startTime);
            
        } 
    }
    
    //printf("T%d About to End all!\n", myRank);

    MPI_Barrier(comm);

    MPI_Finalize();
    return 0;
}