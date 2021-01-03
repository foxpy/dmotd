#include <stdio.h>
#include <stdlib.h>
#include "motd_data.h"

#define LINES 8
#define LINE_SIZE 81

void fail(char* func)
{
    fprintf(stderr, "format_%s: Failed to get %s.\n", func, func);
    exit(EXIT_FAILURE);
}

int main(void)
{
    char motd[LINES*LINE_SIZE];
    char uptime_s[LINE_SIZE];
    char loadavg_s[LINE_SIZE];
    char users_s[LINE_SIZE];
    char kernel_s[LINE_SIZE];
    char memory_s[LINE_SIZE];
    char swap_s[LINE_SIZE];
    char pids_s[LINE_SIZE];
    char storage_s[LINE_SIZE];

    qc_err* err = qc_err_new();

    if (motd_uptime(uptime_s, sizeof(uptime_s), err) == QC_FAILURE) {
        qc_err_fatal(err, "Failed to get uptime");
    }

    if (format_loadavg((char*) loadavg_s, sizeof(loadavg_s))
        != EXIT_SUCCESS) {
        fail("loadavg");
    }

    if (format_users((char*) users_s, sizeof(users_s))
        != EXIT_SUCCESS) {
        fail("users");
    }

    if (format_kernel((char*) kernel_s, sizeof(kernel_s))
        != EXIT_SUCCESS) {
        fail("kernel");
    }

    if (format_memory((char*) memory_s, sizeof(memory_s))
        != EXIT_SUCCESS) {
        fail("memory");
    }

    if (format_swap((char*) swap_s, sizeof(swap_s))
        != EXIT_SUCCESS) {
        fail("swap");
    }

    if (format_pids((char*) pids_s, sizeof(pids_s))
        != EXIT_SUCCESS) {
        fail("pids");
    }

    if (format_storage((char*) storage_s, sizeof(storage_s))
        != EXIT_SUCCESS) {
        fail("storage");
    }

    qc_err_free(err);

    snprintf((char*) motd, sizeof(motd), MOTD_STRING,
             uptime_s,
             loadavg_s,
             users_s,
             kernel_s,
             memory_s,
             swap_s,
             pids_s,
             storage_s);
    puts(motd);
    return EXIT_SUCCESS;
}
