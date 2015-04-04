#include "7w.h"

/*
A brief note on why we have separated the functionality of trade.c and ai_trade.c in regards to gold transfer:
This class is designed for the sole purpose of analyzing potential transactions without commiting to any. Therefore, such handy methods as trade_commit are not used.
*/

int data_getgold(int p);
int get_trade(int player, int type, int direction);
int data_canafford(int p, int era, int card);
void trade_set(int player, int trade[3][GOLD]);
int* trade_gettradables(int player, int direction);
int data_gettotvps(int p);
int data_getdir(int dir, int p);

void ai_cleartrade(int trade[3][GOLD])
{
 int i, j;
 for(i = 0; i < 3; i++)
  for(j = 0; j < GOLD; j++)
   trade[i][j] = 0;
}

int recurse(int ret[], int cost, int trade[3][GOLD], int player, int era, int card)
{
 int i, j, k;
 if(data_canafford(player, era, card)) {
  if(cost < ret[2] || ret[2] == 0) {
   ret[2] = cost;
   ret[0] = ret[1] = 0;
   for(i = 0; i < 2; i++) {
    for(j = 0; j < GOLD; j++) {
     ret[i] += get_trade(player, j, i) * trade[i][j];
    }
   }
  }
  return 1; //if we can afford it, don't bother recursing further.
 }
 for(k = 0; k < 2; k++) {
  if(data_gettotvps(data_getdir(0, player)) > data_gettotvps(data_getdir(1, player)))
   i = 0;
  else i = 1;
  if(k) i = (i-1)*-1; //The preceeding block selects the weaker neighbor for trade preference.
  for(j = 0; j < GOLD; j++) {
   if(data_getgold(player) - cost - get_trade(player, j, i) >= 0 && trade_gettradables(player, i)[j] - trade[i][j] >= trade[i][j]+1) {
    trade[i][j]++;
    trade[2][j]++;
    trade_set(player, trade);
    if(recurse(ret, cost+get_trade(player, j, i), trade, player, era, card)) return 1;
    trade[i][j]--;
    trade[2][j]--;
   }
  }
 }
 return 0;
}

int* ai_trade(int player, int era, int card)
{
 static int ret[3]; //east gold, west gold, player gold
 int trade[3][GOLD];
 int i;
 for(i = 0; i < 3; i++)
  ret[i] = 0;
 ai_cleartrade(trade);
 int gold = data_getgold(player);
 recurse(ret, 0, trade, player, era, card);
 ai_cleartrade(trade);
 trade_set(player, trade);
 return ret;
}
