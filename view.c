#include "7w.h"

int print_wonder(int x, int y, int player, int cursor);
int print_wondersmall(int x, int y, int player, int selected, int dir);
int io_printhand(int x, int y, int player, int cursor);
void io_clearscreen();
int data_numplayers();
void printmessage(int x, int y, int width);
int data_geteast(int p);
int data_getwest(int p);

void view_printwonders(int focus, int cursor, int player)
{
 int num = data_numplayers();
 int x, y, i, dir, p;
 x = y = 0;
 for(i = 0; i < num; i++) {
  p = (player+i)%num;
  dir = 0;
  if(p == data_geteast(player)) dir = 1;
  if(p == data_getwest(player)) dir = 2;
  if(focus == p)
   print_wonder(34, 0, p, cursor);
  y = print_wondersmall(x, y, p, focus == p, dir);
 }
 printmessage(0, y+2, 35);
}

int view_refresh(int focus, int cursor, int player)
{
 int ret;
 io_clearscreen();
 if(focus == data_numplayers()) {
  view_printwonders(player, -1, player);
  ret = io_printhand(61, 0, player, cursor);
 } else {
  view_printwonders((focus+player)%data_numplayers(), cursor, player);
  ret = io_printhand(61, 0, player, -1);
 }
 return ret;
}
