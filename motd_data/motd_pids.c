#include <stddef.h>
#include <sys/sysinfo.h>
#include <stdio.h>
#include <errno.h>
#include <qc.h>
#include <string.h>
#include "motd_data.h"

#define PID_MAX_FILE "/proc/sys/kernel/pid_max"

qc_result motd_pids(size_t len, char dst[static len], qc_err* err) {
    struct sysinfo info;
    FILE *pid_max_f;
    char pid_max_s[32];
    size_t max_pid, used_percent;

    errno = 0;
    if ((pid_max_f = fopen(PID_MAX_FILE, "r")) == NULL) {
        qc_err_set(err, "Failed to obtain maximum number of PIDs: %s", strerror(errno));
        return QC_FAILURE;
    }
    if (fread(pid_max_s, sizeof(char), sizeof(pid_max_s), pid_max_f) <= 0) {
        fclose(pid_max_f);
        qc_err_set(err, "Failed to read maximum number of PIDs from %s", PID_MAX_FILE);
        return QC_FAILURE;
    }
    fclose(pid_max_f);

    if (qc_str_to_unsigned(pid_max_s, &max_pid, NULL) == QC_FAILURE) {
        qc_err_set(err, "Failed to parse \"%s\" as unsigned integer", pid_max_s);
        return QC_FAILURE;
    }

    errno = 0;
    if (sysinfo(&info) != 0) {
        qc_err_set(err, "Failed to obtain current number of processes from system infromation: %s",
                                 strerror(errno));
        return QC_FAILURE;
    }

    used_percent = 100 * info.procs / max_pid;
    snprintf(dst, len, "[%3zu%%] %zu/%zu",
             used_percent, (size_t) info.procs, max_pid);
    return QC_SUCCESS;
}
