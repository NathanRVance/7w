#include "7w.h"
#include <stdlib.h>

void tester();
void io_init();
void cards_init();
void io_clearscreen();
int* getdeck(int era, int numplayers);
void data_init(int num_players);
void player_turn(int player);
void ai_turn(int player);
void wonder_selected(int player);
void view_refresh(int focus, int cursor, int player);
int data_numplayers();
void data_endturn();
int data_getera();

void halt()
{
 endwin();
 exit(0);
}

static int ais[7] = {0, 1, 1, 1, 1, 1, 1};

main_routine()
{
 int player = 0;
 while(1) {
  if(ais[player]) ai_turn(player);
  else player_turn(player);
  if(++player == data_numplayers()) {
   data_endturn();
   player = 0;
  }
 }
}

main()
{
 io_init();
 cards_init();
 data_init(7);
 main_routine();
 halt();
}
