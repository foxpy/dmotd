// 80x24 chars is a standard terminal
// so 80 chars per line
// +1 char because \n or \0
#define LINE_SIZE 81
#define LINES 3

#define MOTD_STRING \
	"Uptime:             %s\n" \
	"Load average:       %s\n" \
	"Used memory:        %s"