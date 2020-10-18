/***********************************************************
* Smithsonian Astrophysical Observatory
* Submillimeter Receiver Laboratory
* am
*
* am_sysdep.c                       S. Paine rev. 2019 May 3
*
* System dependent functions.
************************************************************/

#include <stdlib.h>
#include <time.h>

#if defined (__unix) || defined (__unix__)
#include <unistd.h>
#define UNISTD_INCLUDED
#elif defined _WIN32 /* defined under both win32 and win64 */
#include <windows.h>
#endif

#ifdef _OPENMP
#include <omp.h>
#endif

#include "am_sysdep.h"

/***********************************************************
* void am_sleep(unsigned int seconds)
*
* Purpose:
*   If the environment provides sleep() or equivalent,
*   suspends execution for the specified number of seconds.
*   Otherwise, does a polling loop on the standard C
*   library clock() function to implement a delay.
*
* Arguments:
*   unsigned int seconds - minimum time to suspend execution
*
* Return:
*   none
************************************************************/

void am_sleep(unsigned int seconds)
{
#if defined UNISTD_INCLUDED
    sleep(seconds);
    return;
#elif defined _WIN32
    Sleep((DWORD)(seconds * 1000));
    return;
#else
    clock_t t;

    t = clock() + (clock_t)(seconds * CLOCKS_PER_SEC);
    while (clock() < t)
        ;
    return;
#endif
}   /* am_sleep() */


/***********************************************************
* double am_timer(double tref)
*
* Purpose:
*   For performance timing.  Returns time difference in
*   seconds relative to a reference time tref.
*
*   For the serial version of am, the appropriate
*   performance metric is cpu time.  For the parallel
*   version, a better metric is wall clock time.
*   Consequently, this function returns clock() /
*   CLOCKS_PER_SEC in the serial case, and omp_get_wtime()
*   when compiled with OpenMP.  (Note that, under Windows,
*   this function always returns wall clock time, because
*   Windows clock() returns wall clock time.)
*
* Arguments:
*   double tref - reference time
* Return:
*   time in seconds, as a double.
************************************************************/

double am_timer(double tref)
{
    double t;
#ifdef _OPENMP
    /*
     * The omp_get_wtime() function was added to the OpenMP API
     * in version 2.0 (March 2002).  For older versions, default
     * to clock().
     */
#if (_OPENMP >= 200203)
    t = omp_get_wtime();
#else
    t = (double)clock() / (double)CLOCKS_PER_SEC;
#endif
#else
    t = (double)clock() / (double)CLOCKS_PER_SEC;
#endif
    return t - tref;
}   /* am_timer() */
