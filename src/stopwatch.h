/*
 * stopwatch.h
 *
 *  Created on: Aug 27, 2011
 *      Author: bhoenes
 *  adapted from http://www.guyrutenberg.com/2007/09/22/profiling-code-using-clock_gettime/
 */

#ifndef STOPWATCH_H_
#define STOPWATCH_H_
#define TRUE 1
#define FALSE 0
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

typedef struct stopwatch{
   struct timespec start;
   struct timespec stop ;
   unsigned short running;
} stopwatch;

stopwatch* stopwatch_new();
void stopwatch_delete(stopwatch *sw);
void stopwatch_start(stopwatch *sw);
void stopwatch_stop(stopwatch *sw);
double stopwatch_time(stopwatch *sw);
void stopwatch_reset(stopwatch *sw);
void stopwatch_restart(stopwatch *sw);
#endif /* STOPWATCH_H_ */


