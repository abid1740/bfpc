#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/bio.h>
#include <openssl/sha.h>
#include <string.h>
#include <stdbool.h>

int main()
{
    SHA256_CTX context;
    
    unsigned char md[SHA256_DIGEST_LENGTH];
    unsigned char test2[SHA256_DIGEST_LENGTH];
    char *input = NULL;
    printf("What to hash: ");
    scanf("%m[^\n]%*c", &input);
    size_t length = strlen((const char *)input);
    int i, n = 0;

    for (n = 0; n < 5; n++)
    {
        SHA256_Init(&context);
        SHA256_Update(&context, (unsigned char *)input, length);
        SHA256_Final(md, &context);

        SHA256_Init(&context);
        SHA256_Update(&context, (unsigned char *)input, length);
        SHA256_Final(test2, &context);
        
        
        printf("\n");
        
        for (i = 0; i < SHA256_DIGEST_LENGTH; i++)
        {
            printf("%02x", md[i]);
        }
        printf("\n");
        
        bool same = true;
        for (i = 0; i < SHA256_DIGEST_LENGTH && same; i++) {
            if (md[i] != test2[i]) {
                same = false;
            }
        }
        if (same) {
            printf("matched!\n");
        } else {
            printf("Did not matched \n");
        }

        

        
    }
    free(input);

    return 0;
}