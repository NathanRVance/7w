#include "7w.h"

int* data_getdiscards();
void data_deletediscard(int era, int card);
void data_build(int p, int card);
void data_setspecialflag(int p);
int data_numplayers();
int data_turnnum();
int print_cards(int x, int y, int *cards, int cursor);
int io_printtext(int xorigin, int y, int width, char* text);
void io_printborder(int x, int y, int width);
int io_getkey();
int io_printcard(int x, int y, int wonder, int stage, int player);
void posthelp();
int data_isai(int p);
void data_setfreebuild(int p);
int view_refresh(int focus, int cursor, int player, int mode);

void Halicarnassus(int player)
{
 int x = 61;
 int y = 0;
 int focus = data_numplayers();
 int i;
 int cursor = 0;
 int *discards = data_getdiscards();
 int numcards = 0;
 int bottom = 0;
 while(discards[numcards++] != -1 && numcards < 50);
 numcards /= 2;
 while(1) {
  view_refresh(focus, cursor, player, 1); 
  switch(io_getkey()) {
   case UP: cursor--;
    break;
   case DOWN: cursor++;
    break;
      case RIGHT: focus++;
    cursor = 0;
    break;
   case LEFT: focus--;
    cursor = 0;
    break;
  case '\t': focus = (focus+1)%(data_numplayers()+1);
    cursor = 0;
    break;
  case 'h': posthelp();
    break;
  case ENTER:
   if(data_hasbuiltname(player, discards[cursor*2], discards[cursor*2+1])) {
    postmessage("Cannot have two of the same card!");
   }
   else {
    data_freebuild(player, discards[cursor*2], discards[cursor*2+1]);
    data_deletediscard(discards[cursor*2], discards[cursor*2+1]);
    return;
   }
  default: break;
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

void special_action(int player, int wonder, int stage)
{
 if(wonder == 8) { //Halicarnassus
  if(stage == 2 || stage == 4 || stage == 5 || stage == 6)
   if(data_isai(player));
   else Halicarnassus(player);
 }
 if(wonder == 7) { //Olympia
  if(stage == 2) {
   data_setfreebuild(player);
  }
 }
}
