#include "7w.h"

int* cards_getcost(int wonder, int stage); //remember the stage offset!
int* cards_getproduction(int wonder, int stage);
char* cards_getname(int wonder, int stage); //stage should always be 0
char* cards_specialmessage(int wonder, int stage);
int data_getwonder(int p);
int data_getwonderside(int p);
void io_printborder(int x, int y);
int io_printtext(int xorigin, int y, int width, char* text);
void io_printcard(int x, int y, int wonder, int stage);
int data_getwonderstages(int p);
char* cat(char a[], char b[]);
char* itoa(int i);
int io_getkey();
void io_clearscreen();

int wonder_hasstage(int wonder, int side, int stage)
{
 if(side == 0 && stage > 2 || stage > 3) return 0;
 int *cost = cards_getcost(wonder, stage+1+side*3);
 int i;
 int has = 0;
 for(i = 0; i < NUMPRODUCTS; i++)
  if(cost[i]) has = 1;
 return has;
}

void print_wonder(int x, int y, int player, int cursor)
{
 io_printborder(x, y++);
 y = io_printtext(x, y, 29, cards_getname(data_getwonder(player), 0));
 int i;
 for(i = 0; wonder_hasstage(data_getwonder(player), data_getwonderside(player), i); i++) {
  char *text = cat("Stage ", itoa(i+1));
  if(data_getwonderstages(player) > i)
   text = cat(text, " (complete)");
  else
   text = cat(text, "           ");
  if(cursor == i)
   text = cat(text, " *");
  else
   text = cat(text, "  ");
  y = io_printtext(x, y, 29, text);
 }
 if(wonder_hasstage(data_getwonder(player), data_getwonderside(player), cursor))
  io_printcard(x, y, data_getwonder(player), cursor+1+3*data_getwonderside(player));
}

void wonder_selected(int player)
{
 int cursor = 0;
 while(1) {
  io_clearscreen();
  print_wonder(0, 0, player, cursor);
  switch(io_getkey()) {
   case UP: cursor--;
    break;
   case DOWN: cursor++;
    break;
   case ENTER: data_distributewonders(4);
   default: break;
  }
  if(cursor < 0) cursor = 0;
  if(! wonder_hasstage(data_getwonder(player), data_getwonderside(player), cursor))
   cursor--;
 }
}
