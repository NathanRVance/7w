#include "7w.h"

int* getdeck(int era, int numplayers);
int* cards_getproduction(int era, int card);
int* get_intarray(int size);

#define MISC 3
#define DATAGOLD 0

static int decks[3][49];
static int player[7][4][7]; //3 eras and extra stuff (gold, military wins/defeats, etc.)
static int hands[7][7];
static int numplayers;

void data_init(int n)
{
 numplayers = n;
 int i, j, *deck;
 for(i = 0; i < 3; i++) {
  deck = getdeck(i, numplayers);
  for(j = 0; j < numplayers; j++)
   decks[i][j] = deck[j];
 }
}

void data_startera(int era)
{
 int i, j, k;
 k = 0;
 for(i = 0; i < numplayers; i++)
  for(j = 0; j < 7; j++)
   hands[i][j] = decks[era][k++];
}

//0 is non producing, 1 produces one kind of resource, 2 produces multiple resources
int data_productiontype(int era, int card)
{
 int *prod = cards_getproduction(era, card);
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
