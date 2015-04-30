#include "7w.h"
#include <stdlib.h>
#include <stdio.h>

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
void data_setai(int p);
int data_isai(int p);

void halt()
{
 endwin();
 exit(0);
}

void haltError(char *message, int num)
{
 endwin();
 printf("Something has gone wrong, very wrong.\n");
 printf(message);
 printf("\n");
 printf("Number associated with this error: %d\n", num);
 exit(1);
}

static int ais[7] = {0, 1, 1, 1, 1, 1, 1};

main_routine()
{
 int player = 0;
 while(1) {
  if(data_isai(player)) ai_turn(player);
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
 data_init(4);
 int i;
 for(i = 0; i < 7; i++) {
  if(ais[i]) data_setai(i);
 }
 main_routine();
 halt();
}
