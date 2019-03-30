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

	snprintf(dst, len, "%d months, %02d:%02d:%02d",
			dt->tm_year*12 + dt->tm_yday, dt->tm_hour, dt->tm_min, dt->tm_sec);
	return EXIT_SUCCESS;
}
