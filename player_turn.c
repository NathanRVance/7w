#include "7w.h"

int* data_gethand(int p);
int data_getera();
void io_printhand(int x, int y, int player, int cursor);
void io_printcard(int x, int y, int era, int card);
int io_getkey();
void data_passturn();
void io_clearscreen();
void data_nextera();

void player_turn(int player)
{
 int *hand = data_gethand(player);
 int numcards;
 for(numcards = 0; numcards < 7 && hand[numcards] > -1; numcards++);
 int cursor = 0; 
 while(1) {
  io_clearscreen();
  io_printhand(0, 0, player, cursor);
  io_printcard(0, 8, data_getera(), hand[cursor]);
  switch(io_getkey()) {
   case UP: cursor--;
    break;
   case DOWN: cursor++;
    break;
   case RIGHT: data_passturn();
    break;
   case ENTER: data_nextera();
    break;
   default: break;
  }
  if(cursor < 0) cursor = numcards-1;
  if(cursor >= numcards) cursor = 0;
 }
}
