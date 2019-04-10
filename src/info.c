#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <sys/sysinfo.h>

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

int format_memory(char *dst, size_t len)
{
	struct sysinfo info;
	int8_t used_percent;

	if (sysinfo(&info) != EXIT_SUCCESS) {
		return -1;
	}

	used_percent = (100 * (info.totalram - info.freeram)) / info.totalram;
	snprintf(dst, len, "%ld/%ld MB [%d%%]",
			(info.totalram - info.freeram)/1024/1024,
			info.totalram/1024/1024, used_percent);
	return EXIT_SUCCESS;
}
