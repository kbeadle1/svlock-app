//
// Created by kbeadle on 07/09/25.
//

/*
 * svlock_app.c - the process to use ioctl's to control the kernel module
 *
 */

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "svlock_app.h"

// #ifdef CONFIG_SVOS
//#include <sv/svlib.h>
//#include <sv/sv.h>
// #endif

#define NUM_THREADS 5
#define MAX_RESOURCES 1
#define THREAD_WORKTIME 5
#define SVLOCK_MAX_SEMAPHORES 1024

typedef struct svlock_t
{
    sem_t semaphore[SVLOCK_MAX_SEMAPHORES];
    int initialized[SVLOCK_MAX_SEMAPHORES];
    int count;
} svlock_t;

sem_t semaphore; // Declare a semaphore variable
sem_t *named_semaphore;
svlock_t *shm_svlock;
SvlockData svlock;
int *g_file_desc;


// driver load/unload
static int
svlockUnloadSvlockDriver(void)
{
    int returnCode;
    char const  *svlock_driver_unload_command =  "rmmod "SVLOCK_MODULE_NAME;

    returnCode = system(svlock_driver_unload_command);
    // printf("Executed command %s: result %d.\n", svlock_driver_unload_command, returnCode);
    return returnCode;
}

static int
svlockLoadSvlockDriver(void)
{
    int return_code;
    char const  *svlock_driver_load_command =  "modprobe "SVLOCK_MODULE_NAME;

    return_code = system(svlock_driver_load_command);

    //if (verbosity > VERBOSITY_LEVEL_NORMAL) {
    //   printf("Executed command %s: result %d.\n", svlock_driver_load_command, return_code);
    //}
    return return_code;
}

/*
 * Functions for the ioctl calls
 */
void ioctlInit(int file_desc, int nlocks, int tag, int pid, int tid)
{
    int ret_val;
    SvlockData svlock_param;
    svlock_param.tag = tag > 0 ? tag : 0;
    svlock_param.pid = pid > 0 ? pid : getpid();
    svlock_param.tid = tid > 0 ? tid : gettid();
    svlock_param.nlocks = nlocks > 0 ? nlocks : 1;
    svlock_param.lock = 0;
    ret_val = ioctl(file_desc, SVLOCK_IOCTL_INIT, &svlock_param);
    if (ret_val < 0) {
        printf("SVLOCK_IOCTL_INIT failed: %d\n", ret_val);
        exit(-1);
    }
}

void ioctlAcquire(int file_desc, int nlocks, int tag, int pid, int tid)
{
    int ret_val;
    SvlockData svlock_param;
    svlock_param.tag = tag > 0 ? tag : 0;
    svlock_param.pid = pid > 0 ? pid : getpid();
    svlock_param.tid = tid > 0 ? tid : gettid();
    svlock_param.nlocks = nlocks > 0 ? nlocks : 1;
    svlock_param.lock = 0;
    ret_val = ioctl(file_desc, SVLOCK_IOCTL_ACQUIRE, &svlock_param);
    if (ret_val < 0) {
        printf("SVLOCK_IOCTL_ACQUIRE failed: %d\n", ret_val);
        exit(-1);
    }
}

void ioctlRelease(int file_desc, int nlocks, int tag, int pid , int tid)
{
    int ret_val;
    SvlockData svlock_param;
    svlock_param.tag = tag > 0 ? tag : 0;
    svlock_param.pid = pid > 0 ? pid : getpid();
    svlock_param.tid = tid > 0 ? tid : gettid();
    svlock_param.nlocks = nlocks > 0 ? nlocks : 1;
    svlock_param.lock = 1;
    ret_val = ioctl(file_desc, SVLOCK_IOCTL_RELEASE, &svlock_param);
    if (ret_val < 0) {
        printf("SVLOCK_IOCTL_RELEASE failed: %d\n", ret_val);
        exit(-1);
    }
}

void ioctlReleaseAll(int file_desc, int nlocks, int tag, int pid , int tid)
{
    int ret_val;
    SvlockData svlock_param;
    svlock_param.tag = tag > 0 ? tag : 0;
    svlock_param.pid = pid > 0 ? pid : getpid();
    svlock_param.tid = tid > 0 ? tid : gettid();
    svlock_param.nlocks = nlocks > 0 ? nlocks : 1;
    svlock_param.lock = 1;
    ret_val = ioctl(file_desc, SVLOCK_IOCTL_RELEASEALL, &svlock_param);
    if (ret_val < 0) {
        printf("SVLOCK_IOCTL_RELEASEALL failed: %d\n", ret_val);
        exit(-1);
    }
}

void ioctlDeinit(int file_desc, int nlocks, int tag, int pid, int tid)
{
    int ret_val;
    SvlockData svlock_param;
    svlock_param.tag = tag > 0 ? tag : 0;
    svlock_param.pid = pid > 0 ? pid : getpid();
    svlock_param.tid = tid > 0 ? tid : gettid();
    svlock_param.nlocks = nlocks > 0 ? nlocks : 1;
    svlock_param.lock = 0;
    ret_val = ioctl(file_desc, SVLOCK_IOCTL_DEINIT, &svlock_param);
    if (ret_val < 0) {
        printf("SVLOCK_IOCTL_DEINIT failed: %d\n", ret_val);
        exit(-1);
    }
}

void ioctlDeinitAll(int file_desc, int nlocks, int tag, int pid, int tid)
{
    int ret_val;
    SvlockData svlock_param;
    svlock_param.tag = tag > 0 ? tag : 0;
    svlock_param.pid = pid > 0 ? pid : getpid();
    svlock_param.tid = tid > 0 ? tid : gettid();
    svlock_param.nlocks = nlocks > 0 ? nlocks : 1;
    svlock_param.lock = 0;
    ret_val = ioctl(file_desc, SVLOCK_IOCTL_DEINITALL, &svlock_param);
    if (ret_val < 0) {
        printf("SVLOCK_IOCTL_DEINITALL failed: %d\n", ret_val);
        exit(-1);
    }
}

void ioctlList(int file_desc, int nlocks, int tag, int pid, int tid)
{
    int ret_val;
    SvlockData svlock_param;
    svlock_param.tag = tag > 0 ? tag : 0;
    svlock_param.pid = pid > 0 ? pid : getpid();
    svlock_param.tid = tid > 0 ? tid : gettid();
    svlock_param.nlocks = nlocks > 0 ? nlocks : 1;
    svlock_param.lock = 0;
    ret_val = ioctl(file_desc, SVLOCK_IOCTL_LIST, &svlock_param);
    if (ret_val < 0) {
        printf("SVLOCK_IOCTL_LIST failed: %d\n", ret_val);
        exit(-1);
    }
}

void testLock(int file_desc, int nlocks, int tag, int pid, int tid)
{
    int ret_val;
    SvlockData svlock_param;
    svlock_param.tag = tag > 0 ? tag : 0;
    svlock_param.pid = getpid();
    svlock_param.tid = gettid();
    svlock_param.nlocks = nlocks > 0 ? nlocks : 1;
    svlock_param.lock = 0;
    ret_val = ioctl(file_desc, SVLOCK_IOCTL_INIT, &svlock_param);
    if (ret_val < 0) {
        printf("SVLOCK_IOCTL_INIT failed: %d\n", ret_val);
        exit(-1);
    }
    ret_val = ioctl(file_desc, SVLOCK_IOCTL_ACQUIRE, &svlock_param);
    if (ret_val < 0) {
        printf("SVLOCK_IOCTL_ACQUIRE failed: %d\n", ret_val);
        exit(-1);
    }
    ret_val = ioctl(file_desc, SVLOCK_IOCTL_RELEASE, &svlock_param);
    if (ret_val < 0) {
        printf("SVLOCK_IOCTL_RELEASE failed: %d\n", ret_val);
        exit(-1);
    }
    ret_val = ioctl(file_desc, SVLOCK_IOCTL_DEINIT, &svlock_param);
    if (ret_val < 0) {
        printf("SVLOCK_IOCTL_DEINIT failed: %d\n", ret_val);
        exit(-1);
    }
}

void *pthread_function(void *arg) {
	printf("Thread: Waiting to acquire semaphore...\n");
	sem_wait(&semaphore); // Decrement semaphore, blocks if 0
	printf("Thread: Semaphore acquired, entering critical section.\n");
	// Critical section: Access shared resource
	printf("Thread: Performing work in critical section...\n");
	sleep(THREAD_WORKTIME); // Simulate work
	printf("Thread: Releasing semaphore.\n");
	sem_post(&semaphore); // Increment semaphore, potentially unblocking a waiting thread
	return NULL;
}

void testPthread() {
	pthread_t tid1, tid2;
	// Initialize the binary semaphore with an initial value of 1
	// 0 for pshared means it's for threads within the same process
	sem_init(&semaphore, 0, 1);
	// Create two threads
	pthread_create(&tid1, NULL, pthread_function, NULL);
	pthread_create(&tid2, NULL, pthread_function, NULL);
	// Join the threads (wait for them to finish)
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	// Destroy the semaphore
	sem_destroy(&semaphore);
	printf("Main: All threads finished. Semaphore destroyed.\n");
}

void *pthread_function2(void *arg) {
	int thread_num = *((int*)arg);
	pid_t pid = getpid();
	sem_wait(&semaphore);
	//printf("Thread %d: Entering critical section.\n", thread_num);
	printf("PID %d Thread %d: Entering critical section.\n", pid, thread_num);
	sleep(THREAD_WORKTIME);
	//printf("Thread %d: Leaving critical section.\n", thread_num);
	printf("PID %d Thread %d: Leaving critical section.\n", pid, thread_num);
	sem_post(&semaphore);
	return NULL;
}

void testPthread2() {
	pthread_t threads[NUM_THREADS];
	int thread_args[NUM_THREADS];

	// Initialize semaphore with MAX_RESOURCES available at start
	sem_init(&semaphore, 0, MAX_RESOURCES);
	// Create threads
	for (int i = 0; i < NUM_THREADS; i++) {
		thread_args[i] = i;
		pthread_create(&threads[i], NULL, pthread_function2, &thread_args[i]);
	}

	// Join threads
	for (int i = 0; i < NUM_THREADS; i++) {
		pthread_join(threads[i], NULL);
	}

	// Destroy semaphore
	sem_destroy(&semaphore);
}

void *thread_function(void *arg) {
	printf("Thread: Waiting to acquire semaphore...\n");
	SvlockData svlock_param;
	svlock_param.pid = getpid();
	svlock_param.tid = gettid();
	svlock_param.nlocks = 1;
	svlock_param.lock = 0;
	
	//sem_wait(&semaphore); // Decrement semaphore, blocks if 0
	while (!svlock_param.lock) {
    		ioctl(*g_file_desc, SVLOCK_IOCTL_ACQUIRE, &svlock_param);
	}
	printf("Thread: Semaphore acquired, entering critical section.\n");
	// Critical section: Access shared resource
	printf("Thread: Performing work in critical section...\n");
	sleep(THREAD_WORKTIME); // Simulate work
	printf("Thread: Releasing semaphore.\n");
	//sem_post(&semaphore); // Increment semaphore, potentially unblocking a waiting thread
    	ioctl(*g_file_desc, SVLOCK_IOCTL_RELEASE, &svlock_param);
	return NULL;
}

// binary semaphore
void testThread(int file_desc, int nlocks, int pid, int tid) {
	pthread_t tid1, tid2;

	SvlockData svlock_param;
	svlock_param.pid = getpid();
	svlock_param.tid = gettid();
	svlock_param.nlocks = 1;
	svlock_param.lock = 0;
	g_file_desc = &file_desc;

	// Initialize the binary semaphore with an initial value of 1
	// 0 for pshared means it's for threads within the same process
	//sem_init(&semaphore, 0, 1);
	ioctl(file_desc, SVLOCK_IOCTL_INIT, &svlock_param);
	// Create two threads
	pthread_create(&tid1, NULL, thread_function, NULL);
	pthread_create(&tid2, NULL, thread_function, NULL);
	// Join the threads (wait for them to finish)
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	// Destroy the semaphore
	//sem_destroy(&semaphore);
    	ioctl(*g_file_desc, SVLOCK_IOCTL_DEINIT, &svlock_param);
	printf("Main: All threads finished. Semaphore destroyed.\n");
}

void *thread_function2(void *arg) {
	int thread_num = *((int*)arg);
	SvlockData svlock_param;
	//svlock_param.pid = getpid();
	svlock_param.pid = 0;
	svlock_param.tid = gettid();
	svlock_param.nlocks = MAX_RESOURCES;
	svlock_param.lock = 0;

	//sem_wait(&semaphore);
	while (!svlock_param.lock) {
    		ioctl(*g_file_desc, SVLOCK_IOCTL_ACQUIRE, &svlock_param);
	}
	printf("Thread %d: Entering critical section.\n", thread_num);
	sleep(THREAD_WORKTIME);
	printf("Thread %d: Leaving critical section.\n", thread_num);
	//sem_post(&semaphore);
    	ioctl(*g_file_desc, SVLOCK_IOCTL_RELEASE, &svlock_param);
	return NULL;
}

// counting semaphore
void testThread2(int file_desc, int nlocks, int tag, int pid, int tid) {
	pthread_t threads[NUM_THREADS];
	int thread_args[NUM_THREADS];

	SvlockData svlock_param;
	svlock_param.tag = 0;
	svlock_param.pid = getpid();
	svlock_param.tid = gettid();
	svlock_param.nlocks = MAX_RESOURCES;
	svlock_param.lock = 0;
	g_file_desc = &file_desc;

	// Initialize semaphore with MAX_RESOURCES available at start
	//sem_init(&semaphore, 0, MAX_RESOURCES);
	ioctl(file_desc, SVLOCK_IOCTL_INIT, &svlock_param);
	// Create threads
	for (int i = 0; i < NUM_THREADS; i++) {
		thread_args[i] = i;
		pthread_create(&threads[i], NULL, thread_function2, &thread_args[i]);
	}

	// Join threads
	for (int i = 0; i < NUM_THREADS; i++) {
		pthread_join(threads[i], NULL);
	}

	// Destroy semaphore
	//sem_destroy(&semaphore);
    	ioctl(*g_file_desc, SVLOCK_IOCTL_DEINIT, &svlock_param);
}

void *thread_function3(void *arg) {
	int thread_num = *((int*)arg);
	SvlockData svlock_param;
	svlock_param.tag = 0x4545;
	svlock_param.pid = getpid();
	svlock_param.tid = gettid();
	svlock_param.nlocks = MAX_RESOURCES;
	svlock_param.lock = 0;

	//sem_wait(&semaphore);
	while (!svlock_param.lock) {
    		ioctl(*g_file_desc, SVLOCK_IOCTL_ACQUIRE, &svlock_param);
	}
	printf("Thread %d: Entering critical section.\n", thread_num);
	sleep(THREAD_WORKTIME);
	printf("Thread %d: Leaving critical section.\n", thread_num);
	//sem_post(&semaphore);
    	ioctl(*g_file_desc, SVLOCK_IOCTL_RELEASE, &svlock_param);
	return NULL;
}

// counting semaphore
void testThread3(int file_desc, int nlocks, int tag, int pid, int tid) {
	pthread_t threads[NUM_THREADS];
	int thread_args[NUM_THREADS];

	SvlockData svlock_param;
	svlock_param.tag = 0x4545;
	svlock_param.pid = getpid();
	svlock_param.tid = gettid();
	svlock_param.nlocks = MAX_RESOURCES;
	svlock_param.lock = 0;
	g_file_desc = &file_desc;

	// Initialize semaphore with MAX_RESOURCES available at start
	//sem_init(&semaphore, 0, MAX_RESOURCES);
	ioctl(file_desc, SVLOCK_IOCTL_INIT, &svlock_param);
	// Create threads
	for (int i = 0; i < NUM_THREADS; i++) {
		thread_args[i] = i;
		pthread_create(&threads[i], NULL, thread_function3, &thread_args[i]);
	}

	// Join threads
	for (int i = 0; i < NUM_THREADS; i++) {
		pthread_join(threads[i], NULL);
	}

	// Destroy semaphore
	//sem_destroy(&semaphore);
    	//ioctl(*g_file_desc, SVLOCK_IOCTL_DEINIT, &svlock_param);
}

// macros
void *thread_function4(void *arg) {
	int thread_num = *((int*)arg);
	pid_t pid = getpid();
	
	//sem_wait(&semaphore);
	svlock_global_wait(*g_file_desc);
	//printf("Thread %d: Entering critical section.\n", thread_num);
	printf("PID %d Thread %d: Entering critical section.\n", pid, thread_num);
	sleep(THREAD_WORKTIME);
	//printf("Thread %d: Leaving critical section.\n", thread_num);
	printf("PID %d Thread %d: Leaving critical section.\n", pid, thread_num);
	//sem_post(&semaphore);
	svlock_global_release(*g_file_desc);
	return NULL;
}

// counting semaphore
void testThread4(int file_desc, int nlocks, int tag, int pid, int tid) {
	pthread_t threads[NUM_THREADS];
	int thread_args[NUM_THREADS];

	g_file_desc = &file_desc;

	// Initialize semaphore with MAX_RESOURCES available at start
	//sem_init(&semaphore, 0, MAX_RESOURCES);
	svlock_global_init(*g_file_desc, MAX_RESOURCES);
	// Create threads
	for (int i = 0; i < NUM_THREADS; i++) {
		thread_args[i] = i;
		pthread_create(&threads[i], NULL, thread_function4, &thread_args[i]);
	}

	// Join threads
	for (int i = 0; i < NUM_THREADS; i++) {
		pthread_join(threads[i], NULL);
	}

	// Destroy semaphore
	//sem_destroy(&semaphore);
    	//ioctl(*g_file_desc, SVLOCK_IOCTL_DEINIT, &svlock_param);
}

// named semaphore
void *thread_function5(void *arg) {
	int thread_num = *((int*)arg);
	pid_t pid = getpid();
	
	sem_wait(named_semaphore);
	//printf("Thread %d: Entering critical section.\n", thread_num);
	printf("PID %d Thread %d: Entering critical section.\n", pid, thread_num);
	sleep(THREAD_WORKTIME);
	//printf("Thread %d: Leaving critical section.\n", thread_num);
	printf("PID %d Thread %d: Leaving critical section.\n", pid, thread_num);
	sem_post(named_semaphore);
	return NULL;
}

void testThread5(int file_desc, int nlocks, int tag, int pid, int tid) {
	pthread_t threads[NUM_THREADS];
	int thread_args[NUM_THREADS];
	char *name = "svos_global_semaphore";

	// Initialize semaphore with MAX_RESOURCES available at start
	named_semaphore = sem_open(name, O_CREAT, 0666, MAX_RESOURCES);
	// Create threads
	for (int i = 0; i < NUM_THREADS; i++) {
		thread_args[i] = i;
		pthread_create(&threads[i], NULL, thread_function5, &thread_args[i]);
	}

	// Join threads
	for (int i = 0; i < NUM_THREADS; i++) {
		pthread_join(threads[i], NULL);
	}

	// Destroy semaphore
	sem_close(named_semaphore);
	sem_unlink(name);
}

// shm semaphore
void *thread_function6(void *arg) {
	int thread_num = *((int*)arg);
	pid_t pid = getpid();
	
	sem_wait(&shm_svlock->semaphore[0]);
	//printf("Thread %d: Entering critical section.\n", thread_num);
	printf("PID %d Thread %d: Entering critical section.\n", pid, thread_num);
	sleep(THREAD_WORKTIME);
	//printf("Thread %d: Leaving critical section.\n", thread_num);
	printf("PID %d Thread %d: Leaving critical section.\n", pid, thread_num);
	sem_post(&shm_svlock->semaphore[0]);
	return NULL;
}

void testThread6(int file_desc, int nlocks, int tag, int pid, int tid) {
	pthread_t threads[NUM_THREADS];
	int thread_args[NUM_THREADS];
	char *path_name = "/svos_global_semaphore";
	int fd;

	fd = shm_open(path_name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

	ftruncate(fd, sizeof(svlock_t));

	shm_svlock = mmap(NULL, sizeof(svlock_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	// Initialize semaphore with MAX_RESOURCES available at start
	if (!shm_svlock->initialized[0]) {
	    sem_init(&shm_svlock->semaphore[0], 1, MAX_RESOURCES);
	    shm_svlock->initialized[0] = 1;
	}

	// Create threads
	for (int i = 0; i < NUM_THREADS; i++) {
		thread_args[i] = i;
		pthread_create(&threads[i], NULL, thread_function6, &thread_args[i]);
	}

	// Join threads
	for (int i = 0; i < NUM_THREADS; i++) {
		pthread_join(threads[i], NULL);
	}

	// Destroy semaphore
	sem_close(&shm_svlock->semaphore[0]);
	shm_unlink(path_name);
}

#if 0
void ioctlAllocateMemoryTargets()
{

// #ifdef CONFIG_SVOS
    int verbose;
    int memType;
    void *memoryTarget;
    void *virtualAddress;
    uint64_t memSize;
    uint64_t tryAddress;
    uint64_t phyAddress;
    // uint64_t origPhysAddress;
    int	error;

    verbose = 1;
    memSize = alloc_size;
    //phyAddress = (((uint64_t) phy_addr_base_hi << 32) | (uint64_t) phy_addr_base_lo);
    phyAddress = phy_addr_base;

    memType = SVuncachableMemory | SVreadMapping | SVwriteMapping | SVexecMapping | SVsharedMapping;
    printf("Using MemType: 0x%u\n", memType);
    // processArgs(argc,argv);
    // svParamInit(&argc,&argv);

    // origPhysAddress = phyAddress;
    // phyAddress = origPhysAddress;
    // Calculate our memory size requirement and enforce page boundary. */
    if( memSize % getpagesize() )
        memSize = (memSize + getpagesize()) - (memSize % getpagesize());

    // if( verbose )
    //{
    printf("Adjusted Memory size to use (per pagesize): 0x%lx\n", memSize);
    //}

    // Open a memory target
    error = getMemoryTarget64(memType, &memoryTarget, &memSize, &virtualAddress, &tryAddress);
    if (error != 0)
    {
        fprintf(stderr, "%s: Can't get memory target\n", strerror(-error));
        exit(1);
    }
    printf("getMemoryTarget yields phy:0x%lx virt:%p\n", tryAddress, virtualAddress);
    if( phyAddress != tryAddress )
        fprintf(stderr, "Did not get requested address:  Wanted 0x%lx, Got 0x%lx\n", phyAddress, tryAddress );

    printf("memset(virtualAddress:0x%p, 0x0, memSize:0x%lx)\n", virtualAddress, memSize);
    memset(virtualAddress, 0x0, memSize);


// #endif

}
#endif


static int
svlockGetMajorNumber(void)
{
    FILE    *procMiscStream;
    int     parsed;
    int     major;
    char    *resStr;
    char    buffer[512];
    char    node[512];

    procMiscStream = fopen(SVLOCK_PROC_DEVICES_PATH, "r");
    if (procMiscStream == NULL) {
        printf("ERROR: Failure opening file: %s: %d.\n", SVLOCK_PROC_DEVICES_PATH, errno);
        return -ENODEV;
    }
    do {
        resStr = fgets(buffer, sizeof(buffer), procMiscStream);
        if (resStr != NULL) {
            parsed = sscanf(resStr, "%d %s", &major, node);
            if (parsed == 2) {
                if (strncmp(node, SVLOCK_DEVICE_NAME, strlen(SVLOCK_DEVICE_NAME)) == 0) {
                    //if (verbosity > VERBOSITY_LEVEL_NORMAL) {
                    //    printf("Determined major number %d.\n", major);
                    //}
                    fclose(procMiscStream);
                    return major;
                }
            }
        }
    } while (resStr != NULL);
    //if (verbosity > VERBOSITY_LEVEL_NORMAL) {
    //    printf("Could not determine major number.\n");
    //}
    fclose(procMiscStream);
    return -ENODEV;
}

static int
svlockRemoveDeviceNode(void)
{
    int return_code;
    return_code = remove(SVLOCK_DEVICE_PATH);
    //if (verbosity > VERBOSITY_LEVEL_NORMAL) {
    //    printf("Remove of file %s: result %d.\n", SVLOCK_DEVICE_PATH, return_code);
    //}
    return return_code;
}

static int
svlockMakeDeviceNode(void)
{
    int major;
    int error;
    dev_t dev;

    major = svlockGetMajorNumber();
    if (major >= 0) {
        dev = MKDEV(major, 0);
        error = mknod(SVLOCK_DEVICE_PATH, S_IFCHR, dev);
        if (error) {
            printf("ERROR: Failure creating node mknod: %s: %d.\n", SVLOCK_DEVICE_PATH, error);
        }
    } else {
        printf("ERROR: Failure getting device major number: %d.\n", major);
        error = -ENODEV;
    }
    return error;
}

static int
svlockIsDriverLoaded(void)
{
    if (svlockGetMajorNumber() >= 0) {
        // if (verbosity > VERBOSITY_LEVEL_NORMAL) {
        //     SVLOG_INFO(
        //             logHandle,
        //             LOGLVL_ERROR,
        //             "Determined driver is already loaded.\n"
        //     );
        // }
        return 1;
    } else {
        // if (verbosity > VERBOSITY_LEVEL_NORMAL) {
        //    SVLOG_INFO(
        //            logHandle,
        //            LOGLVL_ERROR,
        //            "Determined driver is not loaded.\n"
        //    );
        // }
        return 0;
    }
}

static int
svlockCheckAndLoadDriver(void)
{
    int     is_driver_loaded;
    int     result_code;

    result_code = 0;
    is_driver_loaded = svlockIsDriverLoaded();
    if (!is_driver_loaded) {
        result_code = svlockRemoveDeviceNode();
        result_code = svlockLoadSvlockDriver();
        if (result_code == 0) {
            is_driver_loaded = svlockIsDriverLoaded();
            if (!is_driver_loaded) {
                printf("ERROR: driver not loaded.\n");
                result_code = -ENODEV;
                return result_code;
            }
            if (access(SVLOCK_DEVICE_PATH, F_OK) != -1) {
                // if (verbosity > VERBOSITY_LEVEL_NORMAL) {
                //     SVLOG_INFO(
                //             logHandle,
                //             LOGLVL_INFO_HI_PRIORITY,
                //             "Driver file %s was automatically created.\n",
                //             SVOS_SCAFFOLD_DRIVER_PATH
                //     );
                // }
                return 0;
            }
            // If the file isn't automatically created (by e.g. udev), create it.
            result_code = svlockMakeDeviceNode();
        }
    }
    return result_code;
}



/*++

Method Description:

    This method initializes global variables.

Arguments:

    void

Return Value:

    int indicating success (0) or failure.

--*/
static int
initializeDefaults(void)
{
    do_log = 0;
    unload_driver = 0;
    switch_cmd = 0;
    alloc_cmd = 0;
    phy_addr_base = 0;
    halt_cmd = 0;
    halt_return_code = 0;
    alloc_size = 0x1000;
    debug = 0;
    init_cmd = 0;
    deinit_cmd = 0;
    acquire_cmd = 0;
    release_cmd = 0;
    releaseall_cmd = 0;
    list_cmd = 0;
    posix_cmd = 0;
    named_cmd = 0;
    shm_cmd = 0;
    nlocks = 0;
    tag = 0;
    pid = 0;
    tid = 0;
    return 0;
}


/*++

Method Description:

    This method processes the command line arguments, setting
    global state accordingly.

Arguments:

    int argc,       - argument count
    char **argv     - pointer to list of argument strings

Return Value:

    VOID

--*/
static void
processArgs(int argc, char **argv)
{
    int         c;
    extern int  optind;
    int         argCount;
    static const struct option long_options[] = {
        {"switch", no_argument, &switch_cmd, 1}, // 0
        {"halt", 1, &halt_cmd, 1}, // 1
        {"svfs_alloc_targets", no_argument, &alloc_cmd, 1},  // 2
        {"phy_addr", 1, 0, 0}, // 3
        {"alloc_size", 1, 0, 0}, // 4
        {"log", no_argument, &do_log, 1},  // 5
        {"debug", no_argument, &debug, 1},  // 6
        {"unload", no_argument, &unload_driver, 1}, // 7
        {"init", no_argument, &init_cmd, 1}, // 8
        {"acquire", no_argument, &acquire_cmd, 1}, // 9
        {"release", no_argument, &release_cmd, 1}, // 10
        {"deinit", no_argument, &deinit_cmd, 1}, // 11
        {"list", no_argument, &list_cmd, 1}, // 12
        {"nlocks", 1, 0, 1}, // 13
        {"tag", 1, 0, 1}, // 14
        {"pid", 1, 0, 1}, // 15
        {"tid", 1, 0, 1}, // 16
        {"test", no_argument, &test_cmd, 1}, // 17
        {"posix", no_argument, &posix_cmd, 1}, // 18
        {"named", no_argument, &named_cmd, 1}, // 19
        {"shm", no_argument, &shm_cmd, 1}, // 20
        {"releaseall", no_argument, &releaseall_cmd, 1}, // 21
        {"deinitall", no_argument, &deinitall_cmd, 1}, // 22
        {0, 0, 0, 0}  // terminating element
    };
    argCount = 0;
    while (1) {
        int     option_index = 0;
        c = getopt_long (argc, argv, "", long_options, &option_index);
        switch (c) {
            case -1:
                if (argCount == 0) {
                    // printf("Nothing to do.\n");
                }
                return;

            case '?':
                fprintf(stderr, usage, argv[0]);
                printf("ERROR: illegal arguments.\n");
                exit(1);
                break;

            default:
                break;
        }
        switch (option_index) {
            case 0: // switch
            case 2: // allocate
            case 5: // log
            case 6: // debug
            case 7: // unload
            case 8: // init
            case 9: // acquire
            case 10: // release
            case 11: // deinit
            case 12: // list
            case 17: // test
            case 18: // posix
            case 19: // named
            case 20: // shm
            case 21: // releaseall
            case 22: // deinitall
                break;
            case 1: // halt return code
                halt_return_code = strtol(optarg, NULL, 16);
                break;
            case 3: // phy_addr_lo
                phy_addr_base = strtol(optarg, NULL, 16);
                break;
            case 4: // alloc_size
                alloc_size = strtol(optarg, NULL, 16);
                break;
            case 13: // nlocks 
                nlocks = strtol(optarg, NULL, 10);
                break;
            case 14: // tag
                tag = strtol(optarg, NULL, 16);
                break;
            case 15: // pid
                pid = strtol(optarg, NULL, 10);
                break;
            case 16: // tid
                tid = strtol(optarg, NULL, 10);
                break;
            default:
                printf("?? getopt returned character code 0%o ??.\n", c);
                break;
        }
        argCount += 1;
    }
}


/*
 * Main - Call the ioctl functions
 */
int
main(int argc, char **argv)
{
    int return_code;
    int file_desc;
    return_code = initializeDefaults();
    processArgs(argc, argv);


    if (!named_cmd && !shm_cmd && !posix_cmd) {
        return_code = svlockCheckAndLoadDriver();
    }

    time_t start_time, end_time;
    double elapsed_seconds;

    if (unload_driver) {
        return svlockUnloadSvlockDriver();
    }
    if (alloc_cmd) {
        printf("Starting allocation \n");
        //ioctlAllocateMemoryTargets();
    }
    if (switch_cmd) {
        file_desc = open(SVLOCK_DEVICE_PATH, 0);

        if (file_desc < 0) {
            printf("Can't open device file: %s\n", SVLOCK_DEVICE_PATH);
            exit(-1);
        }
        //ioctlStartQuiesce(file_desc);
        close(file_desc);
    }
    if (halt_cmd) {
        file_desc = open(SVLOCK_DEVICE_PATH, 0);
        if (file_desc < 0) {
            printf("Can't open device file: %s\n", SVLOCK_DEVICE_PATH);
            exit(-1);
        }
        //ioctlHalt(file_desc, halt_return_code);
        close(file_desc);
    }
    if (init_cmd) {
        file_desc = open(SVLOCK_DEVICE_PATH, 0);

        if (file_desc < 0) {
            printf("Can't open device file: %s\n", SVLOCK_DEVICE_PATH);
            exit(-1);
        }
        ioctlInit(file_desc, nlocks, tag, pid, tid);
        close(file_desc);
    }
    if (acquire_cmd) {
        file_desc = open(SVLOCK_DEVICE_PATH, 0);

        if (file_desc < 0) {
            printf("Can't open device file: %s\n", SVLOCK_DEVICE_PATH);
            exit(-1);
        }
        ioctlAcquire(file_desc, nlocks, tag, pid, tid);
        close(file_desc);
    }
    if (release_cmd) {
        file_desc = open(SVLOCK_DEVICE_PATH, 0);

        if (file_desc < 0) {
            printf("Can't open device file: %s\n", SVLOCK_DEVICE_PATH);
            exit(-1);
        }
        ioctlRelease(file_desc, nlocks, tag, pid, tid);
        close(file_desc);
    }
    if (releaseall_cmd) {
        file_desc = open(SVLOCK_DEVICE_PATH, 0);

        if (file_desc < 0) {
            printf("Can't open device file: %s\n", SVLOCK_DEVICE_PATH);
            exit(-1);
        }
        ioctlReleaseAll(file_desc, nlocks, tag, pid, tid);
        close(file_desc);
    }
    if (deinit_cmd) {
        file_desc = open(SVLOCK_DEVICE_PATH, 0);

        if (file_desc < 0) {
            printf("Can't open device file: %s\n", SVLOCK_DEVICE_PATH);
            exit(-1);
        }
        ioctlDeinit(file_desc, nlocks, tag, pid, tid);
        close(file_desc);
    }
    if (deinitall_cmd) {
        file_desc = open(SVLOCK_DEVICE_PATH, 0);

        if (file_desc < 0) {
            printf("Can't open device file: %s\n", SVLOCK_DEVICE_PATH);
            exit(-1);
        }
        ioctlDeinitAll(file_desc, nlocks, tag, pid, tid);
        close(file_desc);
    }
    if (list_cmd) {
        file_desc = open(SVLOCK_DEVICE_PATH, 0);

        if (file_desc < 0) {
            printf("Can't open device file: %s\n", SVLOCK_DEVICE_PATH);
            exit(-1);
        }
        ioctlList(file_desc, nlocks, tag, pid, tid);
        close(file_desc);
    }
    if (test_cmd) {
        file_desc = open(SVLOCK_DEVICE_PATH, 0);

        if (file_desc < 0) {
            printf("Can't open device file: %s\n", SVLOCK_DEVICE_PATH);
            exit(-1);
        }
    	start_time = time(NULL);
        //testLock(file_desc, nlocks, tag, pid, tid);
        //testThread(file_desc, nlocks, tag, pid, tid);
        //testThread2(file_desc, nlocks, tag, pid, tid);
        //testThread3(file_desc, nlocks, tag, pid, tid);
        testThread4(file_desc, nlocks, tag, pid, tid);
        end_time = time(NULL);
        elapsed_seconds = difftime(end_time, start_time);
        printf("Elapsed time: %.2f seconds\n", elapsed_seconds);
	close(file_desc);
    }
    if (posix_cmd) {
    	start_time = time(NULL);
        //testLock(file_desc, nlocks, pid, tid);
        //testPthread();
        testPthread2();
        end_time = time(NULL);
        elapsed_seconds = difftime(end_time, start_time);
        printf("Elapsed time: %.2f seconds\n", elapsed_seconds);
    }
    if (named_cmd) {
    	start_time = time(NULL);
        testThread5(file_desc, nlocks, tag, pid, tid);
        end_time = time(NULL);
        elapsed_seconds = difftime(end_time, start_time);
        printf("Elapsed time: %.2f seconds\n", elapsed_seconds);
    }
    if (shm_cmd) {
    	start_time = time(NULL);
        testThread6(file_desc, nlocks, tag, pid, tid);
        end_time = time(NULL);
        elapsed_seconds = difftime(end_time, start_time);
        printf("Elapsed time: %.2f seconds\n", elapsed_seconds);
    }


    return return_code;
}
