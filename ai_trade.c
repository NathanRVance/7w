#include "7w.h"

/*
 A brief note on why we have separated the functionality of trade.c and ai_trade.c in regards to gold transfer:
 This class is designed for the sole purpose of analyzing potential transactions without committing to any. Therefore, such handy methods as trade_commit are not used.
 */

static int bestCost[3];
static int baseCost[NUMRESOURCES];

void ai_cleartrade(int trade[3][GOLD]) {
	int i, j;
	for (i = 0; i < 3; i++) {
		bestCost[i] = 0;
		for (j = 0; j < GOLD; j++)
			trade[i][j] = 0;
	}
}

int saveCost(int trade[3][GOLD], int player) {
	int buffer[3] = { 0 };
	int i, j;
	for (i = 0; i < 2; i++) {
		for (j = 0; j < GOLD; j++) {
			buffer[i] += get_trade(player, j, i) * trade[i][j];
		}
		buffer[2] += buffer[i];
	}
	if (buffer[2] < bestCost[2] || bestCost[2] == 0) {
		for (i = 0; i < 3; i++)
			bestCost[i] = buffer[i];
	}
	return buffer[2];
}

void restoreCost(int ret[3]) {
	int i;
	for (i = 0; i < 3; i++)
		ret[i] = bestCost[i];
}

int bestNeighbor(int player, int res) {
	int best = 0;
	if (get_trade(player, res, 0) != get_trade(player, res, 1)) {
		best = (get_trade(player, res, 0) < get_trade(player, res, 1)) ? 0 : 1;
	} else if (data_gettotvps(data_getdir(0, player))
			> data_gettotvps(data_getdir(1, player)))
		best = 0;
	else
		best = 1;
	return best;
}

void recurse(int ret[], int cost, int trade[3][GOLD], int player, int era,
		int card) {
	int i, j, k;
	int tradables[GOLD + 1];
	data_gettradables(player, tradables);
	if (data_canafford(player, era, card)) {
		saveCost(trade, player);
		return; //if we can afford it, don't bother recursing further.
	}
	for (j = 0; j < GOLD; j++) {
		if (baseCost[j] - trade[0][j] - trade[1][j] - tradables[j] <= 0)
			continue;
		for (k = 0; k < 1; k++) {
			i = bestNeighbor(player, j);
			if (k)
				i = (i - 1) * -1;
			int temp[GOLD + 1];
			if (data_getgold(player) - cost - get_trade(player, j, i) >= 0
					&& trade_gettradables(player, i, temp)[j] >= 1) {
				trade[i][j]++;
				trade[2][j]++;
				trade_set(player, trade);
				recurse(ret, cost + get_trade(player, j, i), trade, player, era,
						card);
				trade[i][j]--;
				trade[2][j]--;
			}
		}
	}
}

int* ai_trade(int player, int era, int card, int ret[3]) {
	int trade[3][GOLD];
	int i;
	for (i = 0; i < 3; i++)
		ret[i] = 0;
	ai_cleartrade(trade);
	cards_getcost(era, card, baseCost);
	recurse(ret, 0, trade, player, era, card);
	restoreCost(ret);
	ai_cleartrade(trade);
	trade_set(player, trade);
	return ret;
}
