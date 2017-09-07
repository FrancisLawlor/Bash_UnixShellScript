typedef struct RedirectData {
	char* command;
	char* fileName;
} RedirectData;

output parseInput(char* input);
static void handleCtrlC(int sig);
void printCurrentTime();
