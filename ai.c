#include "7w.h"

int data_canafford(int p, int era, int card);
int* data_gethand(int p);
int data_numcards(int p);
int data_getera();
int data_getnext(int p); //next recipient of the hand
int data_getdir(int dir, int p);
int data_getwonderstages(int p);
void data_discard(int p, int card);
void data_build(int p, int card);
void data_buildwonder(int p, int card);
void data_addgold(int amnt, int p);
int wonder_numstages(int player);
int weight_buildcard(int era, int card, int player);
int weight_buildwonder(int player);
int* cards_getproduction(int era, int card);
int* cards_getcost(int era, int card);
int data_hasbuiltname(int p, int era, int card);
int* get_intarray(int size);
int* ai_trade(int player, int era, int card);

int* ai_bestcard(int *hand, int player) //return card
{
 int i, temp;
 int max = 0;
 int card = 0;
 int *trade = get_intarray(3);
 static int ret[5];
 for(i = 0; hand[i] != -1 && i < 7; i++) {
  trade[0] = trade[1] = trade[2] = 0;
  temp = weight_buildcard(data_getera(), hand[i], player);
  if(data_hasbuiltname(player, data_getera(), hand[i]))
   temp = 0;
  else if(data_canafford(player, data_getera(), hand[i]) != 1) {
   trade = ai_trade(player, data_getera(), hand[i]);
   if(trade[2]) temp -= trade[2] / 3;
   else temp = 0;
  }
  if(temp > max) {
   max = temp;
   card = hand[i];
   ret[2] = trade[0];
   ret[3] = trade[1];
   ret[4] = trade[2];
  }
 }
 ret[0] = max;
 ret[1] = card;
 return ret;
}

void ai_turn(int player)
{
 int *bestcard;
 int *hand = data_gethand(player);
 int wonder = 0;
 int i;
 if(data_getwonderstages(player) < wonder_numstages(player))
  wonder = weight_buildwonder(player);
 bestcard = ai_bestcard(hand, player);
 if(bestcard[0] > wonder && bestcard[0] > 0) {
  data_build(player, bestcard[1]);
  for(i = 0; i < 2; i++) {
   data_addgold(bestcard[2+i], data_getdir(i, player));
  }
  data_addgold(bestcard[4] * -1, player);
  return;
 }
 bestcard = ai_bestcard(hand, data_getnext(player));
 if(wonder > 0) {
  data_buildwonder(player, bestcard[1]);
  return;
 }
 data_discard(player, bestcard[1]);
 data_addgold(3, player);
}
