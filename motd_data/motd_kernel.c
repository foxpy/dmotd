#include <stddef.h>
#include <stdio.h>
#include <sys/utsname.h>
#include <qc.h>

qc_result motd_kernel(size_t len, char dst[static len], qc_err* err) {
    struct utsname name;
    if (uname(&name) == -1) {
        qc_err_set(err, "Failed to obtain name of current system");
        return QC_FAILURE;
    }
    snprintf(dst, len, "%s", name.release);
    return QC_SUCCESS;
}
