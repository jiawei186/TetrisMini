#include <time.h>
#include "TimeCheck.h"

struct timespec timeFirst, timeSecond;

void timeInit()
{
    clock_gettime(CLOCK_MONOTONIC, &timeFirst);
}

int timeCheck()
{
    clock_gettime(CLOCK_MONOTONIC, &timeSecond);

    time_t difSecond =  timeSecond.tv_sec - timeFirst.tv_sec;
    long int difNanosecond = timeSecond.tv_nsec - timeFirst.tv_nsec;

    if ((difSecond > 0) || ((difSecond == 0) && (difNanosecond > TIMECHECKINTERVAL)))
    {
        clock_gettime(CLOCK_MONOTONIC, &timeFirst);

        return 1;
    }
    else
    {
        return 0;
    }
}
