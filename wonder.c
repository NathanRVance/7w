#include "7w.h"

int* cards_getcost(int wonder, int stage); //remember the stage offset!
int* cards_getproduction(int wonder, int stage);
char* cards_getname(int wonder, int stage); //stage should always be 0
char* cards_specialmessage(int wonder, int stage);
char* getname(int res);
int cards_gettype(int wonder, int stage); //stage is 0
int data_getwonder(int p);
int data_getwonderside(int p);
void io_printborder(int x, int y);
int io_printtext(int xorigin, int y, int width, char* text);
int io_printcard(int x, int y, int wonder, int stage);
void io_printname(int x, int y, int era, int card);
int data_getwonderstages(int p);
char* cat(char a[], char b[]);
char* itoa(int i);
int io_getkey();
void io_clearscreen();
int* data_getbuilt(int p);
void io_printplain(int x, int y, char *s);

int wonder_numstages(int player)
{
 int side = data_getwonderside(player);
 int wonder = data_getwonder(player);
 if(side == 0) return 3;
 if(wonder == 9) return 4;
 if(wonder == 3) return 2;
 return 3;
}

int print_wonder(int x, int y, int player, int cursor)
{
// if(cursor != -1) cursor = cursor % wonder_numstages(player);
 io_printborder(x, y++);
 y = io_printtext(x, y, 29, cards_getname(data_getwonder(player), 0));
 y = io_printtext(x, y, 29, cat("Produces 1 ", getname(cards_gettype(data_getwonder(player), 0))));
 int i;
 for(i = 0; i < wonder_numstages(player); i++) {
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
 io_printborder(x, y++);

 //Print what has been built
 int *built = data_getbuilt(player);
 for(i = 0; built[i] != -1; i+=2) {
  io_printname(x, y++, built[i], built[i+1]);
 }
 if(i == 0) y--;

 //Info about component
 if(cursor >= 0 && cursor < wonder_numstages(player))
 return io_printcard(x, y, data_getwonder(player), cursor+1+3*data_getwonderside(player));
}

//dir is 0 for none, 1 for east, 2 for west
int print_wondersmall(int x, int y, int player, int selected, int dir)
{
 io_printborder(x, y++);
 if(selected) io_printplain(28, y, "*");
 if(dir == 1) io_printplain(29, y, "East");
 if(dir == 2) io_printplain(29, y, "West");
 y = io_printtext(x, y, 29, cards_getname(data_getwonder(player), 0));
 io_printborder(x, y);
 return y;
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
  if(cursor >= wonder_numstages(player))
   cursor--;
 }
}
