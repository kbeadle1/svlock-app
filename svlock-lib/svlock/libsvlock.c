//-----------------------------------------------------------------------------
// File:        svlock.c
//
// Description: Library routines to support svlock API
//
// Author:      Kevin Beadle
//-----------------------------------------------------------------------------

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "svlock.h"

typedef struct svlock_t
{
    sem_t semaphore[SVLOCK_MAX_SEMAPHORES];
    int value[SVLOCK_MAX_SEMAPHORES];
    int initialized[SVLOCK_MAX_SEMAPHORES];
    time_t initialized_time[SVLOCK_MAX_SEMAPHORES];
    int count[SVLOCK_MAX_SEMAPHORES];
} svlock_t;

int __svlock_fd;
svlock_t *__svlock = NULL;

int svlock_is_initialized(int index, pid_t pid, pid_t tid, time_t duration)
{
    int ret = 0;

    if (!__svlock)
    {
        ret = svlock_shm_open();
        if (ret == -1) {
            return -1;
        }
    }
    if (
         !__svlock->initialized[index] ||
         !__svlock->initialized_time[index]
       )
    {
        return 0;
    }
    else
    {
        time_t elapsed_time = difftime(time(NULL), __svlock->initialized_time[index]);
        if (elapsed_time >= SVLOCK_MAX_HOLD_TIME)
        {
            //__svlock->initialized[index] = 0;
            svlock_release_all_index(index, 0, 0, 0);
            return 0;
        }
        return 1;
    }
    return 0;
}

int svlock_get_value(int index, pid_t pid, pid_t tid, time_t duration)
{
    int ret = 0;

    if (!__svlock)
    {
        ret = svlock_shm_open();
        if (ret == -1) {
            return -1;
        }
    }
    return __svlock->value[index];
}

int svlock_get_count(int index, pid_t pid, pid_t tid, time_t duration)
{
    int ret = 0;

    if (!__svlock)
    {
        ret = svlock_shm_open();
        if (ret == -1) {
            return -1;
        }
    }

    if (__svlock->count[index] > 0) {
        return __svlock->count[index]-1;
    }

    return __svlock->count[index];
}

int svlock_get_initialized(int index, pid_t pid, pid_t tid, time_t duration)
{
    int ret = 0;

    if (!__svlock)
    {
        ret = svlock_shm_open();
        if (ret == -1) {
            return -1;
        }
    }
    return __svlock->initialized[index];
}

int svlock_set_initialized(int index, int value, pid_t pid, pid_t tid, time_t duration)
{
    int ret = 0;

    if (!__svlock)
    {
        ret = svlock_shm_open();
        if (ret == -1) {
            return -1;
        }
    }
    __svlock->initialized[index] = 1 ? value > 0 : 0;
    return __svlock->initialized[index];
}

int svlock_shm_open(void)
{
    int ret = 0;

    __svlock_fd = shm_open(SVLOCK_GLOBAL_SEMAPHORE_NAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (__svlock_fd == -1) {
        return -1;
    }

    ret = ftruncate(__svlock_fd, sizeof(svlock_t));
    if (ret == -1) {
        return -1;
    }

    __svlock = mmap(NULL, sizeof(svlock_t), PROT_READ | PROT_WRITE, MAP_SHARED, __svlock_fd, 0);
    if (__svlock == MAP_FAILED) {
        return -1;
    }

    return 0;
}

int svlock_init_index(int index, int value, pid_t pid, pid_t tid, time_t duration)
{
    int ret = 0;

    if (index >= SVLOCK_MAX_SEMAPHORES) {
        return -1;
    }

    if (value <= 0) {
        return -1;
    }

    if (!__svlock)
    {
        ret = svlock_shm_open();
        if (ret == -1) {
            return -1;
        }
    }

    //if (!svlock_is_initialized(index, 0, 0, 0))
    if (!__svlock->initialized[index])
    {
        // Initialize semaphores as process-shared with value
        ret = sem_init(&__svlock->semaphore[index], 1, value);
        __svlock->value[index] = value;
        __svlock->initialized_time[index] = time(NULL);
        __svlock->initialized[index] = 1;
        __svlock->count[index] = 0;
        if (ret == -1) {
            return -1;
        }
    }
    else
    {
        // Semaphore slot already initialized
        return 0; 
    }

    return 0; 
}

int svlock_init(int value, pid_t pid, pid_t tid, time_t duration)
{
    int ret = 0;
    int index = 0;

    if (value <= 0) {
       return -1;
    }

    if (!__svlock)
    {
        ret = svlock_shm_open();
        if (ret == -1) {
            return -1;
        }
    }

    // Get next open semaphore slot
    for (int i = 0; i < SVLOCK_MAX_SEMAPHORES; i++)
    {
        //if (!svlock_is_initialized(index, 0, 0, 0))
        if (!__svlock->initialized[index])
        {
            index = i;
            break;
        }
    }

    svlock_init_index(index, value, 0, 0, 0);

    return index;
}

int svlock_acquire(int index, pid_t pid, pid_t tid, time_t duration)
{
    int ret = 0;

    if (index >= SVLOCK_MAX_SEMAPHORES) {
        return -1;
    }

    if (!__svlock)
    {
        ret = svlock_shm_open();
        if (ret == -1) {
            return -1;
        }
    }

    if (!__svlock->initialized[index])
    {
        svlock_init_index(index, 1, 0, 0, 0);
    }

    __svlock->count[index]++;

    if (__svlock->value[index] > 0) {
        __svlock->value[index]--;
    }

    ret = sem_wait(&__svlock->semaphore[index]);

    // TODO: use sem_timedwait for non-blocking
#if 0
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec += SVLOCK_MAX_WAIT_TIME;
    ret = sem_timedwait(&shm_svlock->semaphore[0], &ts);
    if (ret == -1)
    {
        if (errno == ETIMEDOUT)
        {
            return -1;
        }
    }           
#endif

    return ret;
}

int svlock_release(int index, pid_t pid, pid_t tid, time_t duration)
{
    int ret = 0;

    if (index >= SVLOCK_MAX_SEMAPHORES) {
        return -1;
    }

    if (!__svlock)
    {
        ret = svlock_shm_open();
        if (ret == -1) {
            return -1;
        }
    }

    if (!__svlock->initialized[index]) {
        return -1;
    }

    ret = sem_post(&__svlock->semaphore[index]);

    __svlock->value[index]++;

    if (__svlock->count[index] > 0) {
        __svlock->count[index]--;
    }

    return ret;
}

int svlock_getvalue(int index, pid_t pid, pid_t tid, time_t duration)
{
    int ret = 0;
    int value = 0;

    if (index >= SVLOCK_MAX_SEMAPHORES) {
        return -1;
    }

    if (!__svlock)
    {
        ret = svlock_shm_open();
        if (ret == -1) {
            return -1;
        }
    }

    if (!__svlock->initialized[index]) {
        return -1;
    }

    ret = sem_getvalue(&__svlock->semaphore[index], &value);

    __svlock->value[index] = value;

    if (value > 0) {
        __svlock->count[index] = 0;
    }

    return value;
}

int svlock_close(int index, pid_t pid, pid_t tid, time_t duration)
{
    int ret = 0;

    if (index >= SVLOCK_MAX_SEMAPHORES) {
        return -1;
    }

    if (!__svlock)
    {
        ret = svlock_shm_open();
        if (ret == -1) {
            return -1;
        }
    }

    if (!__svlock->initialized[index]) {
        return -1;
    }

    ret = sem_close(&__svlock->semaphore[index]);
    
    __svlock->initialized[index] = 0;
    __svlock->value[index] = 0;
    __svlock->count[index] = 0;

    return ret;
}

int svlock_unlink(void)
{
    int ret = 0;

    ret = shm_unlink(SVLOCK_GLOBAL_SEMAPHORE_NAME);
    return ret;
}

int svlock_release_all(void)
{
    int value = 0;

    if (!__svlock) {
        svlock_shm_open();
    }

    for (int i = 0; i < SVLOCK_MAX_SEMAPHORES; i++)
    {  
        value = svlock_getvalue(i, 0, 0, 0);
        while (value == 0)
        {
            svlock_release(i, 0, 0, 0);
            // Give some time for semaphore to refresh value
            usleep(100000);
            value = svlock_getvalue(i, 0, 0, 0);
        }
    }

    return 0;
}

int svlock_release_all_index(int index, pid_t pid, pid_t tid, time_t duration)
{
    int curr_value = 0;

    if (!__svlock) {
        svlock_shm_open();
    }

    curr_value = svlock_getvalue(index, 0, 0, 0);

    while (curr_value == 0)
    {
        svlock_release(index, 0, 0, 0);
        // Give time for semaphore to refresh value
        usleep(100000);
        curr_value = svlock_getvalue(index, 0, 0, 0);
    }

    return 0;
}

int svlock_set_value(int index, int value, pid_t pid, pid_t tid, time_t duration)
{
    int curr_value = 0;

    if (value <= 0) {
        // value must be greater than 0
        return -1;
    }

    if (!__svlock) {
        svlock_shm_open();
    }

    curr_value = svlock_getvalue(index, 0, 0, 0);

    if (curr_value < 0) {
        svlock_init_index(index, value, 0, 0, 0);
        usleep(100000);
        curr_value = svlock_getvalue(index, 0, 0, 0);
    }

    while (curr_value > value)
    {
        svlock_acquire(index, 0, 0, 0);
        usleep(100000);
        curr_value = svlock_getvalue(index, 0, 0, 0);
    }

    while (curr_value < value)
    {
        svlock_release(index, 0, 0, 0);
        usleep(100000);
        curr_value = svlock_getvalue(index, 0, 0, 0);
    }

    __svlock->value[index] = value;

    return 0;
}

int svlock_set_value_all(int value, pid_t pid, pid_t tid, time_t duration)
{
    int curr_value = 0;

    if (!__svlock) {
        svlock_shm_open();
    }


    for (int i = 0; i < SVLOCK_MAX_SEMAPHORES; i++)
    {  
        curr_value = svlock_getvalue(i, 0, 0, 0);

        if (curr_value < 0) {
            svlock_init_index(i, value, 0, 0, 0);
            usleep(100000);
            curr_value = svlock_getvalue(i, 0, 0, 0);
        }


        while (curr_value > value)
        {
            svlock_acquire(i, 0, 0, 0);
            usleep(100000);
            curr_value = svlock_getvalue(i, 0, 0, 0);
        }

        while (curr_value < value)
        {
            svlock_release(i, 0, 0, 0);
            usleep(100000);
            curr_value = svlock_getvalue(i, 0, 0, 0);
        }

        __svlock->value[i] = value;
    }

    return 0;
}


int svlock_close_all(void)
{
    int ret = 0;

    if (!__svlock)
    {
        ret = svlock_shm_open();
        if (ret == -1) {
            return -1;
        }
    }


    for (int i = 0; i < SVLOCK_MAX_SEMAPHORES; i++) {
        svlock_close(i, 0, 0, 0);
    }

    return 0;
}

int svlock_cleanup(void)
{
    int ret = 0;

    if (!__svlock)
    {
        ret = svlock_shm_open();
        if (ret == -1) {
            return -1;
        }
    }

    svlock_release_all();
    svlock_close_all();
    svlock_unlink();

    return 0;
}

int svlock_shm_close(void)
{
    int ret = 0;

    if (!__svlock)
    {
        ret = svlock_shm_open();
        if (ret == -1) {
            return -1;
        }
    }

    ret = munmap(__svlock, sizeof(svlock_t)); 
    if (ret == -1) {
        return -1;
    }

    ret = close(__svlock_fd);
    if (ret == -1) {
        return -1;
    }

    return ret;
}
