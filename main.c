#include "7w.h"
#include <stdlib.h>
#include <stdio.h>
#include <curses.h>

void halt() {
	endwin();
	exit(0);
}

void haltError(char *message, int num) {
	endwin();
	printf("Something has gone wrong, very wrong.\n");
	printf(message);
	printf("\n");
	printf("Number associated with this error: %d\n", num);
	exit(1);
}

void main_routine() {
	int player = 0;
	while (1) {
		if (data_isai(player))
			ai_turn(player);
		else
			player_turn(player);
		if (++player == data_numplayers()) {
			data_endturn();
			player = 0;
		}
	}
}

int main() {
	io_init();
	cards_init();
	int *setupdat = pregame();
	data_init(setupdat[7]);
	int i;
	for (i = 0; i < 7; i++) {
		if (setupdat[i])
			data_setai(i);
	}
	main_routine();
	halt();
}
