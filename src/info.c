#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/sysinfo.h>
#include <sys/utsname.h>
#include <sys/statvfs.h>
#include <utmp.h>

#define PID_MAX_FILE "/proc/sys/kernel/pid_max"

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
	struct sysinfo info;
	int_fast8_t used_percent;

	if (sysinfo(&info) != EXIT_SUCCESS) {
		return -1;
	}

	used_percent = (100 * (info.totalram - info.freeram)) / info.totalram;
	snprintf(dst, len, "%ld/%ld MB [%d%%]",
			(info.totalram - info.freeram)/1024/1024,
			info.totalram/1024/1024, used_percent);
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

	used_percent = (100 * (info.totalswap - info.freeswap)) / info.totalswap;
	snprintf(dst, len, "%ld/%ld MB [%d%%]",
			(info.totalswap - info.freeswap)/1024/1024,
			info.totalswap/1024/1024, used_percent);
	return EXIT_SUCCESS;
}

int format_pids(char *dst, size_t len)
{
	struct sysinfo info;
	int pid_max_fd;
	char pid_max_s[32];
	long max_pid;
	int_fast8_t used_percent;

	pid_max_fd = open(PID_MAX_FILE, O_RDONLY);
	if (pid_max_fd == -1) {
		perror("open");
		return -1;
	}
	if (read(pid_max_fd, pid_max_s, sizeof(pid_max_s)) == -1) {
		perror("read");
		return -1;
	}
	max_pid = atol(pid_max_s);

	if (sysinfo(&info) != EXIT_SUCCESS) {
		return -1;
	}
	used_percent = 100 * info.procs / max_pid;

	snprintf(dst, len, "%d/%ld [%d%%]",
			info.procs, max_pid, used_percent);
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

	snprintf(dst, len, "%ld/%ld GB [%d%%]",
			stat.f_bsize * f_bused / 1024 / 1024 / 1024,
			stat.f_bsize * stat.f_blocks / 1024 / 1024 / 1024,
			used_percent);
	return EXIT_SUCCESS;
}
