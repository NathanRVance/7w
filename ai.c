#include "7w.h"

int data_canafford(int p, int *cost);
int* data_gethand(int p);
int data_numcards(int p);
int data_getera();
int weight_buildcard(int era, int card, int player);
int weight_buildwonder(int player);
int* cards_getproduction(int era, int card);

void ai_turn(int player)
{
 int i, temp;
 int *hand = data_gethand(player);
 int numcards = data_numpcards(player);
 int max = 0;
 int card = 0;
 for(i = 0; i < numcards; i++) {
  temp = weight_buildcard(data_getera(), hand[i], player);
  if(temp > max ) {
   max = temp;
   card = hand[i];
  }
 }
}
