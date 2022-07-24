#include "project.h"

bool addOne(int currentPoint, int length, int maxNumber, int *array)
{
    bool maxReached = false;
    if (currentPoint >= 0)
    {
        if (array[currentPoint - 1] < maxNumber)
        {

            array[currentPoint - 1]++;

            if (array[currentPoint - 1] >= maxNumber)
            {
                if ((currentPoint - 1) >= 0)
                {
                    array[currentPoint - 1] = 0;
                    maxReached = addOne((currentPoint - 1), length, maxNumber, array);
                }
            }
        }
        else
        {
            maxReached = true;
        }
    }

    return maxReached;
}