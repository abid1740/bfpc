/***********************************************************************************************************************
 * Name:        GeneratePassword
 * Description: This treats Characters as if it is a number, instead of base 8 or 16, we have base the length of the 
 *              alphabets. 
 * 
 * Problem:     Even with Long Long Int, the generated code only about 11 characters long and is not a perfect cut off
 * 
 *              And it seems to be missing AA
 * 
 **********************************************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <limits.h>

void generatePassword(unsigned long long int num, char *outbuf)
{
    
    char alphabets[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789`~!@#$%^&*()_+-=";

    int i = 12;
    int j = 0;

    int base = strlen(alphabets);

    do
    {
        outbuf[i] = alphabets[num % base];
        i--;
        num = num / base;
    } while (num > 0);

    
    while (++i < 13)
    {
        outbuf[j++] = outbuf[i];
    }

    outbuf[j] = 0;
}

int main()
{

    unsigned long long int i = ULONG_MAX - 5;

    int base = 2;
    char output[100];

    while(1){
        generatePassword(i, output);

        printf("%llu: %s\n", i, output);
        i++;
    }

    return 0;
}
