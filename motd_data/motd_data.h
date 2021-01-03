#pragma once
#include <stddef.h>
#include <qc.h>

qc_result motd_uptime(size_t len, char dst[static len], qc_err* err);
qc_result motd_loadavg(size_t len, char dst[static len], qc_err* err);
qc_result motd_users(size_t len, char dst[static len], qc_err* err);
qc_result motd_kernel(size_t len, char dst[static len], qc_err* err);
qc_result motd_memory(size_t len, char dst[static len], qc_err* err);
qc_result motd_swap(size_t len, char dst[static len], qc_err* err);
qc_result motd_pids(size_t len, char dst[static len], qc_err* err);
qc_result motd_storage(size_t len, char dst[static len], qc_err* err);
