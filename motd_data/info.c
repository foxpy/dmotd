#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <sys/utsname.h>
#include <sys/statvfs.h>
#include <utmp.h>
#include "motd_data.h"

#define PID_MAX_FILE "/proc/sys/kernel/pid_max"
#define MEMINFO_FILE "/proc/meminfo"
#define MEMAVAIL_STR "MemAvailable:"
#define MEMTOTAL_STR "MemTotal:"

int format_kernel(char *dst, size_t len) {
    struct utsname name;

    if (uname(&name) == -1) {
        perror("uname");
        return -1;
    }

    snprintf(dst, len, "%s",
             name.release);
    return EXIT_SUCCESS;
}

int format_memory(char *dst, size_t len) {
    unsigned free_ram, total_ram;
    int_fast8_t used_percent;
    FILE *meminfo;
    char *line;
    size_t rlen;
    ssize_t nread;

    meminfo = fopen(MEMINFO_FILE, "r");
    if (meminfo == NULL) {
        perror("fopen");
        return -1;
    }
    rlen = 0;
    while ((nread = getline(&line, &rlen, meminfo)) != -1) {
        if (!strncmp(line, MEMTOTAL_STR, sizeof(MEMTOTAL_STR) - 1)) {
            while (line[0] < '0' || line[0] > '9') ++line;
            total_ram = atoi(line) / 1024;
        }
        if (!strncmp(line, MEMAVAIL_STR, sizeof(MEMAVAIL_STR) - 1)) {
            while (line[0] < '0' || line[0] > '9') ++line;
            free_ram = atoi(line) / 1024;
        }
    }
    fclose(meminfo);
    if (!free_ram || !total_ram) return -1;

    used_percent = (100 * (total_ram - free_ram)) / total_ram;
    snprintf(dst, len, "[%3d%%] %d/%d MB",
             used_percent,
             (total_ram - free_ram),
             total_ram);
    return EXIT_SUCCESS;
}

int format_swap(char *dst, size_t len) {
    struct sysinfo info;
    int_fast8_t used_percent;

    if (sysinfo(&info) != EXIT_SUCCESS) {
        return -1;
    }
    if (info.totalswap == 0) {
        snprintf(dst, len, "%s", "none");
        return EXIT_SUCCESS;
    }

    used_percent = (100 * (info.totalswap - info.freeswap)) /
                   info.totalswap;
    snprintf(dst, len, "[%3d%%] %ld/%ld MB",
             used_percent,
             (info.totalswap - info.freeswap) / 1024 / 1024,
             info.totalswap / 1024 / 1024);
    return EXIT_SUCCESS;
}

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
