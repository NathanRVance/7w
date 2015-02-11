#include <curses.h>
#include <stdlib.h>
#include "7w.h"

char* cards_getname(int era, int card);
int* cards_getcost(int era, int card);
int cards_gettype(int era, int card);
int* cards_getproduction(int era, int card);
char* getname(int res);
int gettypecolor(int type);
int* cards_getcoupons(int era, int card);
int* cards_getcouponed(int era, int card);

void io_init()
{
 initscr();
 clear();
 noecho();
 cbreak();
 curs_set(0);
 keypad(stdscr, TRUE);
 start_color();
 use_default_colors();

//This may seem slightly conveluted, but it's the curses way.
 init_pair(0, A_NORMAL, -1);
 init_pair(30, COLOR_BLACK, -1);
 init_pair(31, COLOR_RED, -1);
 init_pair(32, COLOR_GREEN, -1);
 init_pair(33, COLOR_YELLOW, -1);
 init_pair(34, COLOR_BLUE, -1);
 init_pair(35, COLOR_MAGENTA, -1);
 init_pair(36, COLOR_CYAN, -1);
 init_pair(37, COLOR_WHITE, -1);
}

int io_getkey()
{
int c;
 switch(c = getch()) {
 case KEY_LEFT:
  return LEFT;
  break;
 case KEY_RIGHT:
  return RIGHT;
  break;
 case KEY_UP:
  return UP;
  break;
 case KEY_DOWN:
  return DOWN;
  break;
 case KEY_ENTER:
 case '\n':
 case '\r':
  return ENTER;
  break;
 default:
  return c;
  break;
 }
}

void io_printname(int x, int y, int era, int card)
{
 mvprintw(y, x, "#  ");
 attron(COLOR_PAIR(gettypecolor(cards_gettype(era, card))));
 printw("%-23s", cards_getname(era, card));
 attrset(A_NORMAL);
 printw(" #");
}

void io_printcard(int x, int y, int era, int card)
{
 mvprintw(y++, x, "############################");
 io_printname(x, y++, era, card);
 mvprintw(y++, x, "# Costs:   | Produces:     #");
 int *costs = cards_getcost(era, card);
 int *products = cards_getproduction(era, card);
 int i, j, k;
 i = j = -1;
 while(i < NUMRESOURCES || j < NUMPRODUCTS) {
  while(i < NUMRESOURCES && costs[++i] == 0);
  while(j < NUMPRODUCTS && products[++j] == 0);
  if(i == NUMRESOURCES && j == NUMPRODUCTS) break;
  mvprintw(y++, x, "# ");
  if(i < NUMRESOURCES) printw(" %d %-6s| ", costs[i], getname(i));
  else printw("         | ");
  int isFinal = 1;
  for(k = j+1; k < NUMPRODUCTS; k++)
   if(products[k]) isFinal = 0;
  if(j < NUMPRODUCTS)
   if(isFinal) printw(" %d %-10s #", products[j], getname(j));
   else printw(" %d %-7s or #", products[j], getname(j));
  else printw("              #");
 }
 
 int* coupons = cards_getcoupons(era, card);
 if(coupons[1] || coupons[3])
 { //print the coupons!
  mvprintw(y++, x, "# Coupon for:              #");
  if(coupons[1])
   io_printname(x, y++, coupons[0], coupons[1]);
  if(coupons[3])
   io_printname(x, y++, coupons[2], coupons[3]);
 }

 coupons = cards_getcouponed(era, card);
 if(coupons[1] || coupons[3])
 { //print the coupons!
  mvprintw(y++, x, "# Free if owned:           #");
  if(coupons[1])
   io_printname(x, y++, coupons[0], coupons[1]);
  if(coupons[3])
   io_printname(x, y++, coupons[2], coupons[3]);
 } 
 
 mvprintw(y++, x, "############################");
}
