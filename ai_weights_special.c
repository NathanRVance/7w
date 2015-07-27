#include "7w.h"

//dir 0 is east, 1 is west
int weight_tradingpost(int dir, int player) {
	int weight = 0;
	int trade[GOLD + 1];
	data_gettradables(data_getdir(dir, player), trade);
	int prod[GOLD + 1];
	data_gettradables(player, prod);
	int i;
	for (i = 0; i < CLOTH; i++) {
		if (trade[i] && !prod[i])
			weight++;
	}
	return weight;
}

int weight_marketplace(int player) {
	int weight = 0;
	int i, j;
	int prod[GOLD + 1];
	data_gettradables(player, prod);
	int trade[GOLD + 1];
	for (i = 0; i < 2; i++) {
		data_gettradables(data_getdir(i, player), trade);
		for (j = CLOTH; j <= PAPER; j++) {
			if (trade[j] && !prod[j])
				weight++;
		}
	}
	return weight;
}

int weight_gold(int gold, int player) {
	return gold - data_getgold(player);
}

