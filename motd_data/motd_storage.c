#include <stddef.h>
#include <sys/statvfs.h>
#include <stdio.h>
#include <qc.h>
#include <errno.h>
#include <string.h>
#include "motd_data.h"

qc_result motd_storage(size_t len, char dst[static len], qc_err* err) {
    struct statvfs stat;
    fsblkcnt_t f_bused;
    size_t used_percent;

    errno = 0;
    if (statvfs("/", &stat) == -1) {
        qc_err_set(err, "Failed to obtain information about file system: %s", strerror(errno));
        return QC_FAILURE;
    }

    f_bused = stat.f_blocks - stat.f_bfree;
    used_percent = 100 * f_bused / stat.f_blocks;
    snprintf(dst, len, "[%3zu%%] %zu/%zu GB",
             used_percent,
             (size_t) (stat.f_bsize * f_bused / 1024 / 1024 / 1024),
             (size_t) (stat.f_bsize * stat.f_blocks / 1024 / 1024 / 1024));
    return QC_SUCCESS;
}
