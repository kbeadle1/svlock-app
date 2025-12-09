#include <stdbool.h>

void process_args(int argc, char *argv[]);
bool want_display_thread_list_sct(void);
bool want_display_pci_devices_list(void);
bool want_display_memory_list(void);
bool want_display_socket_count(void);
bool want_display_cpu_count(void);
bool want_display_thread_count(void);
const char *get_config_file(void);
const char *get_target_file(void);
const char *get_acpi_path(void);
bool want_display_mmconfig_count(void);
bool want_display_mmconfig_list(void);
bool want_display_target_list(void);
bool want_display_verbose(void);
