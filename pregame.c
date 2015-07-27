#include "7w.h"
#include <stdio.h>

void pregame_refresh(int* data, int cursor) {
	io_clearscreen();
	int y = 0;
	int width = 28;
	io_printborder(0, y++, width);
	char s[40];
	sprintf(s, "Number of players: %d  %c", data[7], (cursor == 0) ? '*' : ' ');
	y = io_printtext(0, y, width, s);
	int i;
	for (i = 0; i < data[7]; i++) {
		sprintf(s, "Player %d is: %s    %c", i + 1,
				(data[i] == 1) ? "ai   " : "human",
				(cursor == i + 1) ? '*' : ' ');
		y = io_printtext(0, y, width, s);
	}
	sprintf(s, "Play                  %c", (cursor == data[7] + 1) ? '*' : ' ');
	y = io_printtext(0, y, width, s);
	io_printborder(0, y++, width);
}

int* pregame() {
	static int ret[8] = { 0, 1, 1, 1, 1, 1, 1, 1 };
	ret[7] = 3;
	int cursor = 0;
	int pregaming = 1;
	while (pregaming) {
		pregame_refresh(ret, cursor);
		switch (io_getkey()) {
		case UP:
			cursor--;
			if (cursor < 0)
				cursor = ret[7] + 1;
			break;
		case DOWN:
			cursor = (cursor + 1) % (ret[7] + 2);
			break;
		case RIGHT:
		case ENTER:
			if (cursor == 0) {
				ret[7]++;
				if (ret[7] > 7)
					ret[7] = 3;
			} else if (cursor == ret[7] + 1) {
				pregaming = 0;
			} else {
				ret[cursor - 1] = !ret[cursor - 1];
			}
			break;
		case LEFT:
			if (cursor == 0) {
				ret[7]--;
				if (ret[7] < 3)
					ret[7] = 7;
			} else if (cursor == ret[7] + 1) {
				//do nothing
			} else {
				ret[cursor - 1] = !ret[cursor - 1];
			}
			break;
		}
	}
	return ret;
}
