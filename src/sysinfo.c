#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/sysinfo.h>

int uptime(char *dst, size_t len)
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
