#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "shell.h"

RedirectData parseRedirectInput(char* input) {
	RedirectData redirectData;
	char** output = malloc(sizeof(char*));

	output[0] = strtok(input, ">");
	output[1] = strtok(NULL, "\0");

	redirectData.command = (char*) malloc(strlen(output[0]));
	strcpy(redirectData.command, output[0]);

	redirectData.fileName = (char*) malloc(strlen(output[1]));
	strcpy(redirectData.fileName, output[1]);
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

bool isEmptyString(char* input) {
	return (strlen(input) == 1);
}
