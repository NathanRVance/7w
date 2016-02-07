#include <curses.h>
#include <stdlib.h>
#include "7w.h"

void io_init() {
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

int io_getkey() {
	int c;
	switch (c = getch()) {
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

static void turn_off_attributes() {
	int i;
	for(i = 31; i <= 38; i++)
		attroff(COLOR_PAIR(i));
	attroff(A_BLINK);
	attroff(A_REVERSE);
}

void io_clearscreen() {
	clear();
}

void io_printname(int x, int y, int era, int card) {
	mvprintw(y, x, "#  ");
	attron(COLOR_PAIR(gettypecolor(cards_gettype(era, card))));
	char array[40];
	printw("%-23s", cards_getname(era, card, array));
	//attrset(A_NORMAL);
	turn_off_attributes();
	printw(" #");
}

void io_printblankline(int x, int y, int width) {
	int i;
	mvprintw(y, x, "#");
	for (i = 0; i < width - 1; i++)
		printw(" ");
	printw("#");
}

int io_printtext(int xorigin, int y, int width, char* text) {
	width -= 1; //padding
	int x = xorigin + 2;
	int wordlength;
	int wordstart = 0;
	int i;
	//start with a cleared line
	io_printblankline(xorigin, y, width);
	while (text[wordstart] != '\0') {
		wordlength = 0;
		while (text[wordstart + (wordlength++)] != ' '
				&& text[wordstart + wordlength] != '\0'
				&& text[wordstart + wordlength] != '\n')
			;
		if (x - xorigin + wordlength > width || text[wordstart] == '\n') {
			io_printblankline(xorigin, ++y, width);
			x = xorigin + 2;
		}
		mvprintw(y, x, "");
		for (i = wordstart; i < wordstart + wordlength; i++) {
			if (text[i] != '\n')
				addch(text[i]);
		}
		x += wordlength;
		wordstart += wordlength;
	}
	return y + 1;
}

void io_printborder(int x, int y, int width) {
	int i;
	for (i = 0; i < width; i++)
		mvprintw(y, x + i, "#");
}

int io_printcard(int x, int y, int era, int card, int player) {
	io_printborder(x, y++, 28);
	char array[40];
	if (cards_getname(era, card, array)[0] != '\0')
		io_printname(x, y++, era, card);
	int cost[NUMRESOURCES];
	cards_getcost(era, card, cost);
	int prod[NUMPRODUCTS];
	cards_getproduction(era, card, prod);
	int hasCP = 0;
	int i, j, k;
	for (i = 0; i < NUMRESOURCES; i++)
		if (cost[i] > 0)
			hasCP = 1;
	for (i = 0; i < NUMPRODUCTS; i++)
		if (prod[i] > 0)
			hasCP = 1;
	if (hasCP) {
		mvprintw(y++, x, "# Costs:   | Produces:     #");
		i = j = -1;
		while (i < NUMRESOURCES || j < NUMPRODUCTS) {
			while (i < NUMRESOURCES && cost[++i] == 0)
				;
			while (j < NUMPRODUCTS && prod[++j] == 0)
				;
			if (i == NUMRESOURCES && j == NUMPRODUCTS)
				break;
			mvprintw(y++, x, "# ");
			if (i < NUMRESOURCES)
				printw(" %d %-6s| ", cost[i], getname(i));
			else
				printw("         | ");
			int isFinal = 1;
			for (k = j + 1; k < NUMPRODUCTS; k++)
				if (prod[k])
					isFinal = 0;
			if (j < NUMPRODUCTS)
				if (isFinal || j == GOLD || j == SHIELD || j == VP)
					printw(" %d %-10s #", prod[j], getname(j));
				else
					printw(" %d %-7s or #", prod[j], getname(j));
			else
				printw("              #");
		}
	}

	char message[200];
	cards_specialmessage(era, card, message);
	if (message[0] != '\0')
		y = io_printtext(x, y, 28, message);

	int* special = get_special(era, card, player);
	if (special[2]) { //print special (vp, gold)
		mvprintw(y++, x, "# In your case, produces:  #");
		mvprintw(y++, x, "# %2d victory points        #", special[0]);
		mvprintw(y++, x, "# %2d gold                  #", special[1]);
	}

	int coupons[4];
	cards_getcoupons(era, card, coupons);
	if (coupons[1] || coupons[3]) { //print the coupons!
		mvprintw(y++, x, "# Coupon for:              #");
		if (coupons[1])
			io_printname(x, y++, coupons[0], coupons[1]);
		if (coupons[3])
			io_printname(x, y++, coupons[2], coupons[3]);
	}

	cards_getcouponed(era, card, coupons);
	if (coupons[1] || coupons[3]) { //print the coupons!
		mvprintw(y++, x, "# Free if owned:           #");
		if (coupons[1])
			io_printname(x, y++, coupons[0], coupons[1]);
		if (coupons[3])
			io_printname(x, y++, coupons[2], coupons[3]);
	}

	io_printborder(x, y, 28);
	return y;
}

int io_printhand(int x, int y, int player, int cursor) {
	int hand[7];
	data_gethand(player, hand);
	int i;
	io_printborder(x, y++, 28);
	y = io_printtext(x, y, 28, "          Hand");
	for (i = 0; i < 7 && hand[i] != -1; i++) {
		io_printname(x, y++, data_getera(), hand[i]);
		if (i == cursor) {
			mvprintw(y - 1, x + 25, "*");
		}
	}
	y = io_printtext(x, y, 28, " Trade...");
	if (i == cursor)
		mvprintw(y - 1, x + 25, "*");
	else if (cursor >= 0 && cursor < 7 && hand[cursor] != -1)
		y = io_printcard(x, y, data_getera(), hand[cursor], player);
	else
		io_printborder(x, y++, 28);
	return y;
}

int io_printdiscards(int x, int y, int player, int cursor) {
	int discards[150];
	data_getdiscards(discards);
	io_printborder(x, y++, 28);
	y = io_printtext(x, y, 28, "         Discards");
	y = print_cards(x, y, discards, cursor);
	y = io_printcard(x, y, discards[cursor * 2], discards[cursor * 2 + 1],
			player);
	return y;
}

void io_printplain(int x, int y, char *s) {
	mvprintw(y, x, "%s", s);
}

void io_printcolor(int x, int y, int color, char *s) {
	attron(COLOR_PAIR(color));
	io_printplain(x, y, s);
	//attrset(A_NORMAL);
	turn_off_attributes();
}
