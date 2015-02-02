#include "7w.h"

char* get_chararray(int size);
int* get_intarray(int size);

//era, card#, resource cost / resource production / coupons / couponed by / card name
#define CARDSPERERA 30
static int cards[3][CARDSPERERA][60];

void cards_setcost(int era, int card, int res, int num)
{
 cards[era][card][res+1] = num;
}

int* cards_getcost(int era, int card)
{
 int *ret = get_intarray(NUMRESOURCES);
 int i;
 for(i = 0; i < NUMRESOURCES; i++) ret[i] = cards[era][card][i+1];
 return ret;
}

void cards_setproduction(int era, int card, int res, int num)
{
 cards[era][card][res+1+NUMRESOURCES] = num;
}

int* cards_getproduction(int era, int card)
{
 int *ret = get_intarray(NUMPRODUCTS);
 int i;
 for(i = 0; i < NUMPRODUCTS; i++)
  ret[i] = cards[era][card][i+1+NUMRESOURCES];
 return ret;
}

void cards_settype(int era, int card, int type)
{
 cards[era][card][0] = type;
}

int cards_gettype(int era, int card)
{
 return cards[era][card][0];
}

void cards_setcoupons(int era, int card, int era1, int card1, int era2, int card2)
{
 int i = 1+NUMRESOURCES+NUMPRODUCTS;
 cards[era][card][i++] = era1;
 cards[era][card][i++] = card1;
 cards[era][card][i++] = era2;
 cards[era][card][i++] = card2;
}

//returns the cards this card is a coupon for
int* cards_getcoupons(int era, int card)
{
 int *ret = get_intarray(4);
 int i;
 for(i = 0; i < 4; i++)
  ret[i] = cards[era][card][1+NUMRESOURCES+NUMPRODUCTS+i];
 return ret;
}

//returns cards that make this card free
int* cards_getcouponed(int era, int card)
{
 int *ret = get_intarray(4);
 int i;
 for(i = 0; i < 4; i++)
  ret[i] = cards[era][card][1+NUMRESOURCES+NUMPRODUCTS+4+i];
 return ret;
}

void cards_updatecoupons()
{
 int i, j, k;
 for(i = 0; i <= 1; i++) {
  for(j = 0; j < CARDSPERERA; j++) {
   for(k = 1+NUMRESOURCES+NUMPRODUCTS; k <= 3+NUMRESOURCES+NUMPRODUCTS; k += 2) {
    if(cards[i][j][k] != 0) {
     cards[cards[i][j][k]][cards[i][j][k+1]][k+4] = i;
     cards[cards[i][j][k]][cards[i][j][k+1]][k+5] = j;
    }
   }
  }
 }
}

void cards_setname(int era, int card, char *name, int type)
{
 int i;
 for(i = 0; (cards[era][card][i+1+NUMRESOURCES+NUMPRODUCTS+8] = name[i]) != '\0'; i++);
 cards_settype(era, card, type);
}

char* cards_getname(int era, int card)
{
 char *ret = get_chararray(26);
 int i;
 for(i = 0; (ret[i] = cards[era][card][i+1+NUMRESOURCES+NUMPRODUCTS+8]) != '\0'; i++);
 return ret;
}

char* getname(int res)
{
 switch(res) {
  case WOOD: return "wood";
  case STONE: return "stone";
  case CLAY: return "clay";
  case ORE: return "ore";
  case CLOTH: return "cloth";
  case GLASS: return "glass";
  case PAPER: return "paper";
  case GOLD: return "gold";
  case COMPASS: return "compass";
  case GEAR: return "gear";
  case TABLET: return "tablet";
  case SHIELD: return "shield";
  case VP: return "vp";
  default: return "error";
 }
}

void cards_init()
{
 //Era 1
 cards_setproduction(0, 0, WOOD, 1);
 cards_setname(0, 0, "Lumber Yard", RESOURCE);

 cards_setproduction(0, 1, STONE, 1);
 cards_setname(0, 1, "Stone Pit", RESOURCE);

 cards_setproduction(0, 2, CLAY, 1);
 cards_setname(0, 2, "Clay Pool", RESOURCE);

 cards_setproduction(0, 3, ORE, 1);
 cards_setname(0, 3, "Ore Vein", RESOURCE);

 cards_setcost(0, 4, GOLD, 1);
 cards_setproduction(0, 4, WOOD, 1);
 cards_setproduction(0, 4, CLAY, 1);
 cards_setname(0, 4, "Tree Farm", RESOURCE);

 cards_setcost(0, 5, GOLD, 1);
 cards_setproduction(0, 5, STONE, 1);
 cards_setproduction(0, 5, CLAY, 1);
 cards_setname(0, 5, "Excavation", RESOURCE);

 cards_setcost(0, 6, GOLD, 1);
 cards_setproduction(0, 6, CLAY, 1);
 cards_setproduction(0, 6, ORE, 1);
 cards_setname(0, 6, "Clay Pit", RESOURCE);

 cards_setcost(0, 7, GOLD, 1);
 cards_setproduction(0, 7, STONE, 1);
 cards_setproduction(0, 7, WOOD, 1);
 cards_setname(0, 7, "Timber Yard", RESOURCE);

 cards_setcost(0, 8, GOLD, 1);
 cards_setproduction(0, 8, WOOD, 1);
 cards_setproduction(0, 8, ORE, 1);
 cards_setname(0, 8, "Forest Cave", RESOURCE);

 cards_setcost(0, 9, GOLD, 1);
 cards_setproduction(0, 9, ORE, 1);
 cards_setproduction(0, 9, STONE, 1);
 cards_setname(0, 9, "Mine", RESOURCE);

 cards_setproduction(0, 10, CLOTH, 1);
 cards_setname(0, 10, "Loom", INDUSTRY);

 cards_setproduction(0, 11, GLASS, 1);
 cards_setname(0, 11, "Glassworks", INDUSTRY);

 cards_setproduction(0, 12, PAPER, 1);
 cards_setname(0, 12, "Press", INDUSTRY);

 cards_setproduction(0, 13, VP, 3);
 cards_setname(0, 13, "Pawnshop", STRUCTURE);

 cards_setcost(0, 14, STONE, 1);
 cards_setproduction(0, 14, VP, 3);
 cards_setcoupons(0, 14, 1, 7, 0, 0);
 cards_setname(0, 14, "Baths", STRUCTURE);

 cards_setproduction(0, 15, VP, 2);
 cards_setcoupons(0, 15, 1, 8, 0, 0);
 cards_setname(0, 15, "Altar", STRUCTURE);

 cards_setproduction(0, 16, VP, 2);
 cards_setcoupons(0, 16, 1, 9, 0, 0);
 cards_setname(0, 16, "Theater", STRUCTURE);

 cards_setproduction(0, 17, GOLD, 5);
 cards_setname(0, 17, "Tavern", COMMERCIAL);

 cards_setcoupons(0, 18, 1, 11, 0, 0);
 cards_setname(0, 18, "East Trading Post", COMMERCIAL);

 cards_setcoupons(0, 19, 1, 11, 0, 0);
 cards_setname(0, 19, "West Trading Post", COMMERCIAL);

 cards_setcoupons(0, 20, 1, 12, 0, 0);
 cards_setname(0, 20, "Marketplace", COMMERCIAL);

 cards_setcost(0, 21, WOOD, 1);
 cards_setproduction(0, 21, SHIELD, 1);
 cards_setname(0, 21, "Stockade", MILITARY);

 cards_setcost(0, 22, ORE, 1);
 cards_setproduction(0, 22, SHIELD, 1);
 cards_setname(0, 22, "Barracks", MILITARY);

 cards_setcost(0, 23, CLAY, 1);
 cards_setproduction(0, 23, SHIELD, 1);
 cards_setname(0, 23, "Guard Tower", MILITARY);

 cards_setcost(0, 24, CLOTH, 1);
 cards_setproduction(0, 24, COMPASS, 1);
 cards_setcoupons(0, 24, 1, 17, 1, 19);
 cards_setname(0, 24, "Apothecary", SCIENTIFIC);

 cards_setcost(0, 25, GLASS, 1);
 cards_setproduction(0, 25, GEAR, 1);
 cards_setcoupons(0, 25, 1, 20, 1, 18);
 cards_setname(0, 25, "Workshop", SCIENTIFIC);

 cards_setcost(0, 26, PAPER, 1);
 cards_setproduction(0, 26, TABLET, 1);
 cards_setcoupons(0, 26, 1, 10, 1, 21);
 cards_setname(0, 26, "Scriptorium", SCIENTIFIC);

 //Era 2
 cards_setcost(1, 0, GOLD, 1);
 cards_setproduction(1, 0, WOOD, 2);
 cards_setname(1, 0, "Sawmill", RESOURCE);

 cards_setcost(1, 1, GOLD, 1);
 cards_setproduction(1, 1, STONE, 2);
 cards_setname(1, 1, "Quarry", RESOURCE);

 cards_setcost(1, 2, GOLD, 1);
 cards_setproduction(1, 2, CLAY, 2);
 cards_setname(1, 2, "Brickyard", RESOURCE);

 cards_setcost(1, 3, GOLD, 1);
 cards_setproduction(1, 3, ORE, 2);
 cards_setname(1, 3, "Foundry", RESOURCE);

 cards_setproduction(1, 4, CLOTH, 1);
 cards_setname(1, 4, "Loom", INDUSTRY);

 cards_setproduction(1, 5, GLASS, 1);
 cards_setname(1, 5, "Glassworks", INDUSTRY);

 cards_setproduction(1, 6, PAPER, 1);
 cards_setname(1, 6, "Press", INDUSTRY);

 cards_setcost(1, 7, STONE, 3);
 cards_setproduction(1, 7, VP, 5);
 cards_setname(1, 7, "Aqueduct", STRUCTURE);

 cards_setcost(1, 8, WOOD, 1);
 cards_setcost(1, 8, CLAY, 1);
 cards_setcost(1, 8, GLASS, 1);
 cards_setproduction(1, 8, VP, 3);
 cards_setcoupons(1, 8, 2, 0, 0, 0);
 cards_setname(1, 8, "Temple", STRUCTURE);

 cards_setcost(1, 9, ORE, 2);
 cards_setcost(1, 9, WOOD, 1);
 cards_setproduction(1, 9, VP, 4);
 cards_setcoupons(1, 9, 2, 1, 0, 0);
 cards_setname(1, 9, "Statue", STRUCTURE);

 cards_setcost(1, 10, CLAY, 2);
 cards_setcost(1, 10, CLOTH, 1);
 cards_setproduction(1, 10, VP, 4);
 cards_setname(1, 10, "Courthouse", STRUCTURE);

 cards_setcost(1, 11, CLAY, 2);
 cards_setproduction(1, 11, CLOTH, 1);
 cards_setproduction(1, 11, GLASS, 1);
 cards_setproduction(1, 11, PAPER, 1);
 cards_setcoupons(1, 11, 2, 5, 0, 0);
 cards_setname(1, 11, "Forum", COMMERCIAL);

 cards_setcost(1, 12, WOOD, 2);
 cards_setproduction(1, 12, CLAY, 1);
 cards_setproduction(1, 12, STONE, 1);
 cards_setproduction(1, 12, ORE, 1);
 cards_setproduction(1, 12, WOOD, 1);
 cards_setcoupons(1, 12, 2, 6, 0, 0);
 cards_setname(1, 12, "Caravansery", COMMERCIAL);

 cards_setname(1, 13, "Vineyard", COMMERCIAL);

 cards_setname(1, 14, "Bazar", COMMERCIAL);

 cards_setcost(1, 15, STONE, 3);
 cards_setproduction(1, 15, SHIELD, 2);
 cards_setcoupons(1, 15, 2, 9, 0, 0);
 cards_setname(1, 15, "Walls", MILITARY);

 cards_setcost(1, 16, ORE, 2);
 cards_setcost(1, 16, WOOD, 1);
 cards_setproduction(1, 16, SHIELD, 2);
 cards_setcoupons(1, 16, 2, 10, 0, 0);
 cards_setname(1, 16, "Training Ground", MILITARY);

 cards_setcost(1, 17, ORE, 1);
 cards_setcost(1, 17, CLAY, 1);
 cards_setcost(1, 17, WOOD, 1);
 cards_setproduction(1, 17, SHIELD, 2);
 cards_setname(1, 17, "Stables", MILITARY);

 cards_setcost(1, 18, ORE, 1);
 cards_setcost(1, 18, WOOD, 2);
 cards_setproduction(1, 18, SHIELD, 2);
 cards_setname(1, 18, "Archery Range", MILITARY);

 cards_setcost(1, 19, GLASS, 1);
 cards_setcost(1, 19, ORE, 2);
 cards_setproduction(1, 19, COMPASS, 1);
 cards_setcoupons(1, 19, 2, 13, 2, 8);
 cards_setname(1, 19, "Dispensary", SCIENTIFIC);

 cards_setcost(1, 20, PAPER, 1);
 cards_setcost(1, 20, CLAY, 2);
 cards_setproduction(1, 20, GEAR, 1);
 cards_setcoupons(1, 20, 2, 14, 2, 12);
 cards_setname(1, 20, "Laboratory", SCIENTIFIC);

 cards_setcost(1, 21, CLOTH, 1);
 cards_setcost(1, 21, STONE, 2);
 cards_setproduction(1, 21, TABLET, 1);
 cards_setcoupons(1, 21, 2, 4, 2, 15);
 cards_setname(1, 21, "Library", SCIENTIFIC);

 cards_setcost(1, 22, PAPER, 1);
 cards_setcost(1, 22, WOOD, 1);
 cards_setproduction(1, 22, TABLET, 1);
 cards_setcoupons(1, 22, 2, 16, 2, 17);
 cards_setname(1, 22, "School", SCIENTIFIC);

 //Era 3
 cards_setcost(2, 0, CLAY, 2);
 cards_setcost(2, 0, ORE, 1);
 cards_setcost(2, 0, PAPER, 1);
 cards_setcost(2, 0, CLOTH, 1);
 cards_setcost(2, 0, GLASS, 1);
 cards_setproduction(2, 0, VP, 7);
 cards_setname(2, 0, "Pantheon", STRUCTURE);

 cards_setcost(2, 1, CLAY, 2);
 cards_setcost(2, 1, WOOD, 1);
 cards_setproduction(2, 1, VP, 5);
 cards_setname(2, 1, "Gardens", STRUCTURE);

 cards_setcost(2, 2, STONE, 2);
 cards_setcost(2, 2, ORE, 1);
 cards_setcost(2, 2, GLASS, 1);
 cards_setproduction(2, 2, VP, 6);
 cards_setname(2, 2, "Town Hall", STRUCTURE);

 cards_setcost(2, 3, WOOD, 1);
 cards_setcost(2, 3, ORE, 1);
 cards_setcost(2, 3, CLAY, 1);
 cards_setcost(2, 3, STONE, 1);
 cards_setcost(2, 3, GLASS, 1);
 cards_setcost(2, 3, PAPER, 1);
 cards_setcost(2, 3, CLOTH, 1);
 cards_setproduction(2, 3, VP, 8);
 cards_setname(2, 3, "Palace", STRUCTURE);

 cards_setcost(2, 4, WOOD, 2);
 cards_setcost(2, 4, ORE, 1);
 cards_setcost(2, 4, STONE, 1);
 cards_setproduction(2, 4, VP, 6);
 cards_setname(2, 4, "Senate", STRUCTURE);

 cards_setcost(2, 5, ORE, 1);
 cards_setcost(2, 5, WOOD, 1);
 cards_setcost(2, 5, CLOTH, 1);
 cards_setname(2, 5, "Haven", COMMERCIAL);

 cards_setcost(2, 6, STONE, 1);
 cards_setcost(2, 6, GLASS, 1);
 cards_setname(2, 6, "Lighthouse", COMMERCIAL);

 cards_setcost(2, 7, CLAY, 2);
 cards_setcost(2, 7, PAPER, 1);
 cards_setname(2, 7, "Chamber Of Commerce", COMMERCIAL);

 cards_setcost(2, 8, STONE, 2);
 cards_setcost(2, 8, ORE, 1);
 cards_setname(2, 8, "Arena", COMMERCIAL);

 cards_setcost(2, 9, ORE, 3);
 cards_setcost(2, 9, STONE, 1);
 cards_setproduction(2, 9, SHIELD, 3);
 cards_setname(2, 9, "Fortifications", MILITARY);

 cards_setcost(2, 10, STONE, 3);
 cards_setcost(2, 10, ORE, 1);
 cards_setproduction(2, 10, SHIELD, 3);
 cards_setname(2, 10, "Circus", MILITARY);

 cards_setcost(2, 11, ORE, 1);
 cards_setcost(2, 11, WOOD, 2);
 cards_setcost(2, 11, CLOTH, 1);
 cards_setproduction(2, 11, SHIELD, 3);
 cards_setname(2, 11, "Arsenal", MILITARY);

 cards_setcost(2, 12, CLAY, 3);
 cards_setcost(2, 12, WOOD, 1);
 cards_setproduction(2, 12, SHIELD, 3);
 cards_setname(2, 12, "Siege Workshop", MILITARY);

 cards_setcost(2, 13, CLAY, 2);
 cards_setcost(2, 13, CLOTH, 1);
 cards_setcost(2, 13, PAPER, 1);
 cards_setproduction(2, 13, COMPASS, 1);
 cards_setname(2, 13, "Lodge", SCIENTIFIC);

 cards_setcost(2, 14, ORE, 2);
 cards_setcost(2, 14, GLASS, 1);
 cards_setcost(2, 14, CLOTH, 1);
 cards_setproduction(2, 14, GEAR, 1);
 cards_setname(2, 14, "Observatory", SCIENTIFIC);

 cards_setcost(2, 15, PAPER, 1);
 cards_setcost(2, 15, GLASS, 1);
 cards_setcost(2, 15, WOOD, 2);
 cards_setproduction(2, 15, TABLET, 1);
 cards_setname(2, 15, "University", SCIENTIFIC);

 cards_setcost(2, 16, GLASS, 1);
 cards_setcost(2, 16, STONE, 3);
 cards_setproduction(2, 16, COMPASS, 1);
 cards_setname(2, 16, "Academy", SCIENTIFIC);

 cards_setcost(2, 17, WOOD, 1);
 cards_setcost(2, 17, PAPER, 1);
 cards_setcost(2, 17, CLOTH, 1);
 cards_setproduction(2, 17, GEAR, 1);
 cards_setname(2, 17, "Study", SCIENTIFIC);

 cards_setcost(2, 18, ORE, 2);
 cards_setcost(2, 18, CLAY, 1);
 cards_setcost(2, 18, STONE, 1);
 cards_setcost(2, 18, WOOD, 1);
 cards_setname(2, 18, "Workers Guild", GUILD);

 cards_setcost(2, 19, ORE, 2);
 cards_setcost(2, 19, STONE, 2);
 cards_setname(2, 19, "Craftmens Guild", GUILD);

 cards_setcost(2, 20, CLOTH, 1);
 cards_setcost(2, 20, PAPER, 1);
 cards_setcost(2, 20, GLASS, 1);
 cards_setname(2, 20, "Traders Guild", GUILD);

 cards_setcost(2, 21, CLAY, 3);
 cards_setcost(2, 21, CLOTH, 1);
 cards_setcost(2, 21, PAPER, 1);
 cards_setname(2, 21, "Philosophers Guild", GUILD);

 cards_setcost(2, 22, CLAY, 3);
 cards_setcost(2, 22, GLASS, 1);
 cards_setname(2, 22, "Spy Guild", GUILD);

 cards_setcost(2, 23, ORE, 2);
 cards_setcost(2, 23, STONE, 1);
 cards_setcost(2, 23, CLOTH, 1);
 cards_setname(2, 23, "Strategy Guild", GUILD);

 cards_setcost(2, 24, WOOD, 3);
 cards_setcost(2, 24, PAPER, 1);
 cards_setcost(2, 24, GLASS, 1);
 cards_setname(2, 24, "Shipowners Guild", GUILD);

 cards_setcost(2, 25, WOOD, 2);
 cards_setcost(2, 25, ORE, 2);
 cards_setcost(2, 25, CLOTH, 1);
 cards_setname(2, 25, "Scientists Guild", GUILD);

 cards_setcost(2, 26, WOOD, 3);
 cards_setcost(2, 26, STONE, 1);
 cards_setcost(2, 26, CLOTH, 1);
 cards_setname(2, 26, "Magistrates Guild", GUILD);

 cards_setcost(2, 27, STONE, 2);
 cards_setcost(2, 27, CLAY, 2);
 cards_setcost(2, 27, GLASS, 1);
 cards_setname(2, 27, "Builders Guild", GUILD);

 cards_updatecoupons();
}
