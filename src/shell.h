//struct used to return command and filename from function parsing input containing '>'
typedef struct output{
	char * phrase;
	char * filename;
} output;

//function declarations
output parseInput(char * input);
static void handlectrlc(int sig);
void printCurrentTime();

//Name: Francis Lawlor
//Student Number: 14461158
//Email: francis.lawlor@ucdconnect.ie