// 80x24 chars is a standard terminal
// so 80 chars per line
// +1 char because \n or \0
#define LINE_SIZE 81
#define LINES 1

// 2 entries per line
// so 40 chars per entry
// +1 char because \n or \0
#define BUF_SIZE (LINE_SIZE-1)/2+1

#define MOTD_STRING \
	"Uptime: %s"
