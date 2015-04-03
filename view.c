#include "7w.h"

int* print_wonder(int x, int y, int player, int cursor);
int print_wondersmall(int x, int y, int player, int selected, int dir);
int io_printhand(int x, int y, int player, int cursor, int mode);
int io_printcard(int x, int y, int wonder, int stage, int player);
void io_clearscreen();
int data_numplayers();
void printmessage(int x, int y, int width);
int data_geteast(int p);
int data_getwest(int p);
int write_history(int player, int x, int y);

int* view_printwonders(int focus, int cursor, int player)
{
 int *ret;
 int num = data_numplayers();
 int x, y, i, dir, p;
 x = y = 0;
 for(i = 0; i < num; i++) {
  p = (player+i)%num;
  dir = 0;
  if(p == data_geteast(player)) dir = 1;
  if(p == data_getwest(player)) dir = 2;
  if(focus == p)
   ret = print_wonder(34, 0, p, cursor);
  y = print_wondersmall(x, y, p, focus == p, dir);
 }
 y = write_history(focus, 0, y);
 printmessage(0, y+2, 35);
 return ret;
}

int view_refresh(int focus, int cursor, int player, int mode) //mode 0 is normal, 1 is discard search
{
 int ret;
 int *card;
 io_clearscreen();
 if(focus == data_numplayers()) {
  card = view_printwonders(player, -1, player);
  ret = io_printhand(61, 0, player, cursor, mode);
 } else {
  card = view_printwonders((focus+player)%data_numplayers(), cursor, player);
  ret = io_printhand(61, 0, player, -1, mode);
 }
 if(card[0] != -1)
  ret = io_printcard(61, ret-1, card[0], card[1], player);
 return ret;
}
