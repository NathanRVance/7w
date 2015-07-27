#include <stdlib.h>
#include <stdio.h>

#define LOGFILE	"7w.log"     // all Log(); messages will be appended to this file

int firstOpen = 1;

void Log(char *message) {
	FILE *file;

	if(firstOpen)
		file = fopen(LOGFILE, "w");
	else
		file = fopen(LOGFILE, "a+");
	firstOpen = 0;
	fprintf(file, "%s\n", message); /*writes*/
	fclose(file); /*done!*/
}
