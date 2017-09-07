#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "shell.h"

output parseInput(char* input) {
	output redirectData;
	char output[2];

	output[0] = strtok(input, ">");
	output[1] = strtok(NULL, "/0");

	redirectData.command = (char*) malloc(strlen(output[0]));
	strcpy(redirectData.command, output[0]);

	redirectData.filename = (char*) malloc(strlen(output[1]));
	strcpy(redirectData.filename, output[1]);

	return redirectData;
}

void printCurrentTime() {
	time_t currentTime;
	time(&currentTime);

	struct tm* data;
	char timeString[50];

	data = localtime(&currentTime);

	strftime(timeString, 50, "%d/%m %H:%M", data);

	printf("[%s]", timeString);
}
