/***********************************************************************************************************************
 * Take from: https://stackoverflow.com/questions/56502361/openssl-c-hash-passwords-w-sha256-or-sha512
 **********************************************************************************************************************/

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/bio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int iter = 1007;

    unsigned char salt[32] = {0};
    RAND_bytes(salt, sizeof(salt));

    unsigned char key[32] = {0};
    unsigned char key2[32] = {0};

    PKCS5_PBKDF2_HMAC("password", 8,
        salt, sizeof(salt),
        iter, EVP_sha256(),
        sizeof(key), key);

    PKCS5_PBKDF2_HMAC("password", 8,
        salt, sizeof(salt),
        iter, EVP_sha256(),
        sizeof(key2), key2);    

    BIO *bio = BIO_new_fp(stdout, BIO_NOCLOSE);
    //BIO_dump(bio, (const char*)salt, sizeof(salt));
    BIO_dump(bio, (const char*)key, sizeof(key));
    printf("\n");
    BIO_dump(bio, (const char*)key2, sizeof(key2));
    BIO_free(bio);

    
    if (strcmp(key, key2) == 0) {
        printf("Yay!\n");
    }
}