#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <sys/sysinfo.h>
#include <sys/utsname.h>
#include <sys/statvfs.h>
#include <utmp.h>

#define PID_MAX_FILE "/proc/sys/kernel/pid_max"
#define MEMINFO_FILE "/proc/meminfo"
#define MEMAVAIL_STR "MemAvailable:"
#define MEMTOTAL_STR "MemTotal:"

int format_uptime(char *dst, size_t len)
{
	struct sysinfo info;
	struct tm *dt;

	if (sysinfo(&info) != EXIT_SUCCESS) {
		return -1;
	}
	dt = gmtime(&info.uptime);

	// dt.tm_year starts from 1970, so we just subtract it
	dt->tm_year -= 70;
	// dt.tm_mday is 1-31, but we need to count the quantity of days
	dt->tm_mday -= 1;

	snprintf(dst, len, "%d months, %d days, %02d:%02d:%02d",
			dt->tm_year*12 + dt->tm_mon,
			dt->tm_mday,
			dt->tm_hour,
			dt->tm_min,
			dt->tm_sec);
	return EXIT_SUCCESS;
}

int format_loadavg(char *dst, size_t len)
{
	double loadavg[3];

	if (getloadavg(loadavg, 3) != sizeof(loadavg)/sizeof(double)) {
		return -1;
	}

	snprintf(dst, len, "%.2f, %.2f, %.2f",
			loadavg[0],
			loadavg[1],
			loadavg[2]);
	return EXIT_SUCCESS;
}

int format_kernel(char *dst, size_t len)
{
	struct utsname name;

	if (uname(&name) == -1) {
		perror("uname");
		return -1;
	}

	snprintf(dst, len, "%s",
			name.release);
	return EXIT_SUCCESS;
}

int format_users(char *dst, size_t len)
{
	unsigned users;
	struct utmp *utmps;

	users = 0;
	setutent();
	while ((utmps = getutent()))
		if ((utmps->ut_type == USER_PROCESS) &&
		    (utmps->ut_name[0] != '\0'))
			++users;
	endutent();

	snprintf(dst, len, "%d", users);
	return EXIT_SUCCESS;
}

int format_memory(char *dst, size_t len)
{
	unsigned free_ram, total_ram;
	int_fast8_t used_percent;
	FILE *meminfo;
	char *line;
	size_t rlen;
	ssize_t nread;

	meminfo = fopen(MEMINFO_FILE, "r");
	if (meminfo == NULL) {
		perror("fopen");
		return -1;
	}
	rlen = 0;
	while ((nread = getline(&line, &rlen, meminfo)) != -1) {
		if (!strncmp(line, MEMTOTAL_STR, sizeof(MEMTOTAL_STR)-1)) {
			while (line[0] < '0' || line[0] > '9') ++line;
			total_ram = atoi(line)/1024;
		}
		if (!strncmp(line, MEMAVAIL_STR, sizeof(MEMAVAIL_STR)-1)) {
			while (line[0] < '0' || line[0] > '9') ++line;
			free_ram = atoi(line)/1024;
		}
	}
	fclose(meminfo);
	if (!free_ram || !total_ram) return -1;

	used_percent = (100 * (total_ram - free_ram)) / total_ram;
	snprintf(dst, len, "[%d%%] %d/%d MB",
			used_percent,
			(total_ram - free_ram),
			total_ram);
	return EXIT_SUCCESS;
}

int format_swap(char *dst, size_t len)
{
	struct sysinfo info;
	int_fast8_t used_percent;

	if (sysinfo(&info) != EXIT_SUCCESS) {
		return -1;
	}
	if (info.totalswap == 0) {
		snprintf(dst, len, "%s", "none");
		return EXIT_SUCCESS;
	}

	used_percent = (100 * (info.totalswap - info.freeswap)) /
		info.totalswap;
	snprintf(dst, len, "[%d%%] %ld/%ld MB",
			used_percent,
			(info.totalswap - info.freeswap)/1024/1024,
			info.totalswap/1024/1024);
	return EXIT_SUCCESS;
}

int format_pids(char *dst, size_t len)
{
	struct sysinfo info;
	FILE *pid_max_f;
	char pid_max_s[32];
	long max_pid;
	int_fast8_t used_percent;

	pid_max_f = fopen(PID_MAX_FILE, "r");
	if (pid_max_f == NULL) {
		perror("fopen");
		return -1;
	}
	fread(pid_max_s, sizeof(char), sizeof(pid_max_s), pid_max_f);
	fclose(pid_max_f);
	max_pid = atol(pid_max_s);

	if (sysinfo(&info) != EXIT_SUCCESS) {
		return -1;
	}
	used_percent = 100 * info.procs / max_pid;

	snprintf(dst, len, "[%d%%] %d/%ld",
			used_percent, info.procs, max_pid);
	return EXIT_SUCCESS;
}

int format_storage(char *dst, size_t len)
{
	struct statvfs stat;
	fsblkcnt_t f_bused;
	int_fast8_t used_percent;

	if (statvfs("/", &stat) == -1) {
		perror("statvfs");
		return -1;
	}
	f_bused = stat.f_blocks - stat.f_bfree;
	used_percent = 100 * f_bused / stat.f_blocks;

	snprintf(dst, len, "[%d%%] %ld/%ld GB",
			used_percent,
			stat.f_bsize * f_bused / 1024 / 1024 / 1024,
			stat.f_bsize * stat.f_blocks / 1024 / 1024 / 1024);
	return EXIT_SUCCESS;
}
