#include "7w.h"
#include <time.h>
#include <stdio.h>

int* getdeck(int era, int numplayers);
int* cards_getproduction(int era, int card);
int cards_gettype(int era, int card);
int* cards_getcost(int era, int card);
int* cards_getproduction(int era, int card);
int* cards_getcouponed(int era, int card);
int* get_intarray(int size);
void shuffle(int *deck, int n);
int* trade_buffer();
int* get_special(int era, int card, int player);
void war();
int science(int player);
void halt();
char* cards_getname(int wonder, int stage);
void io_printborder(int x, int y, int width);
int io_printtext(int xorigin, int y, int width, char* text);
int io_getkey();
int data_haswonderstage(int p, int wonder, int stage);

#define MISC 3
#define DATAGOLD 0

static int decks[3][49];
static int discards[3][49];
static int player[7][4][7]; //3 eras and extra stuff (wonder, wonder side, wonder stages completed, gold, military wins, defeats, vps(partial))
static int buffer[7][3];
static int hands[7][7];
static int numplayers;
static int era;
static int turn;
static int totturns;
static int turngoldbuffer[7];

void data_sorthands()
{
 int i, j, k, type, buff[7];
 for(i = 0; i < numplayers; i++) {
  k = 0;
  for(type = 0; type <= 7; type++) {
   for(j = 0; j < 7; j++) {
    if(cards_gettype(era, hands[i][j]) == type) buff[k++] = hands[i][j];
   }
  }
  for(k = 0; k < 7; k++) hands[i][k] = buff[k];
 }
}

void data_endgame();

void data_nextera()
{
 war();
 if(era == 2) data_endgame();
 era++;
 turn = 0;
 totturns = 0;
 int i, j, k;
 k = 0;
 for(i = 0; i < numplayers; i++)
  for(j = 0; j < 7; j++)
   hands[i][j] = decks[era][k++];
 data_sorthands();
}

void data_distributewonders()
{
 int i;
 int wonders[7];
 for(i = 0; i < 7; i++)
  wonders[i] = i+3; //wonders are numbered 3-9
 shuffle(wonders, 7);
 for(i = 0; i < numplayers; i++) {
  player[i][3][0] = wonders[i];
  player[i][3][1] = rand()%2;
 }
}

void data_init(int n)
{
 numplayers = n;
 int i, j, k, *deck;
 for(i = 0; i < 3; i++) {
  deck = getdeck(i, numplayers);
  for(j = 0; j < numplayers*7; j++)
   decks[i][j] = deck[j];
 }
 for(i = 0; i < 3; i++)
  for(j = 0; j < 49; j++)
   discards[i][j] = -1;
 era = -1;
 data_nextera();
 data_distributewonders(n);
 for(i = 0; i < n; i++) {
  for(j = 0; j < 3; j++)
   for(k = 0; k < 7; k++)
    player[i][j][k] = -1;
  player[i][3][3] = 2; //start with 2 gold
  buffer[i][0] = -1;
 }
}

void data_endturn()
{ //remember, player numbers are arranged clockwise
 if(era == 0 || era == 2) //pass to the left
  turn++;
 else  //pass to the right
  turn--;
 if(turn < 0) turn = numplayers-1;
 int i, j;
 for(i = 0; i < numplayers; i++) {
  if(buffer[i][0] == -2) {
   player[i][3][2]++;
  } else {
   for(j = 0; player[i][era][j] != -1; j++); //get free spot in array
   player[i][era][j] = buffer[i][0]; //build card
  }
  player[i][3][3] += buffer[i][1]; //change in gold
  player[i][3][6] += buffer[i][2];
  buffer[i][0] = -1;
  buffer[i][1] = 0;
  buffer[i][2] = 0;
 }
 totturns++;
 if(totturns == 6) data_nextera();
}

int data_geteast(int p)
{
 int ret = p-1;
 if(ret < 0) ret = numplayers-1;
}

int data_getwest(int p)
{
 return (p+1)%numplayers;
}

int data_getdir(int dir, int p)
{
 if(dir == 0) return data_geteast(p);
 if(dir == 1) return data_getwest(p);
 return p;
}

int data_getnext(int p)
{
 if(era == 0 || era == 2) return data_getwest(p);
 return data_geteast(p);
}

int* data_gethand(int p)
{
 int *ret = get_intarray(7);
 int i;
 for(i = 0; i < 7; i++)
  ret[i] =  hands[(p+turn)%numplayers][i];
 return ret;
}

int data_numcards(int p)
{
 int *hand = data_gethand(p);
 int numcards;
 for(numcards = 0; numcards < 7 && hand[numcards] > -1; numcards++);
 return numcards;
}

int data_getera()
{
 return era;
}

int data_getwonder(int p)
{
 return player[p][3][0];
}

int data_getwonderside(int p)
{
 return player[p][3][1];
}

int data_getwonderstages(int p)
{
 return player[p][3][2];
}

int data_getnextwonderstage(int p)
{
 int side = data_getwonderside(p);
 int stage = data_getwonderstages(p);
 int wonder = data_getwonder(p);
 if(side == 0) {
  if(stage >= 3) return -1;
 } else {
  if(wonder == 3 && stage >= 2) return -1;
  if(wonder != 9 && stage >= 3) return -1;
  if(wonder == 9 && stage >= 4) return -1;
 }
 return side*3+1+stage;
}

int data_getdefeats(int p)
{
 return player[p][3][5];
}

void data_adddefeat(int p)
{
 player[p][3][5]++;
}

void data_addvictory(int p)
{
 int amnt;
 if(era == 0) amnt = 1;
 if(era == 1) amnt = 3;
 if(era == 2) amnt = 5;
 player[p][3][4] += amnt;
}

int data_getgold(int p)
{
 return player[p][3][3] + turngoldbuffer[p];
}

void data_addgold(int amnt, int p)
{
 buffer[p][1] += amnt;
 turngoldbuffer[p] += amnt;
}

void data_addvps(int amnt, int p)
{
 buffer[p][2] += amnt;
}

int data_numplayers()
{
 return numplayers;
}

int** data_getdiscards()
{
 static int **ret;
 int i;
 for(i = 0; i < 3; i++)
  ret[i] = discards[i];
 return ret;
}

void data_deletediscard(int era, int card)
{
 int i;
 for(i = 0; discards[era][i] != card && i < 48; i++);
 discards[era][i] = -1;
}

void data_remove(int p, int card) //as in, removes it from the hand
{
 int i;
 int *hand = hands[(p+turn)%numplayers];
 for(i = 0; hand[i] != card; i++);
 for(; i < 6; i++) hand[i] = hand[i+1];
 hand[6] = -1;
 for(i = 0; i < 7; i++) turngoldbuffer[i] = 0; //end of turn
}

void data_discard(int p, int card)
{
 data_remove(p, card);
 int i;
 for(i = 0; discards[era][i] != -1 && i < 49; i++);
 discards[era][i] = card;
}

void data_build(int p, int card)
{
 buffer[p][0] = card; //will be added to player array at end of turn
 data_addgold(cards_getcost(era, card)[GOLD] * -1, p);
 data_addgold(cards_getproduction(era, card)[GOLD], p);
 data_addvps(cards_getproduction(era, card)[VP], p);
 data_addgold(get_special(era, card, p)[1], p);
 data_remove(p, card);
}

void data_buildwonder(int p, int card)
{
 buffer[p][0] = -2;
 data_addgold(cards_getcost(data_getwonder(p), data_getwonderside(p)*3+1+data_getwonderstages(p))[GOLD] * -1, p);
 data_addgold(cards_getproduction(data_getwonder(p), data_getwonderside(p)*3+1+data_getwonderstages(p))[GOLD], p);
 data_addvps(cards_getproduction(data_getwonder(p), data_getwonderside(p)*3+1+data_getwonderstages(p))[VP], p);
 data_remove(p, card);
}

//0 is non producing, 1 produces one kind of resource, 2 produces multiple resources
int data_productiontype(int e, int card)
{
 if(e < 0 || card < 0) return 0;
 int *prod = cards_getproduction(e, card);
 int i;
 int type = 0;
 for(i = 0; i < GOLD; i++) {
  if(prod[i]) {
   if(type == 1) type = 2;
   if(type == 0) type = 1;
  }
 }
 return type;
}

int* data_getdefinites(int p)
{
 int *ret = get_intarray(GOLD);
 int i, j, k, *prod;
 for(i = 0; i < GOLD; i++) ret[i] = 0;
 for(i = 0; i < 3; i++) {
  for(j = 0; j < 7; j++) {
   if(data_productiontype(i, player[p][i][j]) == 1) {
    prod = cards_getproduction(i, player[p][i][j]);
    for(k = 0; k < GOLD; k++) {
     ret[k] += prod[k];
    }
   }
  }
 }
 return ret;
}

void data_removedefinites(int p, int *cost)
{
 cost[cards_gettype(data_getwonder(p), 0)]--;
 int *prod = data_getdefinites(p);
 int i;
 for(i = 0; i < GOLD; i++) {
  cost[i] -= prod[i];
  if(cost[i] < 0) cost[i] = 0;
 }
}

void data_removetraded(int p, int *cost)
{
 int *trade = trade_buffer();
 int i;
 for(i = 0; i < GOLD; i++) {
  cost[i] -= trade[i];
  if(cost[i] < 0) cost[i] = 0;
 }
}

int** data_getindefinites(int p)
{
 int i, j, k, l, m, *prod;
 static int dats[4*INDEF];
 static int *ret[INDEF];
 for(i = 0; i < INDEF; i++) {
  //ret[i] = get_intarray(4);
  ret[i] = &dats[4*i];
  for(j = 0; j < 4; j++)
   ret[i][j] = -1;
 }
 k = 0;
 for(i = 0; i < 2; i++) {
  for(j = 0; j < 7; j++) {
   if(data_productiontype(i, player[p][i][j]) == 2) {
    m = 0;
    prod = cards_getproduction(i, player[p][i][j]);
    for(l = 0; l < GOLD; l++) {
     if(prod[l]) ret[k][m++] = l;
    }
    k++;
   }
  }
 }
 if(data_haswonderstage(p, 4, 2) || data_haswonderstage(p, 4, 4)) {
  m = 0;
  prod = cards_getproduction(1, 12); //production for caravansery
  for(l = 0; l < GOLD; l++) {
   if(prod[l]) ret[k][m++] = l;
  }
  k++;
 }
 if(data_haswonderstage(p, 4, 5)) {
  m = 0;
  prod = cards_getproduction(1, 11); //production for forum
  for(l = 0; l < GOLD; l++) {
   if(prod[l]) ret[k][m++] = l;
  }
  k++;
 }
 return ret;
}

int* data_gettradables(int p)
{
 int *ret = get_intarray(GOLD+1);
 int i, j, k, *prod;
 for(i = 0; i < GOLD+1; i++) ret[i] = 0;
 for(i = 0; i < 3; i++) {
  for(j = 0; j < 7; j++) {
   if(player[p][i][j] != -1 && (cards_gettype(i, player[p][i][j]) == RESOURCE || cards_gettype(i, player[p][i][j]) == INDUSTRY)) {
    prod = cards_getproduction(i, player[p][i][j]);
    for(k = 0; k < GOLD; k++) {
     ret[k] += prod[k];
    }
   }
  }
 }
 ret[GOLD] = data_getgold(p);
 return ret;
}

int data_iszerocost(int *cost)
{
 int i;
 int free = 1;
 for(i = 0; i < GOLD; i++)
  if(cost[i] > 0) free = 0;
 return free;
}

static int recurse(int *cost, int **indef, int c)
{
 if(c == INDEF) return 0;
 int i;
 int ncost[GOLD];
 for(i = 0; i < GOLD; i++) ncost[i] = cost[i];
 for(i = 0; i < 4 && indef[c][i] != -1; i++) {
  ncost[indef[c][i]]--;
  if(data_iszerocost(ncost)) return 1;
  if(recurse(ncost, indef, c+1)) return 1;
  ncost[indef[c][i]]++;
 }
 return 0;
}

int* data_getbuilt(int p)
{
 int *ret = get_intarray(43);
 int type, e, c;
 int i = 0;
 for(type = 0; type <= 7; type++)
  for(e = 0; e < 3; e++)
   for(c = 0; c < 7; c++)
    if(player[p][e][c] != -1 && cards_gettype(e, player[p][e][c]) == type) {
     ret[i++] = e;
     ret[i++] = player[p][e][c];
    }
 ret[i] = -1; //Cap it here
 return ret;
}

int data_hasbuilt(int p, int era, int card)
{
 int *built = data_getbuilt(p);
 int i;
 for(i = 0; built[i] != -1; i += 2)
  if(built[i] == era && built[i+1] == card) return 1;
 return 0;
}

int data_hasbuiltname(int p, int era, int card)
{
 if(data_hasbuilt(p, era, card)) return 1;
 if(era == 1) {
  if(card == 4 && data_hasbuilt(p, 0, 10)) return 1;
  if(card == 5 && data_hasbuilt(p, 0, 11)) return 1;
  if(card == 6 && data_hasbuilt(p, 0, 12)) return 1;
 }
 return 0;
}

int data_iscouponed(int p, int era, int card)
{
 int* coupons = cards_getcouponed(era, card);
 int i;
 for(i = 0; i < 3; i += 2) {
  if(coupons[i+1])
   if(data_hasbuilt(p, coupons[i], coupons[i+1])) return 1;
 }
}

int data_canafford(int p, int era, int card)
{
 if(data_iscouponed(p, era, card)) return 1;
 int *cost = cards_getcost(era, card);
 if(cost[GOLD] > data_getgold(p)) return 0;
 int i, j, k;
 data_removedefinites(p, cost);
 data_removetraded(p, cost);
 if(data_iszerocost(cost)) return 1;
 return recurse(cost, data_getindefinites(p), 0);
}

int data_haswonderstage(int p, int wonder, int stage)
{
 if(data_getwonder(p) != wonder) return 0;
 if(data_getwonderside(p)*3+1 > stage) return 0;
 if(data_getwonderstages(p)+data_getwonderside(p)*3+1 > stage) return 1;
 return 0;
}

int data_numbuilt(int p)
{
 int num = 0;
 int i, j;
 for(i = 0; i < 3; i++)
  for(j = 0; j < 7; j++)
   if(player[p][i][j] != -1) num++;
 return num;
}

int data_gettotvps(int p)
{
 int tot = 0;
 tot += player[p][3][3] / 3; //gold
 tot += player[p][3][4]; //military wins
 tot -= player[p][3][5]; //military defeats
 tot += player[p][3][6]; //vps
 int *built = data_getbuilt(p);
 int i;
 for(i = 0; built[i] != -1; i += 2) {
  tot += get_special(built[i], built[i+1], p)[0];
 }
 tot += science(p);
 return tot;
}

int data_potentialvps(int p, int era, int card)
{
 int i;
 for(i = 0; player[p][era][i] != -1; i++); //get free spot in array
 player[p][era][i] = card; //build card
 player[p][3][3] += cards_getproduction(era, card)[GOLD];
 player[p][3][3] += get_special(era, card, p)[1];
 player[p][3][6] += cards_getproduction(era, card)[VP], p;
 int vps = data_gettotvps(p);
 player[p][3][6] -= cards_getproduction(era, card)[VP], p;
 player[p][3][3] -= get_special(era, card, p)[1];
 player[p][3][3] -= cards_getproduction(era, card)[GOLD];
 player[p][era][i] = -1;
 return vps;
}

void data_endgame()
{
 int x = 20;
 int y = 5;
 int width = 100;
 char s[width];
 io_printborder(x, y++, width);
 int tots[numplayers];
 int i, max;
 for(i = 0; i < numplayers; i++) tots[i] = data_gettotvps(i);
 max = 0;
 while(1) {
  for(i = 0; i < numplayers; i++) {
   if(tots[i] > tots[max]) max = i;
  }
  if(tots[max] == -1) break;
  sprintf(s, "vps: %-2d  %s", tots[max], cards_getname(data_getwonder(max), 0));
  y = io_printtext(x, y, width, s);
  tots[max] = -1;
 }
 io_printborder(x, y++, width);
 io_getkey();
 halt();
}
