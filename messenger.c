#include "7w.h"

int io_printtext(int xorigin, int y, int width, char* text);
void io_printborder(int x, int y, int width);

#define MAXMESSAGE 100
static char message[MAXMESSAGE];

void printmessage(int x, int y, int width)
{
 if(message[0] == '\0') return;
 io_printborder(x, y++, width);
 y = io_printtext(x, y, width, message);
 io_printborder(x, y, width);
}

void postmessage(char* m)
{
 int i;
 for(i = 0; i < MAXMESSAGE && (message[i] = m[i]) != '\0'; i++);
}

void clearmessage()
{
 message[0] = '\0';
}

void posthelp()
{
 postmessage("    7 Wonders Help:\nKeys:\n Arrow keys move cursor\n Return - buys resource\n h - prints this message");
}
