/*
 * stopwatch.c
 *
 *  Created on: Aug 27, 2011
 *  Author: bhoenes
 *  adapted from http://www.guyrutenberg.com/2007/09/22/profiling-code-using-clock_gettime/
 */
#include "stopwatch.h"

// utility function for time difference
struct timespec diff(struct timespec start, struct timespec end)
{
	struct timespec temp;
	if ((end.tv_nsec-start.tv_nsec)<0) {
		temp.tv_sec = end.tv_sec-start.tv_sec-1;
		temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
	} else {
		temp.tv_sec = end.tv_sec-start.tv_sec;
		temp.tv_nsec = end.tv_nsec-start.tv_nsec;
	}
	return temp;
}

stopwatch* stopwatch_new() {
	stopwatch* sw;
	sw = (stopwatch*) malloc(sizeof(stopwatch));
	return sw;
}

void stopwatch_delete(stopwatch* sw) {
	free(sw);
}

void stopwatch_start(stopwatch* sw) {
	if (!sw->running) {
		sw->running = TRUE;
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &sw->start);
	}
}
void stopwatch_stop(stopwatch* sw) {
	if(sw->running) {
		sw->running = FALSE;
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &sw->stop);
	}
}
double stopwatch_time(stopwatch* sw) {
	struct timespec elapsed;
	if(sw->running) {
		struct timespec now;
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &now);
		elapsed = diff(sw->start, now);
	} else {
		elapsed = diff(sw->start, sw->stop);
	}
	return ((double)(elapsed.tv_sec) + ((double)(elapsed.tv_nsec)*0.000000001));	
}

void stopwatch_reset(stopwatch* sw) {
	struct timespec now;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &now);
	sw->running=FALSE;
	sw->start=now;
	sw->stop=now;	
}
void stopwatch_restart(stopwatch* sw) {
	stopwatch_reset(sw);
	stopwatch_start(sw);
}
