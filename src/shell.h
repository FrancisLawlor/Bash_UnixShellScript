typedef struct RedirectData {
	char* command;
	char* fileName;
} RedirectData;

RedirectData parseRedirectInput(char* input);
static void handleCtrlC(int sig);
void printCurrentTime();
bool isEmptyString(char* input);
