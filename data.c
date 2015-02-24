#include "7w.h"
#include <time.h>

int* getdeck(int era, int numplayers);
int* cards_getproduction(int era, int card);
int cards_gettype(int era, int card);
int* cards_getcost(int era, int card);
int* cards_getproduction(int era, int card);
int* get_intarray(int size);
void shuffle(int *deck, int n);

#define MISC 3
#define DATAGOLD 0

static int decks[3][49];
static int player[7][4][7]; //3 eras and extra stuff (wonder, wonder side, wonder stages completed, gold, military wins, defeats)
static int buffer[7][2];
static int hands[7][7];
static int numplayers;
static int era;
static int turn;
static int totturns;

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
  buffer[i][0] = -1;
  buffer[i][1] = 0;
 }
 totturns++;
 if(totturns == 6) data_nextera();
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

int data_getgold(int p)
{
 return player[p][3][3];
}

void data_addgold(int amnt, int p)
{
 buffer[p][1] += amnt;
}

int data_numplayers()
{
 return numplayers;
}

void data_discard(int p, int card)
{
 int i;
 int *hand = data_gethand(p);
 for(i = 0; hand[i] != card; i++);
 for(; i < 6; i++) hand[i] = hand[i+1];
 hand[6] = -1;
}

void data_build(int p, int card)
{
 data_discard(p, card);
 buffer[p][0] = card; //will be added to player array at end of turn
 data_addgold(cards_getcost(era, card)[GOLD] * -1, p);
 data_addgold(cards_getproduction(era, card)[GOLD], p);
}

void data_buildwonder(int p, int card)
{
 data_discard(p, card);
 buffer[p][0] = -2;
 data_addgold(cards_getcost(data_getwonder(p), data_getwonderside(p)*3+1+data_getwonderstages(p))[GOLD] * -1, p);
 data_addgold(cards_getproduction(data_getwonder(p), data_getwonderside(p)*3+1+data_getwonderstages(p))[GOLD], p);
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

int** data_getindefinites(int p)
{
 int i, j, k, l, m, *prod;
 static int *ret[INDEF];
 for(i = 0; i < INDEF; i++) {
  ret[i] = get_intarray(4);
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

int data_canafford(int p, int *cost)
{
 if(cost[GOLD] > data_getgold(p)) return 0;
 int i, j, k;
 data_removedefinites(p, cost);
 if(data_iszerocost(cost)) return 1;
 return recurse(cost, data_getindefinites(p), 0);
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

int data_numbuilt(int p)
{
 int num = 0;
 int i, j;
 for(i = 0; i < 3; i++)
  for(j = 0; j < 7; j++)
   if(player[p][i][j] != -1) num++;
 return num;
}
