#include "common_time.h"

#include <stdlib.h>
#include <errno.h>

#include <time.h>
#include <sys/time.h>

#define MICROS_TO_MILLIS    0.001   // from microseconds to ms
#define MILLIS_TO_NANOS     1000000 // from ms to ns

static micro_t microseconds_since_epoch = -1;

int get_microseconds_since_epoch(micro_t *t){
    struct timeval tv;
    if(gettimeofday(&tv, NULL)) return EXIT_FAILURE;
    *t = (micro_t)(tv.tv_sec)*1000000L + (micro_t)(tv.tv_usec);
    return EXIT_SUCCESS;
}

int common_starttime(micro_t *micros_since_epoch){
    if(get_microseconds_since_epoch(&microseconds_since_epoch)) return EXIT_FAILURE;
    if(micros_since_epoch != NULL) *micros_since_epoch = microseconds_since_epoch;
    return EXIT_SUCCESS;
}

int common_gettime(milli_t *t){
    if(t == NULL){
        errno = EINVAL;
        return EXIT_FAILURE;
    }
    micro_t now;
    if(get_microseconds_since_epoch(&now)) return EXIT_FAILURE;
    now -= microseconds_since_epoch;
    *t = now*MICROS_TO_MILLIS;
    return EXIT_SUCCESS;
}

int common_wait(milli_t t){
    struct timespec request = {
        .tv_sec = 0,
        .tv_nsec = MILLIS_TO_NANOS * t
    };
    return clock_nanosleep(CLOCK_REALTIME, 0, &request, NULL);
}
