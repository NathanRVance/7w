#include "7w.h"

int* data_gethand(int p);
int data_getera();
void io_printhand(int x, int y, int player, int cursor);
void io_printcard(int x, int y, int era, int card);
int io_getkey();
void data_passturn();
void data_nextera();
int data_numplayers();
void view_refresh(int focus, int cursor, int player);
int wonder_numstages(int player);

void player_turn(int player)
{
 int *hand = data_gethand(player);
 int numcards;
 for(numcards = 0; numcards < 7 && hand[numcards] > -1; numcards++);
 int cursor = 0; 
 int focus = data_numplayers();
 while(1) {
  view_refresh(focus, cursor, player);
  switch(io_getkey()) {
   case UP: cursor--;
    break;
   case DOWN: cursor++;
    break;
   case RIGHT: data_passturn();
    break;
   case LEFT: player = (player+1)%data_numplayers();
    break;
   case ENTER: data_nextera();
    break;
   case '\t': focus = (focus+1)%(data_numplayers()+1);
    break;
   default: break;
  }
  if(focus == data_numplayers()) {
   if(cursor < 0) cursor = numcards-1;
   if(cursor >= numcards) cursor = 0;
  }
  else {
   if(cursor < 0) cursor = wonder_numstages((player+focus)%data_numplayers())-1;
   cursor = cursor % wonder_numstages((player+focus)%data_numplayers());
  }
 }
}
