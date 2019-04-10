#include <stdio.h>
#include <stdlib.h>

#include "info.h"
#include "config.h"

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
	char memory_s[LINE_SIZE];
	char pids_s[LINE_SIZE];

	if (format_uptime((char*) uptime_s, sizeof(uptime_s))
			!= EXIT_SUCCESS) {
		fail("uptime");
	}

	if (format_loadavg((char*) loadavg_s, sizeof(loadavg_s))
			!= EXIT_SUCCESS) {
		fail("loadavg");
	}

	if (format_memory((char*) memory_s, sizeof(memory_s))
			!= EXIT_SUCCESS) {
		fail("memory");
	}

	if (format_pids((char*) pids_s, sizeof(pids_s))
			!= EXIT_SUCCESS) {
		fail("pids");
	}

	snprintf((char*) motd, sizeof(motd), MOTD_STRING,
			uptime_s,
			loadavg_s,
			memory_s,
			pids_s);
	puts(motd);
	return EXIT_SUCCESS;
}
