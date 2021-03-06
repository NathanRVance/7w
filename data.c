#include "7w.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define MISC 3
#define DATAGOLD 0

static int decks[3][49];
static int discards[3][49];
static int player[7][4][7]; //3 eras and extra stuff (wonder, wonder side, wonder stages completed, gold, military wins, defeats, vps(partial))
static int buffer[7][4];
static int hands[7][7];
static int ais[7];
static int numplayers;
static int era;
static int turn;
static int totturns;
static int turngoldbuffer[7];
static int hasfreebuild; //holds player that has a free build. Once set, will be that player, or (player+1)*-1 if already used.

void data_sorthands() {
	int i, j, k, type, buff[7];
	for (i = 0; i < numplayers; i++) {
		k = 0;
		for (type = 0; type <= 7; type++) {
			for (j = 0; j < 7; j++) {
				if (cards_gettype(era, hands[i][j]) == type)
					buff[k++] = hands[i][j];
			}
		}
		for (k = 0; k < 7; k++)
			hands[i][k] = buff[k];
	}
}

void data_endgame();
void data_discard(int p, int card);

void data_nextera() {
	int i, j, k;
	if(era != -1)
		war();
	if (era == 2)
		data_endgame();
	era++;
	turn = 0;
	totturns = 0;
	k = 0;
	for (i = 0; i < numplayers; i++)
		for (j = 0; j < 7; j++)
			hands[i][j] = decks[era][k++];
	data_sorthands();
	if (hasfreebuild < 0 && hasfreebuild != -10)
		hasfreebuild = hasfreebuild * -1 - 1;
}

void data_distributewonders() {
	int i;
	int wonders[7];
	for (i = 0; i < 7; i++)
		wonders[i] = i + 3; //wonders are numbered 3-9
	shuffle(wonders, 7);
	for (i = 0; i < numplayers; i++) {
		player[i][3][0] = wonders[i];
		player[i][3][1] = rand() % 2;
	}
}

void data_init(int n) {
	hasfreebuild = -10;
	numplayers = n;
	int i, j, k;
	int deck[numplayers * 7];
	for (i = 0; i < 3; i++) {
		getdeck(i, numplayers, deck);
		for (j = 0; j < numplayers * 7; j++)
			decks[i][j] = deck[j];
	}
	for (i = 0; i < 3; i++)
		for (j = 0; j < 49; j++)
			discards[i][j] = -1;
	era = -1;
	data_nextera();
	data_distributewonders(n);
	for (i = 0; i < n; i++) {
		for (j = 0; j < 3; j++)
			for (k = 0; k < 7; k++)
				player[i][j][k] = -1;
		player[i][3][3] = 2; //start with 2 gold
		buffer[i][0] = -1;
	}
}

void data_flushbuffers() {
	int i, j;
	for (i = 0; i < 7; i++)
		turngoldbuffer[i] = 0; //Clear the visible buffer, the real one will be processed later.
	for (i = 0; i < numplayers; i++) {
		if (buffer[i][0] == -2) { //build wonder
			player[i][3][2]++;
			write_purchase(i, -1, -1, 2);
		} else if (buffer[i][0] != -1) { //bulid card
			for (j = 0; player[i][era][j] != -1; j++)
				; //get free spot in array
			player[i][era][j] = buffer[i][0];
			write_purchase(i, era, buffer[i][0], 0);
		} else { //sold card
			write_purchase(i, -1, -1, 1);
		}
		player[i][3][3] += buffer[i][1]; //change in gold
		player[i][3][6] += buffer[i][2];
		buffer[i][0] = -1;
		buffer[i][1] = 0;
		buffer[i][2] = 0;
	}
}

void data_endturn() { //remember, player numbers are arranged clockwise
	totturns++;
	data_flushbuffers();
	int i;
	if (totturns == 6) {
		for (i = 0; i < numplayers; i++) {
			if (data_getwonder(i) == 6
					&& data_getwonderside(i) * 3 + data_getwonderstages(i)
							>= 5) {
				if (data_isai(i))
					ai_turn(i);
				else
					player_turn(i);
				data_flushbuffers();
			} else
				data_discard((i + numplayers - turn) % numplayers, hands[i][0]);
		}
		for (i = 0; i < numplayers; i++) {
			special_action(i, data_getwonder(i),
					data_getwonderside(i) * 3 + data_getwonderstages(i));
		}
		data_nextera();
		return;
	}
	for (i = 0; i < numplayers; i++) {
		special_action(i, data_getwonder(i),
				data_getwonderside(i) * 3 + data_getwonderstages(i));
	}
	if (era == 0 || era == 2) //pass to the left
		turn++;
	else
		//pass to the right
		turn--;
	if (turn < 0)
		turn = numplayers - 1;
}

void data_setfreebuild(int p) {
	hasfreebuild = p;
}

void data_spendfreebuild() {
	hasfreebuild = (hasfreebuild + 1) * -1;
}

int data_turnnum() {
	return totturns;
}

void data_setai(int p) {
	ais[p] = 1;
}

int data_isai(int p) {
	return ais[p];
}

int data_geteast(int p) {
	int ret = p - 1;
	if (ret < 0)
		ret = numplayers - 1;
	return ret;
}

int data_getwest(int p) {
	return (p + 1) % numplayers;
}

int data_getdir(int dir, int p) {
	if (dir == 0)
		return data_geteast(p);
	if (dir == 1)
		return data_getwest(p);
	return p;
}

int data_getnext(int p) {
	if (era == 0 || era == 2)
		return data_getwest(p);
	return data_geteast(p);
}

int* data_gethand(int p, int ret[7]) {
	int i;
	for(i = 0; i < 7; i++) {
		ret[i] = hands[(p + turn) % numplayers][i];
	}
	return ret;
}

int data_numcards(int p) {
	int hand[7];
	data_gethand(p, hand);
	int numcards;
	for (numcards = 0; numcards < 7 && hand[numcards] > -1; numcards++)
		;
	return numcards;
}

int data_getera() {
	return era;
}

int data_getwonder(int p) {
	return player[p][3][0];
}

int data_getwonderside(int p) {
	return player[p][3][1];
}

int data_getwonderstages(int p) {
	return player[p][3][2];
}

int data_getnextwonderstage(int p) {
	int side = data_getwonderside(p);
	int stage = data_getwonderstages(p);
	int wonder = data_getwonder(p);
	if (side == 0) {
		if (stage >= 3)
			return -1;
	} else {
		if (wonder == 3 && stage >= 2)
			return -1;
		if (wonder != 9 && stage >= 3)
			return -1;
		if (wonder == 9 && stage >= 4)
			return -1;
	}
	return side * 3 + 1 + stage;
}

int data_getdefeats(int p) {
	return player[p][3][5];
}

void data_adddefeat(int p) {
	player[p][3][5]++;
}

void data_addvictory(int p) {
	int amnt;
	if (era == 0)
		amnt = 1;
	if (era == 1)
		amnt = 3;
	if (era == 2)
		amnt = 5;
	player[p][3][4] += amnt;
}

int data_getvictories(int p) {
	return player[p][3][4];
}

int data_getgold(int p) {
	return player[p][3][3] + turngoldbuffer[p];
}

void data_addgold(int amnt, int p) {
	buffer[p][1] += amnt;
	turngoldbuffer[p] += amnt;
}

void data_addvps(int amnt, int p) {
	buffer[p][2] += amnt;
}

void data_setspecialflag(int p) {
	buffer[p][3] = 1;
}

int data_numplayers() {
	return numplayers;
}

int* data_getdiscards(int ret[150]) {
	int i, j, k;
	for (i = 0; i < 150; i++)
		ret[i] = -1;
	k = 0;
	for (i = 0; i < 3; i++) {
		for (j = 0; discards[i][j] != -1 && j < 49; j++) {
			ret[k++] = i;
			ret[k++] = discards[i][j];
		}
	}
	return ret;
}

void data_deletediscard(int era, int card) {
	int i;
	for (i = 0; discards[era][i] != card && i < 48; i++)
		;
	discards[era][i] = -1;
}

int data_remove(int p, int card) //as in, removes it from the hand
{
	int i;
	int *hand = hands[(p + turn) % numplayers];
	for (i = 0; hand[i] != card && i < 7; i++)
		;
	if (i == 7)
		return 1;
	for (; i < 6; i++)
		hand[i] = hand[i + 1];
	hand[6] = -1;
	for (i = 0; i < 7; i++)
		turngoldbuffer[i] = 0; //end of turn
	return 0;
}

void data_discard(int p, int card) {
	if (data_remove(p, card))
		haltError("Error discarding", totturns);
	int i;
	for (i = 0; discards[era][i] != -1 && i < 49; i++)
		;
	discards[era][i] = card;
}

void data_freebuild(int p, int e, int card) {
	int i;
	for (i = 0; player[p][e][i] != -1; i++)
		;
	player[p][e][i] = card;
	int prod[NUMPRODUCTS];
	data_addgold(cards_getproduction(e, card, prod)[GOLD], p);
	data_addvps(cards_getproduction(e, card, prod)[VP], p);
	data_addgold(get_special(e, card, p)[1], p);
}

void data_build(int p, int card) {
	buffer[p][0] = card; //will be added to player array at end of turn
	int prod[NUMPRODUCTS];
	data_addgold(cards_getproduction(era, card, prod)[GOLD], p);
	data_addvps(cards_getproduction(era, card, prod)[VP], p);
	data_addgold(get_special(era, card, p)[1], p);
	int array[NUMRESOURCES];
	data_addgold(cards_getcost(era, card, array)[GOLD] * -1, p);
	if (data_remove(p, card))
		haltError("Error building card", totturns);
}

void data_buildwonder(int p, int card) {
	buffer[p][0] = -2;
	int cost[NUMRESOURCES];
	int prod[NUMPRODUCTS];
	data_addgold(
			cards_getcost(data_getwonder(p),
					data_getwonderside(p) * 3 + 1 + data_getwonderstages(p), cost)[GOLD]
					* -1, p);
	data_addgold(
			cards_getproduction(data_getwonder(p),
					data_getwonderside(p) * 3 + 1 + data_getwonderstages(p), prod)[GOLD],
			p);
	data_addvps(
			cards_getproduction(data_getwonder(p),
					data_getwonderside(p) * 3 + 1 + data_getwonderstages(p), prod)[VP],
			p);
	data_setspecialflag(p);
	if (data_remove(p, card))
		haltError("Error building wonder", totturns);
}

//0 is non producing, 1 produces one kind of resource, 2 produces multiple resources
int data_productiontype(int e, int card) {
	if (e < 0 || card < 0)
		return 0;
	int prod[NUMPRODUCTS];
	cards_getproduction(e, card, prod);
	int i;
	int type = 0;
	for (i = 0; i < GOLD; i++) {
		if (prod[i]) {
			if (type == 1)
				type = 2;
			if (type == 0)
				type = 1;
		}
	}
	return type;
}

int* data_getdefinites(int p, int ret[GOLD]) {
	int prod[NUMPRODUCTS];
	int i, j, k;
	for (i = 0; i < GOLD; i++)
		ret[i] = 0;
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 7; j++) {
			if (data_productiontype(i, player[p][i][j]) == 1) {
				cards_getproduction(i, player[p][i][j], prod);
				for (k = 0; k < GOLD; k++) {
					ret[k] += prod[k];
				}
			}
		}
	}
	return ret;
}

void data_removedefinites(int p, int *cost) {
	cost[cards_gettype(data_getwonder(p), 0)]--;
	int prod[GOLD];
	data_getdefinites(p, prod);
	int i;
	for (i = 0; i < GOLD; i++) {
		cost[i] -= prod[i];
		if (cost[i] < 0)
			cost[i] = 0;
	}
}

void data_removetraded(int p, int *cost) {
	int *trade = trade_buffer();
	int i;
	for (i = 0; i < GOLD; i++) {
		cost[i] -= trade[i];
		if (cost[i] < 0)
			cost[i] = 0;
	}
}

int** data_getindefinites(int p) {
	int i, j, k, l, m;
	int prod[NUMPRODUCTS];
	static int dats[4 * INDEF];
	static int *ret[INDEF];
	for (i = 0; i < INDEF; i++) {
		ret[i] = &dats[4 * i];
		for (j = 0; j < 4; j++)
			ret[i][j] = -1;
	}
	k = 0;
	for (i = 0; i < 2; i++) {
		for (j = 0; j < 7; j++) {
			if (data_productiontype(i, player[p][i][j]) == 2) {
				m = 0;
				cards_getproduction(i, player[p][i][j], prod);
				for (l = 0; l < GOLD; l++) {
					if (prod[l])
						ret[k][m++] = l;
				}
				k++;
			}
		}
	}
	if (data_haswonderstage(p, 4, 2) || data_haswonderstage(p, 4, 4)) {
		m = 0;
		cards_getproduction(1, 12, prod); //production for caravansery
		for (l = 0; l < GOLD; l++) {
			if (prod[l])
				ret[k][m++] = l;
		}
		k++;
	}
	if (data_haswonderstage(p, 4, 5)) {
		m = 0;
		cards_getproduction(1, 11, prod); //production for forum
		for (l = 0; l < GOLD; l++) {
			if (prod[l])
				ret[k][m++] = l;
		}
		k++;
	}
	return ret;
}

int* data_gettradables(int p, int ret[GOLD + 1]) {
	int i, j, k;
	int prod[NUMPRODUCTS];
	for (i = 0; i < GOLD + 1; i++)
		ret[i] = 0;
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 7; j++) {
			if (player[p][i][j] != -1
					&& (cards_gettype(i, player[p][i][j]) == RESOURCE
							|| cards_gettype(i, player[p][i][j]) == INDUSTRY)) {
				cards_getproduction(i, player[p][i][j], prod);
				for (k = 0; k < GOLD; k++) {
					ret[k] += prod[k];
				}
			}
		}
	}
	ret[GOLD] = data_getgold(p);
	return ret;
}

int data_iszerocost(int *cost) {
	int i;
	int free = 1;
	for (i = 0; i < GOLD; i++)
		if (cost[i] > 0)
			free = 0;
	return free;
}

static int recurse(int *cost, int **indef, int c) {
	if (c == INDEF)
		return 0;
	int i;
	int ncost[GOLD];
	for (i = 0; i < GOLD; i++)
		ncost[i] = cost[i];
	for (i = 0; i < 4 && indef[c][i] != -1; i++) {
		ncost[indef[c][i]]--;
		if (data_iszerocost(ncost))
			return 1;
		if (recurse(ncost, indef, c + 1))
			return 1;
		ncost[indef[c][i]]++;
	}
	return 0;
}

int* data_getbuilt(int p) {
	static int ret[43];
	int type, e, c;
	int i = 0;
	for (type = 0; type <= 7; type++)
		for (e = 0; e < 3; e++)
			for (c = 0; c < 7; c++)
				if (player[p][e][c] != -1
						&& cards_gettype(e, player[p][e][c]) == type) {
					ret[i++] = e;
					ret[i++] = player[p][e][c];
				}
	ret[i] = -1; //Cap it here
	return ret;
}

int data_hasbuilt(int p, int era, int card) {
	int *built = data_getbuilt(p);
	int i;
	for (i = 0; built[i] != -1; i += 2)
		if (built[i] == era && built[i + 1] == card)
			return 1;
	return 0;
}

int data_hasbuiltname(int p, int era, int card) {
	if (data_hasbuilt(p, era, card))
		return 1;
	if (era == 1) {
		if (card == 4 && data_hasbuilt(p, 0, 10))
			return 1;
		if (card == 5 && data_hasbuilt(p, 0, 11))
			return 1;
		if (card == 6 && data_hasbuilt(p, 0, 12))
			return 1;
	}
	return 0;
}

int data_iscouponed(int p, int era, int card) {
	int coupons[4];
	cards_getcouponed(era, card, coupons);
	int i;
	for (i = 0; i < 3; i += 2) {
		if (coupons[i + 1])
			if (data_hasbuilt(p, coupons[i], coupons[i + 1]))
				return 1;
	}
	return 0;
}

int data_canafford(int p, int era, int card) {
	if (data_iscouponed(p, era, card))
		return 1;
	int cost[NUMRESOURCES];
	cards_getcost(era, card, cost);
	if (cost[GOLD] > data_getgold(p) && p != hasfreebuild)
		return 0;
	data_removedefinites(p, cost);
	data_removetraded(p, cost);
	if (data_iszerocost(cost))
		return 1;
	if (recurse(cost, data_getindefinites(p), 0))
		return 1;
	if (p == hasfreebuild)
		return 2;
	return 0;
}

int data_haswonderstage(int p, int wonder, int stage) {
	if (data_getwonder(p) != wonder)
		return 0;
	if (data_getwonderside(p) * 3 + 1 > stage)
		return 0;
	if (data_getwonderstages(p) + data_getwonderside(p) * 3 + 1 > stage)
		return 1;
	return 0;
}

int data_numbuilt(int p) {
	int num = 0;
	int i, j;
	for (i = 0; i < 3; i++)
		for (j = 0; j < 7; j++)
			if (player[p][i][j] != -1)
				num++;
	return num;
}

int data_gettotvps(int p) {
	int tot = 0;
	tot += player[p][3][3] / 3; //gold
	tot += player[p][3][4]; //military wins
	tot -= player[p][3][5]; //military defeats
	tot += player[p][3][6]; //vps
	int *built = data_getbuilt(p);
	int i;
	for (i = 0; built[i] != -1; i += 2) {
		tot += get_special(built[i], built[i + 1], p)[0];
	}
	tot += science(p);
	return tot;
}

int* data_getplayerdat(int p) {
	return player[p][3];
}

int data_potentialvps(int p, int era, int card) {
	int i;
	int prod[NUMPRODUCTS];
	cards_getproduction(era, card, prod);
	for (i = 0; player[p][era][i] != -1; i++)
		; //get free spot in array
	player[p][era][i] = card; //build card
	player[p][3][3] += prod[GOLD];
	player[p][3][3] += get_special(era, card, p)[1];
	player[p][3][6] += prod[VP];
	int vps = data_gettotvps(p);
	player[p][3][6] -= prod[VP];
	player[p][3][3] -= get_special(era, card, p)[1];
	player[p][3][3] -= prod[GOLD];
	player[p][era][i] = -1;
	return vps;
}

void data_endgame() {
	int x = 20;
	int y = 5;
	int width = 100;
	char s[width];
	io_printborder(x, y++, width);
	int tots[numplayers];
	int i, max;
	for (i = 0; i < numplayers; i++)
		tots[i] = data_gettotvps(i);
	max = 0;
	while (1) {
		for (i = 0; i < numplayers; i++) {
			if (tots[i] > tots[max])
				max = i;
		}
		if (tots[max] == -1)
			break;
		char array[40];
		sprintf(s, "vps: %-2d  %s", tots[max],
				cards_getname(data_getwonder(max), 0, array));
		y = io_printtext(x, y, width, s);
		tots[max] = -1;
	}
	io_printborder(x, y++, width);
	io_getkey();
	endgame();
	halt();
}
