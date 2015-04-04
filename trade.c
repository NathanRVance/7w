#include "7w.h"
#include <stdio.h>

int data_hasbuilt(int p, int era, int card);
int data_haswonderstage(int p, int wonder, int stage);
int* data_gettradables(int p);
void data_addgold(int amnt, int p);
int data_getgold(int p);
int data_geteast(int p);
int data_getwest(int p);
int data_numplayers();
int data_numcards(int p);
int io_getkey();
void io_printborder(int x, int y, int width);
int io_printtext(int xorigin, int y, int width, char* text);
void io_printplain(int x, int y, char *s);
void io_printcolor(int x, int y, int color, char *s);
char* getname(int res);
char* itoa(int i);

static int tradebuffer[3][GOLD+1];

//0 is east, 1 is west
int get_trade(int player, int type, int direction) {
 if(type <= ORE) { //resource
  if(data_haswonderstage(player, 7, 4)) return 1;
  if(direction == 0) { //east
   if(data_hasbuilt(player, 0, 18)) return 1;
  } else { //west
   if(data_hasbuilt(player, 0, 19)) return 1;
  }
 } else { //industrial product
  if(data_hasbuilt(player, 0, 20)) return 1;
 }
 return 2;
}

int* trade_buffer()
{
 return tradebuffer[2];
}

void trade_clear(int player)
{
 data_addgold(tradebuffer[0][GOLD]*-1, data_geteast(player));
 data_addgold(tradebuffer[1][GOLD]*-1, data_getwest(player));
 data_addgold(tradebuffer[2][GOLD]*-1, player);
 int i, j;
 for(i = 0; i < 3; i++)
  for(j = 0; j < GOLD+1; j++)
   tradebuffer[i][j] = 0;
}

void trade_set(int player, int trade[3][GOLD])
{
 int i, j;
 for(i = 0; i < 3; i++) {
  for(j = 0; j < GOLD; j++) {
   tradebuffer[i][j] = trade[i][j];
  }
  tradebuffer[i][GOLD] = 0; //we deal with gold in the method that calls this
 }
}

//gold amounts relative to player
void trade_addgold(int player, int amnt, int direction)
{
 int p;
 if(direction == 0) p = data_geteast(player);
 if(direction == 1) p = data_getwest(player);
 data_addgold(amnt, player);
 tradebuffer[2][GOLD] += amnt;
 data_addgold(amnt*-1, p);
 tradebuffer[direction][GOLD] -= amnt;
}

void trade_commit(int player)
{
 int i;
 for(i = 0; i < 3; i++)
  tradebuffer[i][GOLD] = 0;
 trade_clear(player);
}

int* trade_gettradables(int player, int direction)
{
 int i;
 int *ret;
 if(direction) ret = data_gettradables(data_getwest(player));
 else ret = data_gettradables(data_geteast(player));
 for(i = 0; i < GOLD; i++)
  ret[i] += tradebuffer[direction][i];
 return ret;
}

void trade_print(int x, int y, int player, int cursorx, int cursory)
{
 char s[40];
 int *east = trade_gettradables(player, 0);
 int *west = trade_gettradables(player, 1);
 int width = 28;
 io_printborder(x, y++, width);
 y = io_printtext(x, y, width, "West:      | East:");
 sprintf(s, "rate:   %d  | rate:   %d", get_trade(player, cursory, 1), get_trade(player, cursory, 0));
 y = io_printtext(x, y, width, s);
 int i;
 for(i = 0; i < GOLD+1; i++) {
  sprintf(s, "%-7s%2d %c| %-7s %d %c", getname(i), west[i], (cursorx == 1 && cursory == i)? '*' : ' ', getname(i), east[i], (cursorx == 0 && cursory == i)? '*' : ' ');
  y = io_printtext(x, y, width, s);
  if(tradebuffer[1][i])
   io_printcolor(x+10, y-1, 31, itoa(west[i]));
  if(tradebuffer[0][i])
   io_printcolor(x+23, y-1, 31, itoa(east[i]));
 }
 sprintf(s, "Your gold: %d", data_getgold(player));
 y = io_printtext(x, y, width, s);
 io_printborder(x, y++, width);
}

int trade_change(int cursorx, int cursory, int player, int change)
{
 int forced = 0;
 int *tradee = trade_gettradables(player, cursorx);
 if(tradebuffer[2][cursory]-1 < 0) {
  change = 1;
  forced = 1;
 }
 if(tradee[cursory]-1 < 0 || data_getgold(player) < get_trade(player, cursory, cursorx)) {
  if(forced) return 1;
  change = -1;
 }

 tradebuffer[cursorx][cursory] -= change;
 tradebuffer[2][cursory] += change;
 trade_addgold(player, get_trade(player, cursory, cursorx)*change*-1, cursorx);

 if(tradebuffer[2][cursory] == 0 || data_getgold(player) == 0) return change*-1;
 return change;
}

int trade_routine(int x, int y, int player)
{
 int cursorx, cursory, change;
 cursorx = cursory = 0;
 change = 1;
 while(1) {
  trade_print(x, y, player, cursorx, cursory);
  switch(io_getkey()) {
   case LEFT:
   case RIGHT: cursorx = !cursorx;
    change = 1;
    break;
   case UP: cursory--;
    change = 1;
    break;
   case DOWN: cursory++;
    change = 1;
    break;
   case ENTER: change = trade_change(cursorx, cursory, player, change);
    break;
   default: break;
  }
  if(cursory < 0) return 0;
  if(cursory >= GOLD) return 1;
 }
}
