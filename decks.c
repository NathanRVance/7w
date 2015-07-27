#include "7w.h"
#include <time.h>
#include <stdlib.h>

static int era1[] = { 0, 1, 2, 3, 6, 7, 10, 11, 12, 14, 15, 16, 18, 19, 20, 21,
		22, 23, 24, 25, 26, 0, 3, 5, 13, 17, 23, 26, 1, 2, 8, 15, 17, 22, 24, 4,
		9, 10, 11, 12, 16, 20, 13, 14, 17, 18, 19, 21, 25 };
static int era2[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 15, 17, 18,
		19, 20, 21, 22, 0, 1, 2, 3, 14, 16, 19, 4, 5, 6, 10, 12, 17, 20, 8, 11,
		12, 13, 16, 18, 21, 7, 9, 11, 14, 15, 16, 22 };
static int era3[] = { 0, 1, 2, 3, 4, 5, 6, 8, 9, 11, 12, 13, 14, 15, 16, 17, 1,
		5, 7, 10, 11, 15, 2, 4, 8, 10, 12, 17, 0, 2, 6, 7, 10, 13, 3, 8, 9, 11,
		14, 16 };

void shuffle(int *deck, int n) {
	int scratch[n];
	int i = 0;
	int size = n;
	int k;
	srand(time(NULL));
	while (n) {
		k = rand() % n;
		scratch[i++] = deck[k];
		for (; k < n - 1; k++)
			deck[k] = deck[k + 1];
		n--;
	}
	for (i = 0; i < size; i++)
		deck[i] = scratch[i];
}

int* getdeck(int era, int numplayers, int ret[numplayers * 7]) {
	int guilds[10];
	int i, j;
	switch (era) {
	case 0:
		for (i = 0; i < numplayers * 7; i++)
			ret[i] = era1[i];
		break;
	case 1:
		for (i = 0; i < numplayers * 7; i++)
			ret[i] = era2[i];
		break;
	case 2:
		for (i = 0; i < 10; i++)
			guilds[i] = i + 18;
		shuffle(guilds, 10);
		for (i = 0; i < numplayers * 6 - 1; i++)
			ret[i] = era3[i];
		for (j = 0; i < numplayers * 7; i++, j++)
			ret[i] = guilds[j];
		break;
	default:
		break;
	}
	shuffle(ret, numplayers * 7);
	return ret;
}
