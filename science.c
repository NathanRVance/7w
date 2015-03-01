#include "7w.h"

int* data_getbuilt(int p);
int* cards_getproduction(int era, int card);
int cards_gettype(int era, int card);
int data_hasbuilt(int p, int era, int card);
int* get_intarray(int size);

int* get_science(int player)
{
 int *science = get_intarray(3);
 int i, j;
 for(i = 0; i < 3; i++) science[i] = 0;
 int *built = data_getbuilt(player);
 int *prod;
 for(i = 0; built[i] != -1; i += 2) {
  if(cards_gettype(built[i], built[i+1]) == SCIENTIFIC) {
   prod = cards_getproduction(built[i], built[i+1]);
   for(j = COMPASS; j <= TABLET; j++) {
    science[j-COMPASS] += prod[j];
   }
  }
 }
 return science;
}

int calc_science(int player, int guildval)
{
 int i;
 int *science = get_science(player);
 if(guildval != -1) science[guildval]++;
 int min = science[0];
 for(i = 0; i < 3; i++)
  if(science[i] < min) min = science[i];
 int ret = 7*min;
 for(i = 0; i < 3; i++) ret += science[i] * science[i];
 return ret;
}

int science(int player)
{
 if(! data_hasbuilt(player, 2, 25)) return calc_science(player, -1);
 int max = 0;
 int i, m;
 for(i = 0; i < 3; i++) {
  m = calc_science(player, i);
  if(m > max) max = m;
 }
 return max;
}
