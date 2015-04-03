#include "7w.h"
#include <stdio.h>

void io_printborder(int x, int y, int width);
int io_printtext(int xorigin, int y, int width, char* text);
void io_printname(int x, int y, int era, int card);

static int history[7][6]; //goes card era, card, purchase type, trade left, trade right, extra stuff?
static int ready;

int write_history(int player, int x, int y)
{
 if(! ready) return y;
 int width = 28;
 io_printborder(x, y++, width);
 y = io_printtext(x, y, width, "Activity last turn:");
 if(history[player][2] == 0) { // Built card
  y = io_printtext(x, y, width, " Purchased");
  io_printname(x, y++, history[player][0], history[player][1]);
 } else if(history[player][2] == 1) { // Sold card
  y = io_printtext(x, y, width, " Sold a card");
 } else { // Built wonder
  y = io_printtext(x, y, width, " Built a wonder stage");
 }
 y = io_printtext(x, y, width, "Trade activity:");
 if(history[player][3] || history[player][4]) {
  char s[30];
  if(history[player][3]) {
   sprintf(s, " Traded %d gold to the east", history[player][3]);
   y = io_printtext(x, y, width, s);
  }
  if(history[player][4]) {
   sprintf(s, " Traded %d gold to the west", history[player][4]);
   y = io_printtext(x, y, width, s);
  }
 } else {
  y = io_printtext(x, y, width, " none");
 }
 io_printborder(x, y++, width);
 return y;
}

void clear_history()
{
 ready = 0;
 int i, j;
 for(i = 0; i < 7; i++)
  for(j = 0; j < 6; j++)
   history[i][j] = 0;
}

void write_purchase(int player, int era, int card, int type)
{
 ready = 1;
 history[player][0] = era;
 history[player][1] = card;
 history[player][2] = type;
 //type 0 is build, 1 is sell, 2 is wonder
}

void write_trade(int player, int tradel, int trader)
{
 ready = 1;
 history[player][3] = tradel;
 history[player][4] = trader;
}
