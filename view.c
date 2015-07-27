#include "7w.h"

int* view_printwonders(int focus, int cursor, int player, int card[2]) {
	int num = data_numplayers();
	int x, y, i, dir, p;
	x = y = 0;
	for (i = 0; i < num; i++) {
		p = (player + i) % num;
		dir = 0;
		if (p == data_geteast(player))
			dir = 1;
		if (p == data_getwest(player))
			dir = 2;
		if (focus == p)
			print_wonder(34, 0, p, cursor, card);
		y = print_wondersmall(x, y, p, focus == p, dir);
	}
	y = write_history(focus, 0, y);
	printmessage(0, y + 2, 35);
	return card;
}

int view_refresh(int focus, int cursor, int player, int mode) //mode 0 is normal, 1 is discard search
{
	int ret;
	int card[2];
	io_clearscreen();
	if (focus == data_numplayers()) {
		view_printwonders(player, -1, player, card);
	} else {
		view_printwonders((focus + player) % data_numplayers(), cursor, player,
				card);
		cursor = -1;
	}
	if (mode)
		ret = io_printdiscards(61, 0, player, cursor);
	else
		ret = io_printhand(61, 0, player, cursor);
	if (card[0] != -1)
		ret = io_printcard(61, ret - 1, card[0], card[1], player);
	return ret;
}
