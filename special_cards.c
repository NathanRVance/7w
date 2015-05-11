#include "7w.h"

int data_geteast(int p);
int data_getwest(int p);
int* data_getbuilt(int p);
int cards_gettype(int era, int card);
int data_getwonderstages(int p);
int data_getdefeats(int p);

int cardcounter(int *cards, int type)
{
 int sum = 0;
 int i;
 for(i = 0; cards[i] != -1; i+=2) {
  if(cards_gettype(cards[i], cards[i+1]) == type) sum++;
 }
 return sum;
}

int* get_special(int era, int card, int player)
{
 //formatted vp, gold, isspecial
 static int ret[3];
 ret[0] = ret[1] = 0;
 ret[2] = 1;
 switch(era*100+card) {
  case 113: //Vineyard: 1 coin for each resource card of adjacent players or your own.
   ret[1] += cardcounter(data_getbuilt(data_geteast(player)), RESOURCE);
   ret[1] += cardcounter(data_getbuilt(data_getwest(player)), RESOURCE);
   ret[1] += cardcounter(data_getbuilt(player), RESOURCE);
   break;
  case 114: //Bazaar: 2 coins for each industry card of adjacent players or your own.
   ret[1] += cardcounter(data_getbuilt(data_geteast(player)), INDUSTRY);
   ret[1] += cardcounter(data_getbuilt(data_getwest(player)), INDUSTRY);
   ret[1] += cardcounter(data_getbuilt(player), INDUSTRY);
   ret[1] *= 2;
   break;
  case 205: //Haven: 1 coin and 1 vp for each resource card.
   ret[0] = ret[1] = cardcounter(data_getbuilt(player), RESOURCE);
   break;
  case 206: //Lighthouse: 1 coin and 1 vp for each commercial card.
   ret[0] = ret[1] = cardcounter(data_getbuilt(player), COMMERCIAL);
   break;
  case 207: //Chamber Of Commerce: 2 coins and 2 vps for each industrial card.
   ret[0] = ret[1] = cardcounter(data_getbuilt(player), INDUSTRY) * 2;
   break;
  case 208: //Arena: 3 coins and 1 vp for each completed wonder stage.
   ret[0] = data_getwonderstages(player);
   ret[1] = 3*ret[0];
   break;
  case 218: //Workers Guild: 1 vp for each resource card owned by adjacent players.
   ret[0] += cardcounter(data_getbuilt(data_geteast(player)), RESOURCE);
   ret[0] += cardcounter(data_getbuilt(data_getwest(player)), RESOURCE);
   break;
  case 219: //Craftmens Guild: 2 vps for each industrial card owned by adjacent players.
   ret[0] += cardcounter(data_getbuilt(data_geteast(player)), INDUSTRY);
   ret[0] += cardcounter(data_getbuilt(data_getwest(player)), INDUSTRY);
   ret[0] *= 2;
   break;
  case 220: //Traders Guild: 1 vp for each commercial card owned by adjacent players.
   ret[0] += cardcounter(data_getbuilt(data_geteast(player)), COMMERCIAL);
   ret[0] += cardcounter(data_getbuilt(data_getwest(player)), COMMERCIAL);
   break;
  case 221: //Philosophers Guild: 1 vp for each scientific card owned by adjacent players.
   ret[0] += cardcounter(data_getbuilt(data_geteast(player)), SCIENTIFIC);
   ret[0] += cardcounter(data_getbuilt(data_getwest(player)), SCIENTIFIC);
   break;
  case 222: //Spy Guild: 1 vp for each military card owned by adjacent players.
   ret[0] += cardcounter(data_getbuilt(data_geteast(player)), MILITARY);
   ret[0] += cardcounter(data_getbuilt(data_getwest(player)), MILITARY);
   break;
  case 223: //Strategy Guild: 1 vp for each military defeat by adjacent players.
   ret[0] += data_getdefeats(data_geteast(player));
   ret[0] += data_getdefeats(data_getwest(player));
   break;
  case 224: //Shipowners Guild: 1 vp for each resource, industrial, and guild card.
   ret[0] += cardcounter(data_getbuilt(player), RESOURCE);
   ret[0] += cardcounter(data_getbuilt(player), INDUSTRY);
   ret[0] += cardcounter(data_getbuilt(player), GUILD);
   break;
  case 225: //Scientists Guild: Counts as either a compass, gear or tablet.
   ret[0] = ret[1] = 0;
   break;
  case 226: //Magistrates Guild: 1 vp for each structure card owned by adjacent players.
   ret[0] += cardcounter(data_getbuilt(data_geteast(player)), STRUCTURE);
   ret[0] += cardcounter(data_getbuilt(data_getwest(player)), STRUCTURE);
   break;
  case 227: //Builders Guild: 1 vp for each completed wonder stage by you or adjacent players.
   ret[0] += data_getwonderstages(data_geteast(player));
   ret[0] += data_getwonderstages(data_getwest(player));
   ret[0] += data_getwonderstages(player);
   break;
  default: ret[0] = ret[1] = ret[2] = 0;
   break;
 }
 return ret;
}
