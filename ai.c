#include "7w.h"

int data_canafford(int p, int era, int card);
int* data_gethand(int p);
int data_numcards(int p);
int data_getera();
int data_getnext(int p); //next recipient of the hand
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

int* ai_bestcard(int *hand, int player) //return card
{
 int i, temp;
 int max = 0;
 int card = 0;
 for(i = 0; hand[i] != -1 && i < 7; i++) {
  temp = weight_buildcard(data_getera(), hand[i], player);
  if(! data_canafford(player, data_getera(), hand[i]))
   temp = 0;
  if(temp > max) {
   max = temp;
   card = hand[i];
  }
 }
 static int ret[2];
 ret[0] = max;
 ret[1] = card;
 return ret;
}

void ai_turn(int player)
{
 int *bestcard;
 int *hand = data_gethand(player);
 int wonder = 0;
 if(data_getwonderstages(player) < wonder_numstages(player))
  wonder = weight_buildwonder(player);
 bestcard = ai_bestcard(hand, player);
 if(bestcard[0] > wonder && bestcard[0] > 0) {
  data_build(player, bestcard[1]);
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
