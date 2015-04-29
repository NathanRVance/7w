#include "7w.h"

int data_getdir(int dir, int p);
void data_adddefeat(int p);
void data_addvictory(int p);
int data_numplayers();
int* data_getbuilt(int p);
int data_haswonderstage(int p, int wonder, int stage);
int* cards_getproduction(int era, int card);

int military_might(int player)
{
 int *built = data_getbuilt(player);
 int i, sum;
 sum = 0;
 for(i = 0; built[i] != -1; i += 2)
  sum += cards_getproduction(built[i], built[i+1])[SHIELD];
 if(data_haswonderstage(player, 3, 2)) sum += 2;
 if(data_haswonderstage(player, 3, 4)) sum += 1;
 if(data_haswonderstage(player, 3, 5)) sum += 1;
 return sum;
}

void war()
{
 int numplayers = data_numplayers();
 int player, j;
 for(player = 0; player < numplayers; player++)
  for(j = 0; j < 2; j++) {
   if(military_might(player) > military_might(data_getdir(j, player)))
    data_addvictory(player);
   else if(military_might(player) != military_might(data_getdir(j, player)))
    data_adddefeat(player);
  }
}
