#include <stdlib.h>
#include <sys/sysinfo.h>

long get_uptime()
{
	struct sysinfo info;
	if (sysinfo(&info) != EXIT_SUCCESS) {
		return -1;
	}
	return info.uptime;
}
