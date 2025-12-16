#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include "svlock/svlock.h"

int main(int argc, char *argv[])
{
    int res = 0;

    res = svlock_init_index(0, 1, 0, 0, 0);
    if (res != 0) {
        printf("Failed svlock_init_index\n");
        return EXIT_FAILURE;
    }
    printf("Passed svlock_init_index\n");

    res = svlock_get_initialized(0, 0, 0, 0);
    if (res != 1) {
        printf("Failed svlock_get_initialized\n");
        return EXIT_FAILURE;
    }
    printf("Passed svlock_get_initialized\n");
    
    res = svlock_getvalue(0, 0, 0, 0);
    if (res != 1) {
        printf("Failed svlock_getvalue\n");
        return EXIT_FAILURE;
    }
    printf("Passed svlock_getvalue\n");
    
    res = svlock_acquire(0, 0, 0, 0);
    if (res != 0) {
        printf("Failed svlock_acquire\n");
        return EXIT_FAILURE;
    }
    printf("Passed svlock_acquire\n");
    
    res = svlock_getvalue(0, 0, 0, 0);
    if (res != 0) {
        printf("Failed svlock_getvalue\n");
        return EXIT_FAILURE;
    }
    printf("Passed svlock_getvalue\n");
    
    res = svlock_release(0, 0, 0, 0);
    if (res != 0) {
        printf("Failed svlock_release\n");
        return EXIT_FAILURE;
    }
    printf("Passed svlock_release\n");
    
    res = svlock_getvalue(0, 0, 0, 0);
    if (res != 1) {
        printf("Failed svlock_getvalue\n");
        return EXIT_FAILURE;
    }
    printf("Passed svlock_getvalue\n");
    
    res = svlock_cleanup();
    if (res != 0) {
        printf("Failed svlock_cleanup\n");
        return EXIT_FAILURE;
    }
    printf("Passed svlock_cleanup\n");

    return 0;
}
