#include "7w.h"

int data_getgold(int p);
int get_trade(int player, int type, int direction);
int data_canafford(int p, int era, int card);
void trade_clear(int player);

int ai_trade(int player, int *cost)
{
 while(data_getgold(player) > 1) {
  //finish trade_set and this
 }
}
