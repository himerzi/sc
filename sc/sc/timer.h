#ifndef TIMER_H_DEF
#define TIMER_H_DEF

#ifdef WIN32                                // Windows system specific
#include <windows.h>
#else                                       // Unix based system specific
#include <sys/time.h>
#endif

void   start_timer();                       // start timer
void   stop_timer();                        // stop the timer
void print_timer(int time_units);           // print execution time
double getElapsedTime();                    // get elapsed time in second
double getElapsedTimeInSec();               // get elapsed time in second (same as getElapsedTime)
double getElapsedTimeInMilliSec();          // get elapsed time in milli-second
double getElapsedTimeInMicroSec();          // get elapsed time in micro-second

double startTimeInMicroSec;                 // starting time in micro-second
double endTimeInMicroSec;                   // ending time in micro-second
int    stopped;                             // stop flag 

// Time Units Constants
#define NS  0                               // nanoseconds
#define US  1                               // microseconds
#define MS  2                               // milliseconds
#define SEC 3                               // seconds

#ifdef WIN32
    LARGE_INTEGER frequency;                // ticks per second
    LARGE_INTEGER startCount;               // starting time
    LARGE_INTEGER endCount;                 // ending time
#else
    struct timeval startCount;              // typedef for compatibility
    struct timeval endCount;                // typedef for compatibility

#endif

#endif // TIMER_H_DEF