#include <errno.h>
#include <stddef.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <time.h>
#include <stdio.h>
#include <qc.h>
#include "motd_data.h"

qc_result motd_uptime(size_t len, char dst[static len], qc_err* err) {
    struct sysinfo info;
    struct tm dt;

    errno = 0;
    if (sysinfo(&info) != 0) {
        qc_err_set(err, "Failed to get system info: %s", strerror(errno));
        return QC_FAILURE;
    }
    gmtime_r(&info.uptime, &dt);

    // dt.tm_year starts from 1970, so we just subtract it
    dt.tm_year -= 70;
    // dt.tm_mday is 1-31, but we need to count the quantity of days
    dt.tm_mday -= 1;

    snprintf(dst, len, "%d months, %d days, %02d:%02d:%02d",
             dt.tm_year * 12 + dt.tm_mon,
             dt.tm_mday,
             dt.tm_hour,
             dt.tm_min,
             dt.tm_sec);
    return QC_SUCCESS;
}
