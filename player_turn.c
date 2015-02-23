#include "7w.h"

int* data_gethand(int p);
int data_getera();
void io_printhand(int x, int y, int player, int cursor);
void io_printcard(int x, int y, int era, int card);
int io_getkey();
void data_endturn();
void data_nextera();
int data_numplayers();
void data_build(int p, int card);
void data_buildwonder(int p, int card);
void view_refresh(int focus, int cursor, int player);
int wonder_numstages(int player);
int* cards_getcost(int era, int card);
int data_numbuilt(int p);

int player_build(int focus, int cursor, int player)
{
 if(focus == data_numplayers()) {
  int *hand = data_gethand(player);
  if(data_canafford(player, cards_getcost(data_getera(), hand[cursor])) == 2) {
   data_build(player, hand[cursor]);
   return 1;
  }
 }
 if(focus == 0) {
  if(data_canafford(player, cards_getcost(data_getwonder(player), data_getwonderside(player)*3+1+cursor)) == 2) {
   data_buildwonder(player, 0); //change to choose card used
   return 1;
  }
 }
 return 0;
}

void player_turn(int player)
{
 int numcards, *hand;
 int cursor = 0; 
 int focus = data_numplayers();
 while(1) {
  hand = data_gethand(player);
  for(numcards = 0; numcards < 7 && hand[numcards] > -1; numcards++);
  view_refresh(focus, cursor, player);
  switch(io_getkey()) {
   case UP: cursor--;
    break;
   case DOWN: cursor++;
    break;
   case RIGHT: focus++;
    break;
   case LEFT: focus--;
    break;
   case ENTER:
    if(player_build(focus, cursor, player))
     player++;
    break;
   case '\t': focus = (focus+1)%(data_numplayers()+1);
    break;
   default: break;
  }
  if(player >= data_numplayers()) {
   data_endturn();
   player = 0;
  }
  if(focus < 0) focus = data_numplayers();
  focus = focus%(data_numplayers()+1);
  if(focus == data_numplayers()) {
   if(cursor < 0) cursor = numcards-1;
   if(cursor >= numcards) cursor = 0;
  }
  else {
   if(cursor < 0) cursor = wonder_numstages((player+focus)%data_numplayers())+data_numbuilt(player+focus%data_numplayers())-1;
   cursor = cursor % (wonder_numstages((player+focus)%data_numplayers())+data_numbuilt(player+focus%data_numplayers()));
  }
 }
}
