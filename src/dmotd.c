#include <stdio.h>
#include <stdlib.h>

#include "sysinfo.h"

int main(void)
{
	char motd[256];
	char tmp[64];
	if (uptime((char*) tmp, sizeof(tmp)) != EXIT_SUCCESS) {
		perror("uptime");
		return EXIT_FAILURE;
	}
	snprintf((char*) motd, sizeof(motd), "Uptime: %s\n", tmp);
	printf("%s", motd);
	return EXIT_SUCCESS;
}
