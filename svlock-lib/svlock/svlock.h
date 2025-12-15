//-----------------------------------------------------------------------------
// File:        svlock.h
//
// Description: Library routines to support svlock API
//
// Author:      Kevin Beadle
//-----------------------------------------------------------------------------
#ifndef _SVLOCK_H_
#define _SVLOCK_H_

#define SVLOCK_GLOBAL_SEMAPHORE_NAME "/svlock_global_semaphore"
#define SVLOCK_GLOBAL_INDEX 0
#define SVLOCK_MAX_SEMAPHORES 1024
#define SVLOCK_MAX_HOLD_TIME 30 
#define SVLOCK_MAX_WAIT_TIME 300 

#ifdef __cplusplus
extern "C" {
#endif

int svlock_is_initialized(int index);
int svlock_get_initialized(int index);
int svlock_set_initialized(int index, int value);
int svlock_shm_open(void);
int svlock_init_index(int index, int value);
int svlock_init(int count);
int svlock_acquire(int index);
int svlock_release(int index);
int svlock_getvalue(int index);
int svlock_get_value(int index);
int svlock_get_count(int index);
int svlock_close(int index);
int svlock_unlink(void);
int svlock_release_all(void);
int svlock_release_all_index(int index);
int svlock_close_all(void);
int svlock_cleanup(void);
int svlock_shm_close(void);

#ifdef __cplusplus
}
#endif

#endif // _SVLOCK_H_
