#include "7w.h"

void print_wonder(int x, int y, int player, int cursor);
void print_wondersmall(int x, int y, int player);
void io_printcard(int x, int y, int era, int card);
void io_printhand(int x, int y, int player, int cursor);
void io_clearscreen();
int data_numplayers();

void view_refresh(int focus, int cursor, int player)
{
 io_clearscreen();
 int num = data_numplayers();
 int x, y, i;
 int offset = 0;
 if(num == 3) x = 0;
 if(num == 4 || num == 5) x = 1;
 if(num == 6 || num == 7) x = 2, offset = -1;
 y = 4;
 int change = -1;
 for(i = 0; i < num; i++) {
  int p = (player+i+offset)%num;
  if(p < 0) p += num;
  if(focus == p)
   print_wonder(x*27, y, p, cursor);
  else print_wondersmall(x*27, y, p);
  if(x == 0) {
   if(change == -1) change = 0;
   else change = 1;
   y = 0;
  }
  x += change;
 }
}
