/* svlock.swg */
%module svlock

%include "typemaps.i"
%include "stdint.i"

%{
#include "../svlock/svlock.h"
%}

typedef int pid_t;
typedef long int time_t;

int svlock_is_initialized(int index=0, pid_t pid=0, pid_t tid=0, time_t duration=0);
int svlock_get_initialized(int index=0, pid_t pid=0, pid_t tid=0, time_t duration=0);
int svlock_set_initialized(int index=0, int value=0, pid_t pid=0, pid_t tid=0, time_t duration=0);
int svlock_shm_open(void);
int svlock_init_index(int index=0, int value=0, pid_t pid=0, pid_t tid=0, time_t duration=0);
int svlock_init(int value=0, pid_t pid=0, pid_t tid=0, time_t duration=0);
int svlock_acquire(int index=0, pid_t pid=0, pid_t tid=0, time_t duration=0);
int svlock_release(int index=0, pid_t pid=0, pid_t tid=0, time_t duration=0);
int svlock_getvalue(int index=0, pid_t pid=0, pid_t tid=0, time_t duration=0);
int svlock_get_value(int index=0, pid_t pid=0, pid_t tid=0, time_t duration=0);
int svlock_get_count(int index=0, pid_t pid=0, pid_t tid=0, time_t duration=0);
int svlock_close(int index=0, pid_t pid=0, pid_t tid=0, time_t duration=0);
int svlock_unlink(void);
int svlock_release_all(void);
int svlock_release_all_index(int index=0, pid_t pid=0, pid_t tid=0, time_t duration=0);
int svlock_close_all(void);
int svlock_cleanup(void);
int svlock_shm_close(void);

%include "svlock.i"
