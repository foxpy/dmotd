#include <stddef.h>
#include <utmp.h>
#include <stdio.h>
#include <qc.h>
#include "motd_data.h"

qc_result motd_users(size_t len, char dst[static len], qc_err* err) {
    unsigned users;
    struct utmp *utmps;

    users = 0;
    setutent();
    while ((utmps = getutent()) != NULL) {
        if ((utmps->ut_type == USER_PROCESS) && (utmps->ut_name[0] != '\0')) {
            ++users;
        }
    }
    endutent();

    snprintf(dst, len, "%d", users);
    return QC_SUCCESS;
}
