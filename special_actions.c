#include "7w.h"

void Halicarnassus(int player) {
	int focus = data_numplayers();
	int cursor = 0;
	int discards[150];
	data_getdiscards(discards);
	int numcards = 0;
	while (discards[numcards++] != -1 && numcards < 50)
		;
	numcards /= 2;
	if (numcards == 0) {
		postmessage("No cards in the discard pile. Sucks to suck.");
		return;
	}
	while (1) {
		view_refresh(focus, cursor, player, 1);
		switch (io_getkey()) {
		case UP:
			cursor--;
			break;
		case DOWN:
			cursor++;
			break;
		case RIGHT:
			focus++;
			cursor = 0;
			break;
		case LEFT:
			focus--;
			cursor = 0;
			break;
		case '\t':
			focus = (focus + 1) % (data_numplayers() + 1);
			cursor = 0;
			break;
		case 'h':
			posthelp();
			break;
		case ENTER:
			if (data_hasbuiltname(player, discards[cursor * 2],
					discards[cursor * 2 + 1])) {
				postmessage("Cannot have two of the same card!");
			} else {
				data_freebuild(player, discards[cursor * 2],
						discards[cursor * 2 + 1]);
				data_deletediscard(discards[cursor * 2],
						discards[cursor * 2 + 1]);
				return;
			}
			break;
		default:
			break;
		}
		if (focus < 0)
			focus = data_numplayers();
		focus = focus % (data_numplayers() + 1);
		if (focus == data_numplayers()) {
			if (cursor < 0)
				cursor = numcards - 1;
			if (cursor >= numcards)
				cursor = 0;
		} else {
			if (cursor < 0)
				cursor = wonder_numstages((player + focus) % data_numplayers())
						+ data_numbuilt(player + focus % data_numplayers()) - 1;
			cursor =
					cursor
							% (wonder_numstages(
									(player + focus) % data_numplayers())
									+ data_numbuilt(
											player + focus % data_numplayers()));
		}
	}
}

static int hal = 0;
static int oly = 0;

void special_action(int player, int wonder, int stage) {
	if (wonder == 8) { //Halicarnassus
		if ((stage == 2 && hal == 0) || (stage == 4 && hal == 0)
				|| (stage == 5 && hal == 1) || (stage == 6 && hal == 2)) {
			if (data_isai(player))
				; //TODO: Implement this!
			else
				Halicarnassus(player);
			hal++;
		}
	}
	if (wonder == 7) { //Olympia
		if (stage == 2 && !oly) {
			data_setfreebuild(player);
			oly = 1;
		}
	}
}
