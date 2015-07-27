#include "7w.h"
#include <stdio.h>

int wonder_numstages(int player) {
	int side = data_getwonderside(player);
	int wonder = data_getwonder(player);
	if (side == 0)
		return 3;
	if (wonder == 9)
		return 4;
	if (wonder == 3)
		return 2;
	return 3;
}

int print_cards(int x, int y, int *cards, int cursor) {
	int i = 0;
	int j;
	for (j = 0; cards[j] != -1; j += 2) {
		io_printname(x, y++, cards[j], cards[j + 1]);
		if (cursor == i++) {
			io_printplain(x + 25, y - 1, "*");
		}
	}
	if (j == 0)
		y--;
	io_printborder(x, y, 28);
	return y;
}

int* print_wonder(int x, int y, int player, int cursor, int ret[2]) {
	int i, j;
	char array[40];
	io_printborder(x, y++, 28);
	y = io_printtext(x, y, 28, cards_getname(data_getwonder(player), 0, array));
	y = io_printtext(x, y, 28,
			cat("Produces: 1 ",
					getname(cards_gettype(data_getwonder(player), 0))));
	y = io_printtext(x, y, 28,
			cat(cat("Treasury: ", itoa(data_getgold(player))), " gold"));
	io_printborder(x, y++, 28);

	//Print resource incomes
	int def[GOLD];
	data_getdefinites(player, def);
	def[cards_gettype(data_getwonder(player), 0)]++;
	int **indef = data_getindefinites(player);
	y = io_printtext(x, y, 28, "Production:");
	for (i = 0; i < GOLD; i++) {
		if (def[i]) {
			y = io_printtext(x, y, 28,
					cat(cat(cat(" ", getname(i)), ": "), itoa(def[i])));
		}
	}
	for (i = 0; i < INDEF; i++) {
		char *text = "";
		for (j = 0; j < 4; j++) {
			if (indef[i][j] != -1)
				text = cat(cat(text, "/"), getname(indef[i][j]));
		}
		if (text[0] != '\0') {
			y = io_printtext(x, y, 28, text);
			io_printplain(x + 2, y - 1, " ");
		}
	}
	io_printborder(x, y++, 28);

	//Print wonder stages
	for (i = 0; i < wonder_numstages(player); i++) {
		char *text = cat("Stage ", itoa(i + 1));
		if (data_getwonderstages(player) > i)
			text = cat(text, " (complete)");
		else
			text = cat(text, "           ");
		if (cursor == i)
			text = cat(text, "     *");
		else
			text = cat(text, "  ");
		y = io_printtext(x, y, 28, text);
	}
	io_printborder(x, y++, 28);

	//Print what has been built
	int *built = data_getbuilt(player);
	y = print_cards(x, y, built, cursor - i);
	int print = 2 * (cursor - i);

	//Info about component
	ret[0] = ret[1] = -1;
	if (cursor >= 0 && cursor < wonder_numstages(player)) {
		ret[0] = data_getwonder(player);
		ret[1] = cursor + 1 + 3 * data_getwonderside(player);
	}
	if (cursor >= wonder_numstages(player)) {
		ret[0] = built[print];
		ret[1] = built[print + 1];
	}
	return ret;
}

//dir is 0 for none, 1 for east, 2 for west
int print_wondersmall(int x, int y, int player, int selected, int dir) {
	char s[28];
	io_printborder(x, y++, 28);
	if (selected)
		io_printplain(28, y, "*");
	if (dir == 1)
		io_printplain(29, y, "East");
	if (dir == 2)
		io_printplain(29, y, "West");
	char array[40];
	y = io_printtext(x, y, 28, cards_getname(data_getwonder(player), 0, array));
	sprintf(s, "army: %-2d vps: %-2d gold: %-2d", military_might(player),
			data_gettotvps(player), data_getgold(player));
	y = io_printtext(x, y, 28, s);
	io_printborder(x, y, 28);
	return y;
}
