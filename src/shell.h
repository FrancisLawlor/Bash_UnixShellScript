typedef struct output {
	char* command;
	char* filename;
} output;

output parseInput(char* input);
static void handleCtrlC(int sig);
void printCurrentTime();
