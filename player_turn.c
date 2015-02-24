#include "7w.h"

int* data_gethand(int p);
int data_getera();
int io_getkey();
void data_endturn();
int data_numplayers();
void data_build(int p, int card);
void data_discard(int p, int card);
void data_buildwonder(int p, int card);
void data_addgold(int amnt, int p);
void view_refresh(int focus, int cursor, int player);
int wonder_numstages(int player);
int* cards_getcost(int era, int card);
int data_numbuilt(int p);
void postmessage(char* message);
void posthelp();
void clearmessage();
int postoptions(int x, int y);

int player_build(int focus, int cursor, int player)
{
 int *hand = data_gethand(player);
 if(focus == data_numplayers()) {
  if(hand[cursor] == -1) return 0;
  int choice = postoptions(65, 20);
  if(choice == 0) {
   if(data_canafford(player, cards_getcost(data_getera(), hand[cursor]))) {
    data_build(player, hand[cursor]);
    return 1;
   }
   else postmessage("Can't afford this!");
  } else if(choice == 1) {
   data_discard(player, hand[cursor]);
   data_addgold(3, player);
   return 1;
  } else if(choice == 2) {
   if(data_canafford(player, cards_getcost(data_getwonder(player), data_getwonderside(player)*3+1+cursor))) {
    data_buildwonder(player, hand[cursor]);
    return 1;
   }
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
    if(player_build(focus, cursor, player)) {
        player++;
        cursor = 0;
        focus = data_numplayers();
        clearmessage();
       }
    break;
   case '\t': focus = (focus+1)%(data_numplayers()+1);
    break;
   case 'h': posthelp();
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
