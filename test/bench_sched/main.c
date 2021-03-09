#include <errno.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

static void print_timespec(const char *name, struct timespec *ts) {
    printf("%s = timsepc { tv_sec = %lu, tv_nsec = %lu }\n", name, ts->tv_sec, ts->tv_nsec);
}

static void timespec_diff(
    struct timespec *start,
    struct timespec *stop,
    struct timespec *result) {
    if ((stop->tv_nsec - start->tv_nsec) < 0) {
        result->tv_sec = stop->tv_sec - start->tv_sec - 1;
        result->tv_nsec = stop->tv_nsec - start->tv_nsec + 1000000000;
    } else {
        result->tv_sec = stop->tv_sec - start->tv_sec;
        result->tv_nsec = stop->tv_nsec - start->tv_nsec;
    }
    return;
}

static double timespec_to_seconds(struct timespec *ts) {
    double seconds = 0;
    seconds += ts->tv_sec;
    seconds += ts->tv_nsec / (1000.0 * 1000 * 1000);
    return seconds;
}

int main(int argc, const char *argv[]) {
    if (argc < 2) {
        printf("error: expected 1 argument: num_repeats\n");
        return 1;
    }

    errno = 0;
    unsigned long nrepeats = strtoul(argv[1], NULL, 10);
    if (errno) {
        printf("error: failed to parse argument: num_repeats\n");
        return 1;
    }
    if (nrepeats == 0) {
        printf("error: invalid argument: num_repeats must not be 0\n");
        return 1;
    }

    struct timespec start, end, elapsed;
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (unsigned long i = 0; i < nrepeats; i++) {
        sched_yield();
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    timespec_diff(&start, &end, &elapsed);

    double elapsed_seconds = timespec_to_seconds(&elapsed);
    double per_op = elapsed_seconds / nrepeats;
    printf("nrepeats : %lu\n", nrepeats);
    printf("elapsed time (seconds): %f\n", elapsed_seconds);
    printf("operation cost (seconds): %E\n", per_op);
    return 0;
}
