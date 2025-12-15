#include <stdbool.h>

void process_args(int argc, char *argv[]);
bool want_display_semaphore_value(void);
bool want_display_semaphore_count(void);
bool want_display_acquire_semaphore(void);
bool want_display_release_semaphore(void);
bool want_display_release_all_semaphore(void);
int get_semaphore_index(void);
