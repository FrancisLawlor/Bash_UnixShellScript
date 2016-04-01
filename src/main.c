#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <dirent.h>
#include <errno.h>
#include <stdbool.h>
#include <fcntl.h>
#include "shell.h"

//function to handle ctrl+c
static void handlectrlc(int sig){
	signal(SIGINT, handlectrlc);
	printf("\n");
	printCurrentTime();
	printf(" # ");
	fflush(stdout);
}

int main(){
	//array of characters to store input.
	char input[1024];

	//while loop ensures shell continues to run after each input
	while(1){
		output o;
		
		//array used to store arguments for commands.
		char *argv[50];
		
		//double pointer used to store output once parsed.
		char **output = malloc(sizeof(char*));
		
		
		pid_t child_pid;
		int childstats;

		//detect ctrl+c input
		signal(SIGINT, handlectrlc);

		//print time and '#' as part of prompt
  		printCurrentTime();
		printf(" # ");
		
		fflush(stdin);
			
		//get input from user and break if input is ctrl+d
		if(!fgets(input, 1024, stdin)){
			break;
		}

		//remove \n from end of input
		input[strlen(input)-1]='\0';

		//Check if input has '>'
		char* checkpresence = strchr(input, '>');
		
		//if the '>' character is present, parse input and store in struct object.
		if(checkpresence!=NULL){
			o=parseInput(input);
		}

		int c=0;

		//split input string by spaces
		char * temp = strtok(input, " ");
		while(temp != NULL){
			output[c]=temp;
			c++;
			output=realloc(output, (c+1)*sizeof(char*));
			temp=strtok(NULL, " ");
		}
		
		output[c]=NULL;

		//assign contents of output array to argv
		int c2;
		for(c2=0;c2<c;c2++){
			argv[c2]=output[c2];
		}
	
		argv[c]=NULL;
		
		//boolean variable to track whether or not user has used the command "cd"
		bool cd=false;

		//if user enters cd and the '>' character is not present
		if(strcmp(argv[0],"cd")==0 && (checkpresence==NULL)){
			//if c>1 then cd has arguments else inform user that he/she must specify a directory
			if(c>1){
				DIR* d = opendir(argv[1]);
				
				//if directory exists, change directory.
				if(d){
					chdir(argv[1]);
				}
				else if (ENOENT == errno){ //if file/directory doesn't exist.
					printf("cd: %s: No such file or directory\n", argv[1]);
				}
				else{
					printf("cd: %s: Permission denied\n", argv[1]);
				}
				//change boolean value to true to signify that user entered "cd" as the first argument.
				cd=true;
			}
			else{
				printf("File/Directory required.\n");
				cd=true;
			}
		}
		
		//create child process

		child_pid=fork();

		if(child_pid==0){
			//if '>' operator is not present in input
			if(checkpresence!=NULL){
				int so=dup(1); //store current stdout location for returning after redirection
				int file=open(o.filename, O_WRONLY|O_CREAT|O_TRUNC, 0666); //open file with parsed filename
				dup2(file,1); //change stdout location to file
				execvp(o.phrase, argv);	//execute command and output to new stdout.
				dup2(so,1); //return to original stdout location.
				close(so);
				printf("Unknown command\n"); //if command doesn't exit output this statement.
				fflush(stdin);
				exit(EXIT_SUCCESS);
			}
			else{
				execvp(argv[0],argv);
				if(cd==false && (checkpresence==NULL)){
					printf("Unknown command\n");
				}
				fflush(stdin);
				exit(EXIT_SUCCESS);
			}
		}
		else {
			//wait for child process to finish
			wait(&childstats);
		}
	}
}

//Name: Francis Lawlor
//Student Number: 14461158
//Email: francis.lawlor@ucdconnect.ie
