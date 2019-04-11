// 80x24 chars is a standard terminal
// so 80 chars per line
// +1 char because \n or \0
#define LINE_SIZE 81
#define LINES 8

#define MOTD_STRING \
	"Uptime:             %s\n" \
	"Load average:       %s\n" \
	"Users:              %s\n" \
	"Kernel:             %s\n" \
	"Used memory:        %s\n" \
	"Used swap:          %s\n" \
	"Total PIDs:         %s\n" \
	"Root usage:         %s"
