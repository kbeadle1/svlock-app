//
// Created by kbeadle on 07/09/25.
//

#ifndef SVLOCK_APP_H
#define SVLOCK_APP_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> /* open */
#include <unistd.h> /* exit */
#include <sys/ioctl.h> /* ioctl */
#include <string.h>
#include <getopt.h>
#include <linux/kdev_t.h>
#include <sys/stat.h>

// #ifdef CONFIG_SVOS
//#include <sv/svlib.h>
//#include <sv/sv.h>
#include <sv/svlock.h>
// #endif

#define SVLOCK_PROC_DEVICES_PATH    "/proc/devices"
#define SVLOCK_DEVICE_PATH          "/dev/"SVLOCK_DEVICE_NAME

const char *usage =
        "Usage: %s\n"
        "          [--init]\n"
        "          [--acquire]\n"
        "          [--release]\n"
        "          [--releaseall]\n"
        "          [--deinit]\n"
        "          [--deinitall]\n"
        "          [--list]\n"
        "          [--unload]\n"
        "          [--nlocks <nlocks>]\n"
        "          [--tag <tag>]\n"
        "          [--pid <pid>]\n"
        "          [--tid <tid>]\n"
        "          [--test]\n"
        "          [--posix]\n"
;

static int do_log = 0;
static int unload_driver = 0;
static int switch_cmd = 0;
static int halt_cmd = 0;
static int halt_return_code = 0;
static int alloc_cmd = 0;
static uint64_t phy_addr_base = 0;
static uint64_t alloc_size = 0;
static int debug = 0;
static int init_cmd = 0;
static int acquire_cmd = 0;
static int release_cmd = 0;
static int deinit_cmd = 0;
static int list_cmd = 0;
static int test_cmd = 0;
static int posix_cmd = 0;
static int releaseall_cmd = 0;
static int deinitall_cmd = 0;
static uint64_t nlocks = 0;
static uint64_t tag = 0;
static uint64_t pid = 0;
static uint64_t tid = 0;

#endif //SVLOCK_APP_H
