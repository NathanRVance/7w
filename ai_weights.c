#include "7w.h"

int data_getdir(int dir, int p); //0 is east, 1 is west, 2 returns player
int* cards_getproduction(int era, int card);
int* cards_getcost(int era, int card);
int data_canafford(int p, int *cost);
int* data_getdefinites(int p);
int** data_getindefinites(int p);
int* data_gettradables(int p);
int data_productiontype(int e, int card);
int data_getwonderstages(int p);
int* get_special(int era, int card, int player);
int get_trade(int player, int type, int direction);
int cards_gettype(int era, int card);
int* get_science(int player);
int* get_special(int era, int card, int player);
int data_potentialvps(int p, int era, int card);
int military_might(int player);

int weight_science(int era, int card, int player)
{
 int weight = 0;
 int *science = get_science(player);
 int min = 100;
 int max = 0;
 int i, j;
 for(i = 0; i < 3; i++) {
  if(science[i] < min)
   min = science[i];
  if(science[i] > max)
   max = science[i];
 }
 if(era > 0 && max == 0) return 0;
 int *prod = cards_getproduction(era, card);
 for(i = COMPASS; prod[i] == 0; i++);
 i -= COMPASS;
 if(science[i] == min) weight = 5;
 int tots[3];
 for(i = 0; i < 3; i++) tots[i] = 0;
 for(i = 0; i < 3; i++) {
  science = get_science(data_getdir(i, player));
  for(j = 0; j < 3; j++) {
   tots[i] != science[j];
  }
 }
 if(tots[2] < tots[0]-1 || tots[2] < tots[1]-1) weight -= 3;
 return weight;
}

int weight_military(int era, int card, int player)
{
 int weight = 0;
 int military = cards_getproduction(era, card)[SHIELD];
 int strength[3];
 int i;
 for(i = 0; i < 3; i++)
  strength[i] = military_might(data_getdir(i, player));
 for(i = 0; i < 2; i++) {
  if(strength[i] >= strength[2] && strength[i] < strength[2] + military)
   weight += (era+2)*2;
  if(strength[i] == strength[2] + military)
   weight += (era+1)*2;
  if(strength[i] < strength[2])
   weight += 5 / (strength[2]-strength[i]);
 }
 return weight;
}

int weight_resource(int era, int card, int player)
{
 int weight = 0;
 int *prod = cards_getproduction(era, card);
 int *trade = data_gettradables(player);
 int i, j;
 for(i = 0; i < GOLD; i++) {
  if(prod[i] && !trade[i]) weight += 5;
  if(prod[i] && (i == STONE || i == ORE) && trade[i] < 2) weight += 2;
 }
 for(j = 0; j < 2; j++) {
  trade = data_gettradables(data_getdir(j, player));
  for(i = 0; i < GOLD; i++) {
   if(prod[i] && !trade[i]) weight++;
   if(prod[i] && trade[i] && get_trade(player, i, j) == 1) weight--;
  }
 }
 return weight;
}

int weight_tradingpost(int dir, int player);
int weight_marketplace(int player);
int weight_gold(int gold, int player);

int weight_commercial(int era, int card, int player)
{
 if(era == 0) {
  if(card == 17) return weight_gold(4, player);
  if(card == 18) return weight_tradingpost(0, player);
  if(card == 19) return weight_tradingpost(1, player);
  if(card == 20) return weight_marketplace(player);
 }
 if(era == 1) {
  if(card == 11 || card == 12) return weight_resource(era, card, player);
  if(card == 13 || card == 14) return weight_gold(get_special(era, card, player)[1], player);
 }
 return 0;
}

int weight_buildcard(int era, int card, int player)
{
 int weight = data_potentialvps(player, era, card);
 weight -= data_gettotvps(player);
 if(era == 2) return weight;
 if(cards_gettype(era, card) == SCIENTIFIC)
  weight += weight_science(era, card, player);
 if(cards_gettype(era, card) == MILITARY)
  weight += weight_military(era, card, player);
 if(cards_gettype(era, card) == RESOURCE || cards_gettype(era, card) == INDUSTRY)
  weight += weight_resource(era, card, player);
 if(cards_gettype(era, card) == COMMERCIAL)
  weight += weight_commercial(era, card, player);
}

int weight_buildwonder(int player)
{
 return (data_getwonderstages(player)+2) * 2;
}
