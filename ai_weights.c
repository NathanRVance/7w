#include "7w.h"

int weight_science(int era, int card, int player) {
	int weight = 0;
	int science[3];
	get_science(player, science);
	int min = 100;
	int max = 0;
	int i, j;
	for (i = 0; i < 3; i++) {
		if (science[i] < min)
			min = science[i];
		if (science[i] > max)
			max = science[i];
	}
	if (era > 0 && max == 0)
		return 0;
	int prod[NUMPRODUCTS];
	cards_getproduction(era, card, prod);
	for (i = COMPASS; prod[i] == 0; i++)
		;
	i -= COMPASS;
	if (science[i] == min)
		weight = 5;
	int tots[3];
	for (i = 0; i < 3; i++)
		tots[i] = 0;
	for (i = 0; i < 3; i++) {
		get_science(data_getdir(i, player), science);
		for (j = 0; j < 3; j++) {
			tots[i] += science[j];
		}
	}
	if (tots[2] < tots[0] - 1 || tots[2] < tots[1] - 1)
		weight -= 3;
	return weight;
}

int weight_military(int era, int card, int player) {
	int weight = 0;
	int prod[NUMPRODUCTS];
	int military = cards_getproduction(era, card, prod)[SHIELD];
	int strength[3];
	int i;
	for (i = 0; i < 3; i++)
		strength[i] = military_might(data_getdir(i, player));
	for (i = 0; i < 2; i++) {
		if (strength[i] >= strength[2] && strength[i] < strength[2] + military)
			weight += (era + 2) * 2;
		if (strength[i] == strength[2] + military)
			weight += (era + 1) * 2;
		if (strength[i] < strength[2])
			weight += 5 / (strength[2] - strength[i]);
	}
	return weight;
}

int weight_resource(int era, int card, int player) {
	int weight = 0;
	int prod[NUMPRODUCTS];
	cards_getproduction(era, card, prod);
	int trade[GOLD + 1];
	data_gettradables(player, trade);
	int i, j;
	for (i = 0; i < GOLD; i++) {
		if (prod[i] && !trade[i])
			weight += 5;
		if (prod[i] && (i == STONE || i == ORE) && trade[i] < 2)
			weight += 2;
	}
	for (j = 0; j < 2; j++) {
		data_gettradables(data_getdir(j, player), trade);
		for (i = 0; i < GOLD; i++) {
			if (prod[i] && !trade[i])
				weight++;
			if (prod[i] && trade[i] && get_trade(player, i, j) == 1)
				weight--;
		}
	}
	return weight;
}

int weight_tradingpost(int dir, int player);
int weight_marketplace(int player);
int weight_gold(int gold, int player);

int weight_commercial(int era, int card, int player) {
	if (era == 0) {
		if (card == 17)
			return weight_gold(4, player);
		if (card == 18)
			return weight_tradingpost(0, player);
		if (card == 19)
			return weight_tradingpost(1, player);
		if (card == 20)
			return weight_marketplace(player);
	}
	if (era == 1) {
		if (card == 11 || card == 12)
			return weight_resource(era, card, player);
		if (card == 13 || card == 14)
			return weight_gold(get_special(era, card, player)[1], player);

	}
	return 0;
}

int weight_buildcard(int era, int card, int player) {
	int weight = data_potentialvps(player, era, card);
	weight -= data_gettotvps(player);
	//if (cards_gettype(era, card) == STRUCTURE); already done
	if (cards_gettype(era, card) == MILITARY)
		weight += weight_military(era, card, player);
	if (era == 2)
		return weight;
	if (cards_gettype(era, card) == SCIENTIFIC)
		weight += weight_science(era, card, player);
	if (cards_gettype(era, card) == RESOURCE
			|| cards_gettype(era, card) == INDUSTRY)
		weight += weight_resource(era, card, player);
	if (cards_gettype(era, card) == COMMERCIAL)
		weight += weight_commercial(era, card, player);
	return weight;
}

int* weight_buildwonder(int player, int ret[4]) {
	int trade[3];
	ret[0] = (data_getwonderstages(player) + 2) * 2;
	ret[1] = ret[2] = ret[3] = 0;
	if (data_canafford(player, data_getwonder(player),
			data_getnextwonderstage(player)) != 1) {
		ai_trade(player, data_getwonder(player), data_getnextwonderstage(player), trade);
		if (trade[2])
			ret[0] -= trade[2] / 3;
		else
			ret[0] = 0;
		ret[1] = trade[0];
		ret[2] = trade[1];
		ret[3] = trade[2];
	}
	return ret;
}
