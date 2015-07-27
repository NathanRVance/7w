#include "7w.h"

void ai_bestcard(int *hand, int player, int ret[2]) {
	int i, temp;
	int max = 0;
	int card = 0;
	int trade[3];
	for (i = 0; i < 5; i++)
		ret[i] = 0;
	for (i = 0; hand[i] != -1 && i < 7; i++) {
		trade[0] = trade[1] = trade[2] = 0;
		temp = weight_buildcard(data_getera(), hand[i], player);
		if (data_hasbuiltname(player, data_getera(), hand[i]))
			temp = 0;
		else if (data_canafford(player, data_getera(), hand[i]) != 1) {
			ai_trade(player, data_getera(), hand[i], trade);
			if (trade[2])
				temp -= trade[2] / 3;
			else
				temp = 0;
		}
		if (temp > max) {
			max = temp;
			card = hand[i];
			ret[2] = trade[0];
			ret[3] = trade[1];
			ret[4] = trade[2];
		}
	}
	ret[0] = max;
	ret[1] = card;
}

void ai_turn(int player) {
	int bestcard[5];
	int hand[7];
	data_gethand(player, hand);
	int wonder[4];
	wonder[0] = 0;
	int i;
	if (data_getwonderstages(player) < wonder_numstages(player))
		weight_buildwonder(player, wonder);
	ai_bestcard(hand, player, bestcard);
	if (bestcard[0] > wonder[0] && bestcard[0] > 0) {
		data_build(player, bestcard[1]);
		for (i = 0; i < 2; i++) {
			data_addgold(bestcard[2 + i], data_getdir(i, player));
		}
		write_trade(player, bestcard[2], bestcard[3]);
		data_addgold(bestcard[4] * -1, player);
		return;
	}
	ai_bestcard(hand, data_getnext(player), bestcard);
	if (bestcard[0] == 0)
		bestcard[1] = hand[0];
	if (wonder[0] > 0) {
		data_buildwonder(player, bestcard[1]);
		for (i = 0; i < 2; i++) {
			data_addgold(wonder[1 + i], data_getdir(i, player));
		}
		write_trade(player, wonder[1], wonder[2]);
		data_addgold(wonder[3] * -1, player);
		return;
	}
	data_discard(player, bestcard[1]);
	data_addgold(3, player);
}
