#define PID_MAX_FILE "/proc/sys/kernel/pid_max"
#define LOADAVG_FILE "/proc/loadavg"
#define MEMINFO_FILE "/proc/meminfo"
#define MEMAVAIL_STR "MemAvailable:"
#define MEMTOTAL_STR "MemTotal:"

int format_uptime(char *dst, size_t len);
int format_loadavg(char *dst, size_t len);
int format_users(char *dst, size_t len);
int format_kernel(char *dst, size_t len);
int format_memory(char *dst, size_t len);
int format_swap(char *dst, size_t len);
int format_pids(char *dst, size_t len);
int format_storage(char *dst, size_t len);
