#include "timer.h"
#include <stdlib.h>
#include <stdio.h>

void start_timer()
// start timer. startCount will be set at this point.
{
    stopped = 0; // reset stop flag
#ifdef WIN32
	QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&startCount);
#else
    gettimeofday(&startCount, NULL);
#endif
}

void stop_timer()
// stop the timer. endCount will be set at this point.
{
    stopped = 1; // set timer stopped flag

#ifdef WIN32
    QueryPerformanceCounter(&endCount);
#else
    gettimeofday(&endCount, NULL);
#endif
}

void print_timer(int time_units)
// print timer.
{
	printf("\n\nExecution Time : ");
	switch(time_units)
	{
	case US  : printf("%f usec\n",getElapsedTimeInMicroSec()); break;
	case MS  : printf("%f msec\n",getElapsedTimeInMilliSec()); break;
	case SEC : printf("%f sec\n",getElapsedTimeInSec()); break;
	default  : printf("?? (Invalid Time Units Specified..)\n"); break;
	}

}

double getElapsedTimeInMicroSec()
// compute elapsed time in micro-second resolution.
// other getElapsedTime will call this first, then convert to correspond resolution.
{
#ifdef WIN32
    if(!stopped)
        QueryPerformanceCounter(&endCount);

    startTimeInMicroSec = startCount.QuadPart * (1000000.0 / frequency.QuadPart);
    endTimeInMicroSec = endCount.QuadPart * (1000000.0 / frequency.QuadPart);
#else
    if(!stopped)
        gettimeofday(&endCount, NULL);

    startTimeInMicroSec = (startCount.tv_sec * 1000000.0) + startCount.tv_usec;
    endTimeInMicroSec = (endCount.tv_sec * 1000000.0) + endCount.tv_usec;
#endif

    return endTimeInMicroSec - startTimeInMicroSec;
}

double getElapsedTimeInMilliSec()
// divide elapsedTimeInMicroSec by 1000
{
    return getElapsedTimeInMicroSec() * 0.001;
}

double getElapsedTimeInSec()
// divide elapsedTimeInMicroSec by 1000000
{
    return getElapsedTimeInMicroSec() * 0.000001;
}

double getElapsedTime()
// same as getElapsedTimeInSec()
{
    return getElapsedTimeInSec();
}
