#include "7w.h"
#include <stdio.h>

int io_getkey();
int data_numplayers();
int postyn(int x, int y, char *message);
void halt();
void io_clearscreen();
void io_printborder(int x, int y, int width);
int io_printtext(int xorigin, int y, int width, char* text);
int* print_wonder(int x, int y, int player, int cursor);
int print_wondersmall(int x, int y, int player, int selected, int dir);
int* data_getplayerdat(int p);
int* data_getbuilt(int p);
int science(int player);
int* get_special(int era, int card, int player);

int endgame_printscorebreakdown(int p)
{
 int* playerdat = data_getplayerdat(p);
 int gold = playerdat[3] / 3;
 int military = playerdat[4] - playerdat[5];
 int vps = playerdat[6];
 int special = 0;
 int *built = data_getbuilt(p);
 int i;
 for(i = 0; built[i] != -1; i += 2) {
  special += get_special(built[i], built[i+1], p)[0];
 }
 int sci = science(p); 
 int x = 61;
 int y = 0;
 int width = 28;
 io_printborder(x, y++, width);
 y = io_printtext(x, y, width, "    Score breakdown:");
 char s[40];
 y = io_printtext(x, y, width, "Category  | Score");
 sprintf(s, " Gold     | %d", gold);
 y = io_printtext(x, y, width, s);
 sprintf(s, " Military | %d", military);
 y = io_printtext(x, y, width, s);
 sprintf(s, " Cards    | %d", vps);
 y = io_printtext(x, y, width, s);
 sprintf(s, " Guilds   | %d", special);
 y = io_printtext(x, y, width, s);
 sprintf(s, " Science  | %d", sci);
 y = io_printtext(x, y, width, s);
 sprintf(s, " Total: %d", gold + military + vps + special + sci);
 y = io_printtext(x, y, width, s);
 io_printborder(x, y, width);
 return y;
}

int endgame_refresh(int focus)
{
 io_clearscreen();
 int num = data_numplayers();
 int x, y, i;
 print_wonder(34, 0, focus, -1);
 x = y = 0;
 for(i = 0; i < num; i++) {
  y = print_wondersmall(x, y, i, focus == i, 0);
 }
 printmessage(x, y+2, 35);
 return endgame_printscorebreakdown(focus);
}

void endgame()
{
 int focus = 0;
 int bottomy;
 while(1) {
  bottomy = endgame_refresh(focus);
  switch(io_getkey()) {
   case UP: focus--;
    break;
   case DOWN: focus++;
    break;
   case RIGHT: focus++;
    break;
   case LEFT: focus--;
    break;
   case ENTER:
    if(postyn(61, bottomy, "Are you sure you want to quit?")) halt();
    break;
   case '\t': focus = (focus+1)%(data_numplayers()+1);
    break;
   case 'q':
    if(postyn(61, bottomy, "Are you sure you want to quit?")) halt();
    break;
   default: break;
  }
  if(focus < 0) focus = data_numplayers()-1;
  focus = focus%data_numplayers();
 }
}
