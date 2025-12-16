#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "svlock/svlock.h"
#include "args.h"

#define ARRAY_SIZE(x)   (sizeof(x) / sizeof((x)[0]))

static bool display_semaphore_value(void);
static bool display_semaphore_count(void);
static bool display_acquire_semaphore(void);
static bool display_release_semaphore(void);
static bool display_release_all_semaphore(void);

struct output_selection {
    bool (*want_selection)(void);
    bool (*generated_successfully)(void);
};

struct output_selection output_selections[] = {
    { want_display_semaphore_value,     display_semaphore_value },
    { want_display_semaphore_count,     display_semaphore_count },
    { want_display_acquire_semaphore,     display_acquire_semaphore },
    { want_display_release_semaphore,     display_release_semaphore },
    { want_display_release_all_semaphore,     display_release_all_semaphore },
};

int main(int argc, char *argv[])
{
    unsigned int i;
    struct output_selection *selection;
    bool had_error = false;
  
    process_args(argc, argv);

    for (i = 0; i < ARRAY_SIZE(output_selections); i++) {
        selection = &output_selections[i];
        if (selection->want_selection()) {
            if (!selection->generated_successfully()) {
                had_error = true;
            }
        }
    }

    if (had_error) {
        return EXIT_FAILURE;
    } else {
        return EXIT_SUCCESS;
    }
}

static bool display_semaphore_value(void)
{
    int semaphore_index = 0;
    int semaphore_value = 0;

    semaphore_index = get_semaphore_index();
    semaphore_value = svlock_getvalue(semaphore_index, 0, 0, 0);

    printf("semaphore[%d] value = %d\n", semaphore_index, semaphore_value);

    return true;
}

static bool display_semaphore_count(void)
{
    int semaphore_index = 0;
    int semaphore_count = 0;

    semaphore_index = get_semaphore_index();
    semaphore_count = svlock_get_count(semaphore_index, 0, 0, 0);

    printf("semaphore[%d] count = %d\n", semaphore_index, semaphore_count);

    return true;
}

static bool display_acquire_semaphore(void)
{
    int semaphore_index = 0;
    int semaphore_value = 0;

    semaphore_index = get_semaphore_index();
    svlock_acquire(semaphore_index, 0, 0, 0);
    usleep(100000);
    semaphore_value = svlock_getvalue(semaphore_index, 0, 0, 0);

    printf("semaphore[%d] value = %d\n", semaphore_index, semaphore_value);

    return true;
}

static bool display_release_semaphore(void)
{
    int semaphore_index = 0;
    int semaphore_value = 0;

    semaphore_index = get_semaphore_index();
    svlock_release(semaphore_index, 0, 0, 0);
    usleep(100000);
    semaphore_value = svlock_getvalue(semaphore_index, 0, 0, 0);

    printf("semaphore[%d] value = %d\n", semaphore_index, semaphore_value);

    return true;
}

static bool display_release_all_semaphore(void)
{
    int semaphore_index = 0;
    int semaphore_value = 0;

    semaphore_index = get_semaphore_index();
    svlock_release_all();
    semaphore_value = svlock_getvalue(semaphore_index, 0, 0, 0);

    printf("semaphore[%d] value = %d\n", semaphore_index, semaphore_value);

    return true;
}
