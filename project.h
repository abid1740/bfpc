

#ifndef PROJ_H
#define PROJ_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <openssl/sha.h>

bool nextCode(int currentPoint, int length, int maxNumber, int *array, unsigned char *encodedEntry);

#endif