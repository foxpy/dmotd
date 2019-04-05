#include <stdio.h>
#include <stdlib.h>

#include "sysinfo.h"
#include "config.h"

int main(void)
{
	char motd[LINES*LINE_SIZE];
	char uptime_s[LINE_SIZE];
	char loadavg_s[LINE_SIZE];
	char memory_s[LINE_SIZE];

	if (format_uptime((char*) uptime_s, sizeof(uptime_s))
			!= EXIT_SUCCESS) {
		perror("format_uptime");
		return EXIT_FAILURE;
	}

	if (format_loadavg((char*) loadavg_s, sizeof(loadavg_s))
			!= EXIT_SUCCESS) {
		perror("format_loadavg");
		return EXIT_FAILURE;
	}

	if (format_memory((char*) memory_s, sizeof(memory_s))
			!= EXIT_SUCCESS) {
		perror("format_memory");
		return EXIT_FAILURE;
	}

	snprintf((char*) motd, sizeof(motd), MOTD_STRING,
			uptime_s,
			loadavg_s,
			memory_s);
	puts(motd);
	return EXIT_SUCCESS;
}
