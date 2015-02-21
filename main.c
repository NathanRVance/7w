#include "7w.h"
#include <stdlib.h>

void tester();
void io_init();
void cards_init();
void io_printcard(int x, int y, int era, int card);
void io_clearscreen();
int* getdeck(int era, int numplayers);
void data_init(int num_players);
void player_turn(int player);
void wonder_selected(int player);
void view_refresh(int focus, int cursor, int player);
int data_numplayers();

void halt()
{
 endwin();
 exit(0);
}

main()
{
 io_init();
 cards_init();
 data_init(7);
// tester();
// wonder_selected(0);
 player_turn(0);
 halt();
}

void tester()
{
 int focus, cursor, player;
 focus = cursor = player = 0;
 int numplayers = data_numplayers();
 while(1) {
  view_refresh(focus, cursor, player);
  switch(io_getkey()) {
   case DOWN: cursor++;
    break;
   case UP: if(cursor > 0) cursor--;
    break;
   case LEFT: if(focus > 0) focus--;
    break;
   case RIGHT: if(focus < numplayers) focus++;
    break;
   case ENTER: player = (player+1)%numplayers;
    break;
   case 'q': halt();
    break;
   default: break;
  }
 }
}
