#include <stdio.h>
#include <stdlib.h>
#include <qc.h>
#include "motd_data.h"

static char const* motd_string =
    "Uptime:             %s\n"
    "Load average:       %s\n"
    "Users:              %s\n"
    "Kernel:             %s\n"
    "Used memory:        %s\n"
    "Used swap:          %s\n"
    "Used PIDs:          %s\n"
    "Used storage on /:  %s\n"
;

int main(void) {
#   define LINE_SIZE 81
    char uptime_s[LINE_SIZE];
    char loadavg_s[LINE_SIZE];
    char users_s[LINE_SIZE];
    char kernel_s[LINE_SIZE];
    char memory_s[LINE_SIZE];
    char swap_s[LINE_SIZE];
    char pids_s[LINE_SIZE];
    char storage_s[LINE_SIZE];

    qc_err* err = qc_err_new();
    if (motd_uptime(sizeof(uptime_s), uptime_s, err) == QC_FAILURE) {
        qc_err_fatal(err, "Failed to get uptime");
    } else if (motd_loadavg(sizeof(loadavg_s), loadavg_s, err) == QC_FAILURE) {
        qc_err_fatal(err, "Failed to get load average");
    } else if (motd_users(sizeof(users_s), users_s, err) == QC_FAILURE) {
        qc_err_fatal(err, "Failed to get number of users");
    } else if (motd_kernel(sizeof(kernel_s), kernel_s, err) == QC_FAILURE) {
        qc_err_fatal(err, "Failed to get kernel version");
    } else if (motd_memory(sizeof(memory_s), memory_s, err) == QC_FAILURE) {
        qc_err_fatal(err, "Failed to get RAM info");
    } else if (motd_swap(sizeof(swap_s), swap_s, err) == QC_FAILURE) {
        qc_err_fatal(err, "Failed to get swap info");
    } else if (motd_pids(sizeof(pids_s), pids_s, err) == QC_FAILURE) {
        qc_err_fatal(err, "Failed to get number of PIDS");
    } else if (motd_storage(sizeof(storage_s), storage_s, err) == QC_FAILURE) {
        qc_err_fatal(err, "Failed to get storage info");
    }
    qc_err_free(err);

    printf(motd_string, uptime_s, loadavg_s, users_s, kernel_s, memory_s, swap_s, pids_s, storage_s);
    return EXIT_SUCCESS;
}
