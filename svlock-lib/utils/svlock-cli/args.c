#define _GNU_SOURCE /* for getopt_long() */

#include <stdbool.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include "svlock/svlock.h"
#include "args.h"

static void print_usage(const char *progname);

static bool display_thread_list_sct = false;	/*!< --list-thread-sct used */
static bool display_pci_devices_list = false;	/*!< --list-pci-devices used */
static bool display_memory_list = false;	/*!< --list-memory used */
static bool display_socket_count = false;	/*!< --socket-count used */
static bool display_cpu_count = false;		/*!< --cpu-count used */
static bool display_thread_count = false;	/*!< --thread-count used */
static char *config_file_name = NULL;		/*!< option to --config-file */
static char *target_file_name = NULL;		/*!< option to --target-file */
static char *acpi_dir_path = NULL;		/*!< option to --acpi-path */
static bool display_mmconfig_count = false;	/*!< --mmconfig-count used */
static bool display_mmconfig_list = false;	/*!< --mmconfig-list used */
static bool display_target_list = false;	/*!< --list-target used */
static bool display_verbose = false;		/*!< --verbose used */

static struct option opts[] = {
	{"socket-count", no_argument, NULL, 's'},
	{"cpu-count", no_argument, NULL, 'c'},
	{"thread-count", no_argument, NULL, 't'},
	{"list-threads-sct", no_argument, NULL, 'l'},
	{"list-pci-devices", no_argument, NULL, 'p'},
	{"list-memory", no_argument, NULL, 'm'},
	{"config-file", required_argument, NULL, 'f'},
	{"target-file", required_argument, NULL, 'F'},
	{"list-mmconfig", no_argument, NULL, 'L'},
	{"mmconfig-count", no_argument, NULL, 'M'},
	{"list-memtarget", no_argument, NULL, 'g'},
	{"acpi-path", required_argument, NULL, 'a'},
	{"verbose", no_argument, NULL, 'v'},
	{NULL, 0, NULL, 0}
};

/**
 *	process the command line options and update internal state
 *
 *	Other parts of the program can get to the information provided
 *	by command line options via controlled interfaces
 *
 *	Xparam argc the number of entries in the argv array
 *	Xparam argv array of command line options
 */
void process_args(int argc, char *argv[])
{
	int opt;
	bool do_usage = false;
	bool specified_cmd = false;
	const char *optstring = "sctlpmf:F:a:LMgv";

	while (-1 != (opt = getopt_long(argc, argv, optstring, opts, NULL))) {
		switch (opt) {
		case 's':
			display_socket_count = true;
			specified_cmd = true;
			break;
		case 'c':
			display_cpu_count = true;
			specified_cmd = true;
			break;
		case 't':
			display_thread_count = true;
			specified_cmd = true;
			break;
		case 'l':
			display_thread_list_sct = true;
			specified_cmd = true;
			break;
		case 'p':
			display_pci_devices_list = true;
			specified_cmd = true;
			break;
		case 'm':
			display_memory_list = true;
			specified_cmd = true;
			break;
		case 'L':
			display_mmconfig_list = true;
			specified_cmd = true;
			break;
		case 'M':
			display_mmconfig_count = true;
			specified_cmd = true;
			break;
		case 'g':
			display_target_list = true;
			specified_cmd = true;
			break;
		case 'f':
			config_file_name = optarg;
			break;
		case 'F':
			target_file_name = optarg;
			break;
		case 'a':
			acpi_dir_path = optarg;
			break;
		case 'v':
			display_verbose = true;
			break;
		default:
			do_usage = true;
			break;
		}
	}

	if (do_usage || !specified_cmd) {
		print_usage(argv[0]);
		if (!specified_cmd)
			fputs("Must specifiy at least one listing option\n",
			      stderr);
		exit(EXIT_FAILURE);
	}
}

/**
 *	returns an indication of whether the thread list should be displayed
 *	based on the command line arguments.
 *
 *	@return true if list should be displayed, else false
 */
bool want_display_thread_list_sct(void)
{
	return display_thread_list_sct;
}

/**
 *	returns an indication of whether the pci device list should be displayed
 *	based on the command line arguments.
 *
 *	@return true if list should be displayed, else false
 */
bool want_display_pci_devices_list(void)
{
	return display_pci_devices_list;
}

/**
 *	returns an indication of whether the memory list should be displayed
 *	based on the command line arguments.
 *
 *	@return true if list should be displayed, else false
 */
bool want_display_memory_list(void)
{
	return display_memory_list;
}

/**
 *	returns an indication of whether the socket count should be displayed
 *	based on the command line arguments.
 *
 *	@return true if the socket count should be displayed, else false
 */
bool want_display_socket_count(void)
{
	return display_socket_count;
}

/**
 *	returns an indication of whether the core/cpu count should be displayed
 *	based on the command line arguments.
 *
 *	@return true if the count should be displayed, else false
 */
bool want_display_cpu_count(void)
{
	return display_cpu_count;
}

/**
 *	returns an indication of whether the thread count should be displayed
 *	based on the command line arguments.
 *
 *	@return true if the count should be displayed, else false
 */
bool want_display_thread_count(void)
{
	return display_thread_count;
}

/**
 *	returns an indication of whether a count of mmconfig memory blocks
 *	should be displayed based on the command line arguments.
 *
 *	@return true if the count should be displayed, else false
 */
bool want_display_mmconfig_count(void)
{
	return display_mmconfig_count;
}

/**
 *	returns an indication of whether list of mmconfig memory blocks should
 *	be displayed based on the command line arguments.
 *
 *	@return true if the index should be displayed, else false
 */
bool want_display_mmconfig_list(void)
{
	return display_mmconfig_list;
}

/**
 *	returns an indication of whether list of memory target blocks should
 *	be displayed based on the command line arguments.
 *
 *	@return true if the index should be displayed, else false
 */
bool want_display_target_list(void)
{
	return display_target_list;
}

/**
 *	returns an indication of whether items should
 *	be displayed iwthmore details based on the command line arguments.
 *
 *	@return true if verbose is set, else false
 */
bool want_display_verbose(void)
{
	return display_verbose;
}

/**
 *	returns a string specifying the configuration file to load
 *	based on the command line arguments.
 *
 *	@return NULL if no configuration file was specified, otherwise returns
 *		the string for the configuration file.
 */
const char *get_config_file(void)
{
	return config_file_name;
}


/**
 *	return a string specifying the pathname for a file containing
 *	information about available targets.
 *
 *	@returns NULL if no target congifuration file was specified, otherwise
 *		returns the string for the target file
 */
const char *get_target_file(void)
{
	return target_file_name;
}

/**
 *	return a string specifying the path for a directory containing
 *	acpi tables to be used in place of the system's actual acpi
 *	tables located in /sys/firmware/acpi/tables/. This capability
 *	is currently defined for the SRAT, MCFG, and/or NFIT table(s).
 *
 *	@returns NULL if no acpi table directory path was specified, otherwise
 *		returns the string for the acpi path
 */
const char *get_acpi_path(void)
{
	return acpi_dir_path;
}


static const char *msgs[] = {
	"\tlong options may also be used, the long\n",
	"\toption corresponding to each option is\n",
	"\tlisted below along with a description of\n",
	"\twhat the option does.\n\n",
	"output formats are designed such that the output can be used\n",
	"as shell script\n",
	"\t-s --socket-count print count of sockets\n",
	"\t-c --cpu-count print total number of cpu (cores) in the system\n",
	"\t-t --thread-count print total number of hyper-threads in the system\n",
	"\t-l --list-threads-sct list the names of each thread in the system\n",
	"\t\tformat is threadX=sXcXtX\n",
	"\t\twhere the X's are replaced by numeric values\n",
	"\t-p --list-pci-devices list the names of each PCI deice in the system\n",
	"\t-m --list-memory list the names of each memory range in the system\n",
	"\t-f --config-file specify the name of an XML file containing system\n"
	"\t\tinformation\n",
	"\t-F --target-file specify the name of a file containing target\n"
	"\t\tinformation\n",
	"\t-a --acpi-path specify the path to a directory containing acpi table\n"
	"\t\tinformation\n",
	"\t-L --list-mmconfig returns the base address and length for registered "
		"mmconfig block\n",
	"\t-M --mmconfig-count returns the number of mmconfig registered blocks\n",
	"\t-g --list-target returns the base address and length for registered\n"
	"\t\tmemory target blocks\n",
	"\t-v --verbose cause more details to be displayed\n",
	NULL,
};

/**
 *	Print a usage message
 *
 *	@param progname the string used to invoke the program.
 */
static void print_usage(const char *progname)
{
	size_t i;

	fprintf(stderr, "Usage:  %s [long-options] [-sctlpmML] "
		"[-f plat-cap-config_file] [-F target-config-file] "
		"[-a acpi-tables-directory]\n", progname);
	for (i = 0; msgs[i] != NULL; i++)
		fputs(msgs[i], stderr);
}

