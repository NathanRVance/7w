#include "7w.h"
#include <time.h>

int* getdeck(int era, int numplayers);
int* cards_getproduction(int era, int card);
int cards_gettype(int era, int card);
int* get_intarray(int size);
void shuffle(int *deck, int n);

#define MISC 3
#define DATAGOLD 0

static int decks[3][49];
static int player[7][4][7]; //3 eras and extra stuff (wonder, wonder side, wonder stages completed, gold, military wins, defeats)
static int hands[7][7];
static int numplayers;
static int era;
static int turn;

void data_sorthands()
{
 int i, j, k, type, buffer[7];
 for(i = 0; i < numplayers; i++) {
  k = 0;
  for(type = 0; type <= 7; type++) {
   for(j = 0; j < 7; j++) {
    if(cards_gettype(era, hands[i][j]) == type) buffer[k++] = hands[i][j];
   }
  }
  for(k = 0; k < 7; k++) hands[i][k] = buffer[k];
 }
}

void data_nextera()
{
 if(era == 2) return;
 era++;
 turn = 0;
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
 era = -1;
 data_nextera();
 data_distributewonders(n);
 for(i = 0; i < n; i++)
  for(j = 0; j < 3; j++)
   for(k = 0; k < 7; k++)
    player[i][j][k] = -1;
}

void data_passturn()
{ //remember, player numbers are arranged clockwise
 if(era == 0 || era == 2) //pass to the left
  turn++;
 else  //pass to the right
  turn--;
 if(turn < 0) turn = numplayers-1;
}

int* data_gethand(int p)
{
 return hands[(p+turn)%numplayers];
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

int data_numplayers()
{
 return numplayers;
}

void data_build(int p, int card)
{
 int i;
 for(i = 0; hands[p][i] != card; i++);
 for(; i < 7; i++) hands[p][i] = hands[p][i+1];
 hands[p][7] = -1;
 for(i = 0; player[p][era][i] != -1; i++);
 player[p][era][i] = card;
}

//0 is non producing, 1 produces one kind of resource, 2 produces multiple resources
int data_productiontype(int e, int card)
{
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

void data_removedefinites(int p, int *cost)
{
 int i, j, k, *prod;
 for(i = 0; i < 3; i++) {
  for(j = 0; j < 7; j++) {
   if(data_productiontype(i, player[p][p][j]) == 1) {
    prod = cards_getproduction(i, player[p][i][j]);
    for(k = 0; k < GOLD; k++) {
     if(prod[k] && cost[k]) {
      cost[k] -= prod[k];
      if(cost[k] < 0) cost[k] = 0;
     }
    }
   }
  }
 }
}

#define INDEF 16

int* data_getindefinites(int p)
{
 int *ret = get_intarray(INDEF);
 int i, j, k;
 k = 0;
 for(i = 0; i < INDEF; i++) ret[i] = 0;
 for(i = 0; i < 2; i++) {
  for(j = 0; j < 7; j++) {
   if(data_productiontype(i, player[p][i][j]) == 2) {
    ret[k++] = i;
    ret[k++] = player[p][i][j];
   }
  }
 }
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

static int recurse(int *cost, int *indef)
{
 int i;
 int ncost[GOLD], nindef[INDEF], *prod;
 for(i = 0; i < GOLD; i++) ncost[i] = cost[i];
 for(i = 0; i < INDEF; i++) nindef[i] = indef[i];
 for(i = 0; i < INDEF && nindef[i] != 0; i += 2);
 if(i < INDEF) {
  prod = cards_getproduction(nindef[i-1], nindef[i]);
  nindef[i-1] = nindef[i] = 0;
  for(i = 0; i < GOLD; i++) {
   if(prod[i] && ncost[i]) {
    ncost[i] -= prod[i];
    if(data_iszerocost(ncost)) return 1;
    if(recurse(ncost, nindef)) return 1;
    ncost[i] += prod[i];
   }
  }
 }
 return 0;
}

//return 0 if can't afford, 1 if must buy stuff from neighbors, 2 if have all resources.
int data_canafford(int p, int *cost)
{
 if(cost[GOLD] > player[p][MISC][DATAGOLD]) return 0;
 int i, j, k;
 data_removedefinites(p, cost);
 if(data_iszerocost(cost)) return 2;
 return recurse(cost, data_getindefinites(p));
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
