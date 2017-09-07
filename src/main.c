#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <dirent.h>
#include <errno.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include "shell.h"

static void handleCtrlC(int sig) {
	signal(SIGINT, handleCtrlC);
	printf("\n");
	printCurrentTime();
	printf(" # ");
	fflush(stdout);
}

int main(void) {
	char input[1024];

	while(1) {
		output out;
		char** argv = malloc(sizeof(char*));
		pid_t child_pid;
		int childStats;

		signal(SIGINT, handleCtrlC);

  		printCurrentTime();
		printf(" # ");

		fflush(stdin);

		// Get input from user and break if input is ctrl + d
		if (!fgets(input, 1024, stdin)) {
			break;
		}

		// Remove '\n' from end of input
		input[strlen(input) - 1] = '\0';

		// Check if input has '>'
		char* greaterThanSymbol = strchr(input, '>');

		if (greaterThanSymbol != NULL) {
			out = parseInput(input);
		}

		int numberOfArguments = 0;
		char* upToSpace = strtok(input, " ");

		while (upToSpace != NULL){
			argv[numberOfArguments++] = upToSpace;
			argv = realloc(argv, numberOfArguments + 1);
			upToSpace = strtok(NULL, " ");
		}

		argv[numberOfArguments] = NULL;

		bool enteredCd = false;

		// If user enters cd and the '>' character is not present
		if (strcmp(argv[0], "cd") == 0 && (greaterThanSymbol == NULL)) {
			if (numberOfArguments > 1) {
				DIR* dir = opendir(argv[1]);

				if (dir) {
					chdir(argv[1]);
				} else if (ENOENT == errno) {
					printf("cd: %s: No such file or directory\n", argv[1]);
				} else {
					printf("cd: %s: Permission denied\n", argv[1]);
				}

				enteredCd = true;
			} else {
				printf("File/Directory required.\n");
				enteredCd = true;
			}
		}

		child_pid = fork();

		if (child_pid == 0) {
			if(greaterThanSymbol != NULL) {
				int originalStdOutputLocation = dup(1);
				int file = open(out.filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);

				dup2(file, 1); // Change stdout location to file
				execvp(out.command, argv);
				dup2(originalStdOutputLocation, 1);
				close(originalStdOutputLocation);

				printf("Unknown command\n");
				fflush(stdin);
				exit(EXIT_SUCCESS);
			} else {
				execvp(argv[0], argv);

				if(enteredCd == false && greaterThanSymbol == NULL) {
					printf("Unknown command\n");
				}

				fflush(stdin);
				exit(EXIT_SUCCESS);
			}
		} else {
			wait(&childStats);
		}
	}
}
