#pragma once
#include <stddef.h>
#include <qc.h>

#define MOTD_STRING \
    "Uptime:             %s\n" \
    "Load average:       %s\n" \
    "Users:              %s\n" \
    "Kernel:             %s\n" \
    "Used memory:        %s\n" \
    "Used swap:          %s\n" \
    "Used PIDs:          %s\n" \
    "Used storage on /:  %s"

qc_result motd_uptime(size_t len, char dst[static len], qc_err* err);
qc_result motd_loadavg(size_t len, char dst[static len], qc_err* err);
int format_users(char *dst, size_t len);
int format_kernel(char *dst, size_t len);
int format_memory(char *dst, size_t len);
int format_swap(char *dst, size_t len);
int format_pids(char *dst, size_t len);
int format_storage(char *dst, size_t len);