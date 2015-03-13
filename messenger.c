#include "7w.h"

int io_getkey();
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
 postmessage("    7 Wonders Help:\nKeys:\n Arrow keys move cursor\n Return - buys card\n h - prints this message");
}

int postoptions(int x, int y)
{
 int width = 28;
 int yorig = y;
 int cursor = 0;
 char* a = "Buy Sell Wonder   Cancel";
 char* b = " *";
 char* c = "     *";
 char* d = "           *";
 char* e = "                    *";
 while(1) {
  y = yorig;
  io_printborder(x, y++, width);
  y = io_printtext(x, y, width, a);
  if(cursor == 0) y = io_printtext(x, y, width, b);
  if(cursor == 1) y = io_printtext(x, y, width, c);
  if(cursor == 2) y = io_printtext(x, y, width, d);
  if(cursor == 3) y = io_printtext(x, y, width, e);
  io_printborder(x, y++, width);
  switch(io_getkey()) {
   case LEFT: cursor--;
    break;
   case RIGHT: cursor++;
    break;
   case ENTER: return cursor;
    break;
   default: break;
  }
  if(cursor < 0) cursor = 3;
  cursor = cursor%4;
 }
}
