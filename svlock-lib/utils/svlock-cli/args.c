#define _GNU_SOURCE /* for getopt_long() */

#include <stdbool.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include "svlock/svlock.h"
#include "args.h"

static void print_usage(const char *progname);

static int  semaphore_index = 0;        /*!< option to --index */
static bool display_semaphore_value = false;    /*!< --value used */
static bool display_semaphore_count = false;    /*!< --count used */
static bool display_acquire_semaphore = false;    /*!< --acquire used */
static bool display_release_semaphore = false;    /*!< --release used */
static bool display_release_all_semaphore = false;    /*!< --release-all used */

static struct option opts[] = {
    {"index", required_argument, NULL, 'i'},
    {"value", no_argument, NULL, 'v'},
    {"count", no_argument, NULL, 'c'},
    {"acquire", no_argument, NULL, 'a'},
    {"release", no_argument, NULL, 'r'},
    {"release-all", no_argument, NULL, 'R'},
    {NULL, 0, NULL, 0}
};

/**
 *  process the command line options and update internal state
 *
 *  Other parts of the program can get to the information provided
 *  by command line options via controlled interfaces
 *
 *  Xparam argc the number of entries in the argv array
 *  Xparam argv array of command line options
 */
void process_args(int argc, char *argv[])
{
    int opt;
    bool do_usage = false;
    bool specified_cmd = false;
    const char *optstring = "i:vcarR";

    while (-1 != (opt = getopt_long(argc, argv, optstring, opts, NULL))) {
        switch (opt) {
        case 'i':
            semaphore_index = atoi(optarg);
            break;
        case 'v':
            display_semaphore_value = true;
            specified_cmd = true;
            break;
        case 'c':
            display_semaphore_count = true;
            specified_cmd = true;
            break;
        case 'a':
            display_acquire_semaphore = true;
            specified_cmd = true;
            break;
        case 'r':
            display_release_semaphore = true;
            specified_cmd = true;
            break;
        case 'R':
            display_release_all_semaphore = true;
            specified_cmd = true;
            break;
        default:
            do_usage = true;
            break;
        }
    }

    if (do_usage || !specified_cmd) {
        print_usage(argv[0]);
        if (!specified_cmd)
            fputs("Must specifiy at least one display option\n",
                  stderr);
        exit(EXIT_FAILURE);
    }
}

/**
 *  returns an indication of whether the semaphore value should be displayed
 *  based on the command line arguments.
 *
 *  @return true if list should be displayed, else false
 */
bool want_display_semaphore_value(void)
{
    return display_semaphore_value;
}

bool want_display_semaphore_count(void)
{
    return display_semaphore_count;
}

bool want_display_acquire_semaphore(void)
{
    return display_acquire_semaphore;
}

bool want_display_release_semaphore(void)
{
    return display_release_semaphore;
}

bool want_display_release_all_semaphore(void)
{
    return display_release_all_semaphore;
}

/**
 *  return a integer specifying the semaphore index
 *
 *  @returns 0 if no semaphore index was specified, otherwise
 *      returns the integer for the semaphore index
 */
int  get_semaphore_index(void)
{
    return semaphore_index;
}

static const char *msgs[] = {
    "\tlong options may also be used, the long\n",
    "\toption corresponding to each option is\n",
    "\tlisted below along with a description of\n",
    "\twhat the option does.\n\n",
    "output formats are designed such that the output can be used\n",
    "as shell script\n",
    "\t-i --index specify semaphore index\n",
    "\t-v --value print current semaphore value\n",
    "\t-c --count print current semaphore count\n",
    "\t-a --acquire acquire semaphore\n",
    "\t-r --release release semaphore value\n",
    "\t-R --release-all release all semaphores\n",
    NULL,
};

/**
 *  Print a usage message
 *
 *  @param progname the string used to invoke the program.
 */
static void print_usage(const char *progname)
{
    size_t i;

    fprintf(stderr, "Usage:  %s [long-options] [-ivcarR] "
        "[-i semaphore-index]\n", progname);
    for (i = 0; msgs[i] != NULL; i++)
        fputs(msgs[i], stderr);
}
