#include "7w.h"

int print_wonder(int x, int y, int player, int cursor);
int print_wondersmall(int x, int y, int player, int selected, int dir);
void io_printcard(int x, int y, int era, int card);
void io_printhand(int x, int y, int player, int cursor);
void io_clearscreen();
int data_numplayers();
void io_printhand(int x, int y, int player, int cursor);

void view_printwonders(int focus, int cursor, int player)
{
 int num = data_numplayers();
 int x, y, i, dir, p;
 x = y = 0;
 for(i = 0; i < num; i++) {
  p = (player+i)%num;
  dir = 0;
  if(i == 1) dir = 1;
  if(i == num-1) dir = 2;
  if(focus == p)
   print_wonder(34, 0, p, cursor);
  y = print_wondersmall(x, y, p, focus == p, dir);
 }
}

void view_refresh(int focus, int cursor, int player)
{
 io_clearscreen();
 if(focus == data_numplayers()) {
  view_printwonders(player, -1, player);
  io_printhand(61, 0, player, cursor);
 } else {
  view_printwonders((focus+player)%data_numplayers(), cursor, player);
  io_printhand(61, 0, player, -1);
 }
}
