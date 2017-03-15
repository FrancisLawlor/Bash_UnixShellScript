#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "shell.h"

//function to parse the input string when the user includes the '>' character
output parseInput(char* input) {
	//struct to store and return data
	output out;

	int i = 0;

	char** output = malloc(sizeof(char*));

	//split input if '>' is present
	char* upToGreaterThan = strtok(input, ">");

	while (upToGreaterThan != NULL) {
		output[i] = upToGreaterThan;
		i++;
		output = realloc(output, (i + 1) * sizeof(char*));
		upToGreaterThan = strtok(NULL, "/0");
	}

	output[i] = NULL;
	
	//store command name in command field in struct
	out.command = (char*) malloc(strlen(output[0]));
	strcpy(out.command, output[0]);
	
	//store output file name in filename field of struct
	out.filename = (char*) malloc(strlen(output[1]));
	strcpy(out.filename, output[1]);

	return out;
}

//function to print current time
void printCurrentTime() {
	time_t currenttime;
	time(&currenttime);

	struct tm* data;
	char timestring[50];
	
	data = localtime(&currenttime);
	
	//assign time to string timestring with format dd/mm hh:mm
	strftime(timestring, 50, "%d/%m %H:%M", data);

	//print out contents of timestring
	printf("[%s]", timestring);
}

//Name: Francis Lawlor
//Student Number: 14461158
//Email: francis.lawlor@ucdconnect.ie


