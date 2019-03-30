#include <stdio.h>
#include <stdlib.h>

#include "sysinfo.h"

int main(void)
{
	printf("Uptime: %zd\n", get_uptime());
	return EXIT_SUCCESS;
}
