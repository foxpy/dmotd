#include <stddef.h>
#include <stdio.h>
#include <qc.h>
#include <errno.h>
#include <string.h>
#include "motd_data.h"

#define LOADAVG_FILE "/proc/loadavg"
#define BUFFER_SIZE 32

qc_result motd_loadavg(size_t len, char dst[static len], qc_err* err) {
    FILE *load_avg_f;
    char load_avg_s[BUFFER_SIZE];
    size_t i, space;

    errno = 0;
    if ((load_avg_f = fopen(LOADAVG_FILE, "r")) == NULL) {
        qc_err_set(err, "Failed to open loadavg file for reading: %s", strerror(errno));
        return QC_FAILURE;
    }
    if (fread(load_avg_s, sizeof(char), BUFFER_SIZE, load_avg_f) > 0) {
        i = space = 0;
        while (space < 3) if (load_avg_s[++i] == ' ') ++space;
        load_avg_s[i] = '\0';
    } else {
        fclose(load_avg_f);
        qc_err_set(err, "Failed to read loadavg data from loadavg file");
        return QC_FAILURE;
    }
    fclose(load_avg_f);

    snprintf(dst, len, "%s", load_avg_s);
    return QC_SUCCESS;
}
