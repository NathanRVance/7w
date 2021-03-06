#include "7w.h"

int player_build(int focus, int cursor, int player, int y) {
	int hand[7];
	data_gethand(player, hand);
	int afford = data_canafford(player, data_getera(), hand[cursor]);
	if (focus == data_numplayers()) {
		if (hand[cursor] == -1)
			return 0;
		int choice = postoptions(61, y);
		if (choice == 0) {
			if (data_hasbuiltname(player, data_getera(), hand[cursor])) {
				postmessage("Cannot have two of the same card!");
				return 0;
			}
			if (afford == 1
					|| (afford == 2 && postyn(61, y, "Spend free build?"))) {
				data_build(player, hand[cursor]);
				if (afford == 2)
					data_spendfreebuild();
				return 1;
			} else
				postmessage("Can't afford this!");
		} else if (choice == 1) {
			data_discard(player, hand[cursor]);
			data_addgold(3, player);
			return 1;
		} else if (choice == 2) {
			if (data_getnextwonderstage(player) == -1) {
				postmessage("Wonder already complete.");
			} else if (data_canafford(player, data_getwonder(player),
					data_getnextwonderstage(player))) {
				data_buildwonder(player, hand[cursor]);
				return 1;
			} else
				postmessage("Can't afford this!");
		}
	}
	return 0;
}

void player_turn(int player) {
	int numcards, tradey;
	int cursor = 0;
	int focus = data_numplayers();
	while (1) {
		numcards = data_numcards(player) + 1; //include trade option in hand
		tradey = view_refresh(focus, cursor, player, 0);
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
		case ENTER:
			if (player_build(focus, cursor, player, tradey)) {
				trade_commit(player);
				clearmessage();
				return;
			}
			break;
		case '\t':
			focus = (focus + 1) % (data_numplayers() + 1);
			cursor = 0;
			break;
		case 'h':
			posthelp();
			break;
		case 'q':
			if (postyn(61, tradey, "Are you sure you want to quit?"))
				halt();
			break;
		default:
			break;
		}
		if (focus < 0)
			focus = data_numplayers();
		focus = focus % (data_numplayers() + 1);
		if (focus == data_numplayers()) {
			if (cursor < 0)
				cursor = numcards - 2;
			if (cursor >= numcards - 1) {
				if (trade_routine(61, view_refresh(focus, cursor, player, 0),
						player))
					cursor = 0;
				else
					cursor = numcards - 2;
			}
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
