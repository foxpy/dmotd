#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <qc.h>

#define MEMINFO_FILE "/proc/meminfo"
#define MEMAVAIL_STR "MemAvailable:"
#define MEMTOTAL_STR "MemTotal:"

qc_result motd_memory(size_t len, char dst[static len], qc_err* err) {
    size_t free_ram, total_ram, used_percent, rlen = 0;
    FILE* meminfo;
    char* line_buffer = NULL;

    errno = 0;
    meminfo = fopen(MEMINFO_FILE, "r");
    if (meminfo == NULL) {
        qc_err_set(err, "Failed to open %s: %s", MEMINFO_FILE, strerror(errno));
        return QC_FAILURE;
    }
    while (getline(&line_buffer, &rlen, meminfo) != -1) {
        char* line = line_buffer;
        if (strncmp(line, MEMTOTAL_STR, sizeof(MEMTOTAL_STR) - 1) == 0) {
            while (line[0] < '0' || line[0] > '9') ++line;
            if (qc_str_to_unsigned(line, &total_ram, NULL) == QC_FAILURE) {
                qc_err_set(err, "Failed to parse %s as unsigned integer", line);
                free(line_buffer);
                return QC_FAILURE;
            } else {
                total_ram /= 1024;
            }
        } else if (strncmp(line, MEMAVAIL_STR, sizeof(MEMAVAIL_STR) - 1) == 0) {
            while (line[0] < '0' || line[0] > '9') ++line;
            if (qc_str_to_unsigned(line, &free_ram, NULL) == QC_FAILURE) {
                qc_err_set(err, "Failed to parse %s as unsigned integer", line);
                free(line_buffer);
                return QC_FAILURE;
            } else {
                free_ram /= 1024;
            }
        }
    }
    free(line_buffer);
    fclose(meminfo);
    if (total_ram == 0) {
        qc_err_set(err, "Failed to obtain amount of RAM");
        return QC_FAILURE;
    }

    used_percent = (100 * (total_ram - free_ram)) / total_ram;
    snprintf(dst, len, "[%3zu%%] %zu/%zu MB", used_percent, (total_ram - free_ram), total_ram);
    return QC_SUCCESS;
}
