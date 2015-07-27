#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define POOLSIZE 100000

static char cpool[POOLSIZE];
static char *cpoolp = cpool;

char* get_chararray(int size) {
	if (cpoolp + size > cpool + POOLSIZE)
		cpoolp = cpool;
	char* ret = cpoolp;
	cpoolp += size;
	return ret;
}

/* Concatinates a and b and returns a different
 string. Only use when returned string can be recycled. */
char* cat(char a[], char b[]) {
	int len = strlen(a) + strlen(b) + 1;
	/* if(len > RET) return "error";
	 if(len + retindex > RET) retindex = 0;
	 char* p = ret + retindex;
	 */
	char *p = get_chararray(len);
	int i, j;
	i = j = 0;
	while ((p[i++] = a[j++]) != '\0')
		;
	i--;
	j = 0;
	while ((p[i++] = b[j++]) != '\0')
		;
	return p;
}

char* itoa(int i) {
	if(i == 0) return "0";
	char *num = get_chararray(5);
	int j;
	for (j = 0; j < 5; num[j++] = ' ')
		;
	num[3] = '0';
	int o = 3;
	int negative = 0;
	if (i < 0) {
		negative = 1;
		i *= -1;
	}
	while (o >= 0 && i > 0) {
		num[o--] = i % 10 + '0';
		i /= 10;
	}
	if (o >= 0 && negative)
		num[o] = '-';
	num[4] = '\0';
	for (i = 0; num[i] == ' '; i++)
		;
	return &num[i];
}

//sleep in tenths of a second
int bettersleep(int ds) {
	struct timespec tim, tim2;
	tim.tv_sec = ds / 10;
	tim.tv_nsec = (ds % 10) * 100000000L;
	return nanosleep(&tim, &tim2);
}
