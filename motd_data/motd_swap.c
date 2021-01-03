#include <stddef.h>
#include <sys/sysinfo.h>
#include <stdio.h>
#include <qc.h>
#include "motd_data.h"

qc_result motd_swap(size_t len, char dst[static len], qc_err* err) {
    struct sysinfo info;
    size_t used_percent;

    if (sysinfo(&info) != 0) {
        qc_err_set(err, "Failed to obtain system information");
        return QC_FAILURE;
    }
    if (info.totalswap == 0) {
        snprintf(dst, len, "%s", "none");
        return QC_SUCCESS;
    }

    used_percent = (100 * (info.totalswap - info.freeswap)) / info.totalswap;
    snprintf(dst, len, "[%3zu%%] %zu/%zu MB",
             used_percent,
             (size_t) ((info.totalswap - info.freeswap) / 1024 / 1024),
             (size_t) (info.totalswap / 1024 / 1024));
    return QC_SUCCESS;
}
