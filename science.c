#include "7w.h"

int* get_science(int player, int science[3]) {
	int i, j;
	for (i = 0; i < 3; i++)
		science[i] = 0;
	int *built = data_getbuilt(player);
	int prod[NUMPRODUCTS];
	for (i = 0; built[i] != -1; i += 2) {
		if (cards_gettype(built[i], built[i + 1]) == SCIENTIFIC) {
			cards_getproduction(built[i], built[i + 1], prod);
			for (j = COMPASS; j <= TABLET; j++) {
				science[j - COMPASS] += prod[j];
			}
		}
	}
	return science;
}

int calc_science(int player, int guildval) {
	int i;
	int science[3];
	get_science(player, science);
	if (guildval != -1)
		science[guildval]++;
	int min = science[0];
	for (i = 0; i < 3; i++)
		if (science[i] < min)
			min = science[i];
	int ret = 7 * min;
	for (i = 0; i < 3; i++)
		ret += science[i] * science[i];
	return ret;
}

int science(int player) {
	if (!data_hasbuilt(player, 2, 25))
		return calc_science(player, -1);
	int max = 0;
	int i, m;
	for (i = 0; i < 3; i++) {
		m = calc_science(player, i);
		if (m > max)
			max = m;
	}
	return max;
}
