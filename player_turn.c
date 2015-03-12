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
int data_numcards(int p);
int data_canafford(int p, int era, int card);
int data_getwonder(int p);
int data_getnextwonderstage(int p);
int view_refresh(int focus, int cursor, int player);
int wonder_numstages(int player);
int* cards_getcost(int era, int card);
int data_numbuilt(int p);
void postmessage(char* message);
void posthelp();
void clearmessage();
int postoptions(int x, int y);
int trade_routine(int x, int y, int player);
void trade_commit(int player);

int player_build(int focus, int cursor, int player)
{
 int *hand = data_gethand(player);
 if(focus == data_numplayers()) {
  if(hand[cursor] == -1) return 0;
  int choice = postoptions(62, 20);
  if(choice == 0) {
   if(data_canafford(player, data_getera(), hand[cursor])) {
    data_build(player, hand[cursor]);
    return 1;
   }
   else postmessage("Can't afford this!");
  } else if(choice == 1) {
   data_discard(player, hand[cursor]);
   data_addgold(3, player);
   return 1;
  } else if(choice == 2) {
   if(data_canafford(player, data_getwonder(player), data_getnextwonderstage(player))) {
    data_buildwonder(player, hand[cursor]);
    return 1;
   }
   else postmessage("Can't afford this!");
  }
 }
 return 0;
}

void player_turn(int player)
{
 int numcards, tradey;
 int cursor = 0; 
 int focus = data_numplayers();
 while(1) {
  numcards = data_numcards(player)+1; //include trade option in hand
  tradey = view_refresh(focus, cursor, player);
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
     trade_commit(player);
     clearmessage();
     return;
    }
    break;
   case '\t': focus = (focus+1)%(data_numplayers()+1);
    break;
   case 'h': posthelp();
    break;
   default: break;
  }
  if(focus < 0) focus = data_numplayers();
  focus = focus%(data_numplayers()+1);
  if(focus == data_numplayers()) {
   if(cursor < 0) cursor = numcards-2;
   if(cursor >= numcards-1) {
    if(trade_routine(61, tradey, player)) cursor = 0;
    else cursor = numcards-2;
   }
  }
  else {
   if(cursor < 0) cursor = wonder_numstages((player+focus)%data_numplayers())+data_numbuilt(player+focus%data_numplayers())-1;
   cursor = cursor % (wonder_numstages((player+focus)%data_numplayers())+data_numbuilt(player+focus%data_numplayers()));
  }
 }
}
