#include <stdio.h>
#include <stdlib.h>

#include "sysinfo.h"
#include "config.h"

int main(void)
{
	char motd[LINES*LINE_SIZE];
	char uptime_s[LINE_SIZE];

	if (uptime((char*) uptime_s, sizeof(uptime_s)) != EXIT_SUCCESS) {
		perror("uptime");
		return EXIT_FAILURE;
	}

	snprintf((char*) motd, sizeof(motd), MOTD_STRING, uptime_s);
	puts(motd);
	return EXIT_SUCCESS;
}
