#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <sys/statvfs.h>
#include <utmp.h>
#include "motd_data.h"

#define PID_MAX_FILE "/proc/sys/kernel/pid_max"

int format_pids(char *dst, size_t len) {
    struct sysinfo info;
    FILE *pid_max_f;
    char pid_max_s[32];
    long max_pid;
    int_fast8_t used_percent;

    if ((pid_max_f = fopen(PID_MAX_FILE, "r")) == NULL) {
        perror("fopen");
        return -1;
    }
    if (fread(pid_max_s, sizeof(char), sizeof(pid_max_s), pid_max_f))
        max_pid = atol(pid_max_s);
    fclose(pid_max_f);

    if (sysinfo(&info) != EXIT_SUCCESS) {
        return -1;
    }
    used_percent = 100 * info.procs / max_pid;

    snprintf(dst, len, "[%3d%%] %d/%ld",
             used_percent, info.procs, max_pid);
    return EXIT_SUCCESS;
}

int format_storage(char *dst, size_t len) {
    struct statvfs stat;
    fsblkcnt_t f_bused;
    int_fast8_t used_percent;

    if (statvfs("/", &stat) == -1) {
        perror("statvfs");
        return -1;
    }
    f_bused = stat.f_blocks - stat.f_bfree;
    used_percent = 100 * f_bused / stat.f_blocks;

    snprintf(dst, len, "[%3d%%] %ld/%ld GB",
             used_percent,
             stat.f_bsize * f_bused / 1024 / 1024 / 1024,
             stat.f_bsize * stat.f_blocks / 1024 / 1024 / 1024);
    return EXIT_SUCCESS;
}
