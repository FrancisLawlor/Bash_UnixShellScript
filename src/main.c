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

//function to handle ctrl+c
static void handleCtrlC(int sig) {
	signal(SIGINT, handleCtrlC);
	printf("\n");
	printCurrentTime();
	printf(" # ");
	fflush(stdout);
}

int main(void) {
	//array of characters to store input.
	char input[1024];

	//while loop ensures shell continues to run after each input
	while(1) {
		output out;
		
		//array used to store arguments for commands.
		char* argv[50];
		
		//double pointer used to store output once parsed.
		char** output = malloc(sizeof(char*));
		
		pid_t child_pid;
		int childStats;

		//detect ctrl+c input
		signal(SIGINT, handleCtrlC);

		//print time and '#' as part of prompt
  		printCurrentTime();
		printf(" # ");
		
		fflush(stdin);
			
		//get input from user and break if input is ctrl+d
		if (!fgets(input, 1024, stdin)) {
			break;
		}

		//remove \n from end of input
		input[strlen(input) - 1] = '\0';

		//Check if input has '>'
		char* greaterThanSymbol = strchr(input, '>');
		
		//if the '>' character is present, parse input and store in struct object.
		if (greaterThanSymbol != NULL) {
			out = parseInput(input);
		}

		int i = 0;

		//split input string by spaces
		char* upToSpace = strtok(input, " ");
		while (upToSpace != NULL){
			output[i] = upToSpace;
			i++;
			output = realloc(output, (i + 1) * sizeof(char*));
			upToSpace = strtok(NULL, " ");
		}
		
		output[i] = NULL;

		//assign contents of output array to argv
		int j;

		for (j = 0; j < i; j++){
			argv[j] = output[j];
		}
	
		argv[i] = NULL;
		
		//boolean variable to track whether or not user has used the command "cd"
		bool cd = false;

		//if user enters cd and the '>' character is not present
		if (strcmp(argv[0], "cd") == 0 && (greaterThanSymbol == NULL)) {
			//if i>1 then cd has arguments, else inform user that he/she must specify a directory
			if (i > 1) {
				DIR* dir = opendir(argv[1]);
				
				//if directory exists, change directory.
				if (dir) {
					chdir(argv[1]);
				} else if (ENOENT == errno) { //if file/directory doesn't exist.
					printf("cd: %s: No such file or directory\n", argv[1]);
				} else {
					printf("cd: %s: Permission denied\n", argv[1]);
				}

				//change boolean value to true to signify that user entered "cd" as the first argument.
				cd = true;
			} else {
				printf("File/Directory required.\n");
				cd = true;
			}
		}
		
		//create child process

		child_pid = fork();

		if (child_pid == 0) {
			//if '>' operator is present in input
			if(greaterThanSymbol != NULL) {
				int stdo = dup(1); //store current stdout location for returning after redirection
				int file = open(out.filename, O_WRONLY | O_CREAT | O_TRUNC, 0666); //open file with parsed filename
				dup2(file, 1); //change stdout location to file
				execvp(out.command, argv);	//execute command and output to new stdout.
				dup2(stdo, 1); //return to original stdout location.
				close(stdo);
				printf("Unknown command\n"); //if command doesn't exit output this statement.
				fflush(stdin);
				exit(EXIT_SUCCESS);
			} else {
				execvp(argv[0], argv);

				if(cd == false && greaterThanSymbol == NULL) {
					printf("Unknown command\n");
				}

				fflush(stdin);
				exit(EXIT_SUCCESS);
			}
		} else {
			//wait for child process to finish
			wait(&childStats);
		}
	}
}

//Name: Francis Lawlor
//Student Number: 14461158
//Email: francis.lawlor@ucdconnect.ie
