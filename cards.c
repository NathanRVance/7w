#include "7w.h"

#define STARTCOST 1
#define STARTPRODUCTION 1+NUMRESOURCES
#define STARTCOUPONS 1+NUMRESOURCES+NUMPRODUCTS
#define STARTCOUPONED 1+NUMRESOURCES+NUMPRODUCTS+4
#define STARTNAME 1+NUMRESOURCES+NUMPRODUCTS+8
#define STARTDESCRIPTION STARTNAME+25

char* get_chararray(int size);
int* get_intarray(int size);

//era, card#, resource cost / resource production / coupons / couponed by / card name / description
static int cards[10][CARDSPERERA][300];
//Final 7 'eras' are actually the wonders.

void cards_setcost(int era, int card, int res, int num)
{
 cards[era][card][STARTCOST+res] = num;
}

int* cards_getcost(int era, int card)
{
 int *ret = get_intarray(NUMRESOURCES);
 int i;
 for(i = 0; i < NUMRESOURCES; i++) ret[i] = cards[era][card][STARTCOST+i];
 return ret;
}

void cards_setproduction(int era, int card, int res, int num)
{
 cards[era][card][STARTPRODUCTION+res] = num;
}

int* cards_getproduction(int era, int card)
{
 int *ret = get_intarray(NUMPRODUCTS);
 int i;
 for(i = 0; i < NUMPRODUCTS; i++)
  ret[i] = cards[era][card][STARTPRODUCTION+i];
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
 int i = STARTCOUPONS;
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
  ret[i] = cards[era][card][STARTCOUPONS+i];
 return ret;
}

//returns cards that make this card free
int* cards_getcouponed(int era, int card)
{
 int *ret = get_intarray(4);
 int i;
 for(i = 0; i < 4; i++)
  ret[i] = cards[era][card][STARTCOUPONED+i];
 return ret;
}

void cards_updatecoupons()
{
 int i, j, k;
 int* card;
 for(i = 0; i <= 1; i++) {
  for(j = 0; j < 27-4*i; j++) {
   for(k = STARTCOUPONS; k <= STARTCOUPONS+2; k += 2) {
    if(cards[i][j][k]) {
     card = cards[cards[i][j][k]][cards[i][j][k+1]];
     if(card[STARTCOUPONED+1]) {
      card[STARTCOUPONED+2] = i;
      card[STARTCOUPONED+3] = j;
     } else {
      card[STARTCOUPONED+0] = i;
      card[STARTCOUPONED+1] = j;
     }
    }
   }
  }
 }
}

void cards_setname(int era, int card, char *name, int type)
{
 int i;
 for(i = 0; (cards[era][card][STARTNAME+i] = name[i]) != '\0'; i++);
 cards_settype(era, card, type);
}

char* cards_getname(int era, int card)
{
 char *ret = get_chararray(40);
 int i;
 for(i = 0; (ret[i] = cards[era][card][STARTNAME+i]) != '\0'; i++);
 return ret;
}

void cards_setmessage(int era, int card, char *message)
{
 int i;
 for(i = 0; (cards[era][card][STARTDESCRIPTION+i] = message[i]) != '\0'; i++);
}

char* cards_specialmessage(int era, int card)
{
 char *ret = get_chararray(200);
 int i;
 for(i = 0; (ret[i] = cards[era][card][STARTDESCRIPTION+i]) != '\0'; i++);
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
 cards_setmessage(0, 18, "Can trade 1 coin for resources with player to the east.");
 cards_setname(0, 18, "East Trading Post", COMMERCIAL);

 cards_setcoupons(0, 19, 1, 11, 0, 0);
 cards_setmessage(0, 19, "Can trade 1 coin for resources with player to the west.");
 cards_setname(0, 19, "West Trading Post", COMMERCIAL);

 cards_setcoupons(0, 20, 1, 12, 0, 0);
 cards_setmessage(0, 20, "Can trade 1 coin for industry products with adjacent players.");
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

 cards_setmessage(1, 13, "1 coin for each resource card of adjacent players or your own.");
 cards_setname(1, 13, "Vineyard", COMMERCIAL);

 cards_setmessage(1, 14, "2 coins for each industry card of adjacent players or your own.");
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
 cards_setmessage(2, 7, "1 coin and 1 vp for each resource card.");
 cards_setname(2, 5, "Haven", COMMERCIAL);

 cards_setcost(2, 6, STONE, 1);
 cards_setcost(2, 6, GLASS, 1);
 cards_setmessage(2, 7, "1 coin and 1 vp for each commercial card.");
 cards_setname(2, 6, "Lighthouse", COMMERCIAL);

 cards_setcost(2, 7, CLAY, 2);
 cards_setcost(2, 7, PAPER, 1);
 cards_setmessage(2, 7, "2 coins and 2 vps for each industrial card.");
 cards_setname(2, 7, "Chamber Of Commerce", COMMERCIAL);

 cards_setcost(2, 8, STONE, 2);
 cards_setcost(2, 8, ORE, 1);
 cards_setmessage(2, 8, "3 coins and 1 vp for each completed wonder stage.");
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
 cards_setmessage(2, 18, "1 vp for each resource card owned by adjacent players.");
 cards_setname(2, 18, "Workers Guild", GUILD);

 cards_setcost(2, 19, ORE, 2);
 cards_setcost(2, 19, STONE, 2);
 cards_setmessage(2, 19, "2 vps for each industrial card owned by adjacent players.");
 cards_setname(2, 19, "Craftmens Guild", GUILD);

 cards_setcost(2, 20, CLOTH, 1);
 cards_setcost(2, 20, PAPER, 1);
 cards_setcost(2, 20, GLASS, 1);
 cards_setmessage(2, 20, "1 vp for each commercial card owned by adjacent players.");
 cards_setname(2, 20, "Traders Guild", GUILD);

 cards_setcost(2, 21, CLAY, 3);
 cards_setcost(2, 21, CLOTH, 1);
 cards_setcost(2, 21, PAPER, 1);
 cards_setmessage(2, 21, "1 vp for each scientific card owned by adjacent players.");
 cards_setname(2, 21, "Philosophers Guild", GUILD);

 cards_setcost(2, 22, CLAY, 3);
 cards_setcost(2, 22, GLASS, 1);
 cards_setmessage(2, 22, "1 vp for each military card owned by adjacent players.");
 cards_setname(2, 22, "Spy Guild", GUILD);

 cards_setcost(2, 23, ORE, 2);
 cards_setcost(2, 23, STONE, 1);
 cards_setcost(2, 23, CLOTH, 1);
 cards_setmessage(2, 23, "1 vp for each military defeat by adjacent players.");
 cards_setname(2, 23, "Strategy Guild", GUILD);

 cards_setcost(2, 24, WOOD, 3);
 cards_setcost(2, 24, PAPER, 1);
 cards_setcost(2, 24, GLASS, 1);
 cards_setmessage(2, 24, "1 vp for each resource, industrial, and guild card.");
 cards_setname(2, 24, "Shipowners Guild", GUILD);

 cards_setcost(2, 25, WOOD, 2);
 cards_setcost(2, 25, ORE, 2);
 cards_setcost(2, 25, CLOTH, 1);
 cards_setmessage(2, 25, "Counts as either a compass, gear or tablet.");
 cards_setname(2, 25, "Scientists Guild", GUILD);

 cards_setcost(2, 26, WOOD, 3);
 cards_setcost(2, 26, STONE, 1);
 cards_setcost(2, 26, CLOTH, 1);
 cards_setmessage(2, 26, "1 vp for each structure card.");
 cards_setname(2, 26, "Magistrates Guild", GUILD);

 cards_setcost(2, 27, STONE, 2);
 cards_setcost(2, 27, CLAY, 2);
 cards_setcost(2, 27, GLASS, 1);
 cards_setmessage(2, 27, "1 vp for each completed wonder stage by you or adjacent players.");
 cards_setname(2, 27, "Builders Guild", GUILD);

 cards_updatecoupons();

 cards_setname(3, 0, "The Colossus of Rhodes", ORE);
 cards_setcost(3, 1, WOOD, 2);
 cards_setproduction(3, 1, VP, 3);
 cards_setcost(3, 2, CLAY, 3);
 cards_setproduction(3, 2, SHIELD, 2);
 cards_setcost(3, 3, ORE, 4);
 cards_setproduction(3, 3, VP, 7);
 cards_setcost(3, 4, STONE, 3);
 cards_setproduction(3, 4, SHIELD, 1);
 cards_setproduction(3, 4, VP, 3);
 cards_setproduction(3, 4, GOLD, 3);
 cards_setcost(3, 5, ORE, 4);
 cards_setproduction(3, 5, SHIELD, 1);
 cards_setproduction(3, 5, VP, 4);
 cards_setproduction(3, 5, GOLD, 4);
 
 cards_setname(4, 0, "The Lighthouse of Alexandria", GLASS);
 cards_setcost(4, 1, STONE, 2);
 cards_setproduction(4, 1, VP, 3);
 cards_setcost(4, 2, ORE, 2);
 cards_setproduction(4, 2, CLAY, 1);
 cards_setproduction(4, 2, ORE, 1);
 cards_setproduction(4, 2, WOOD, 1);
 cards_setproduction(4, 2, STONE, 1);
 cards_setcost(4, 3, GLASS, 2);
 cards_setproduction(4, 3, VP, 7);
 cards_setcost(4, 4, CLAY, 2);
 cards_setproduction(4, 4, WOOD, 1);
 cards_setproduction(4, 4, STONE, 1);
 cards_setproduction(4, 4, ORE, 1);
 cards_setproduction(4, 4, CLAY, 1);
 cards_setcost(4, 5, WOOD, 2);
 cards_setproduction(4, 5, GLASS, 1);
 cards_setproduction(4, 5, CLOTH, 1);
 cards_setproduction(4, 5, PAPER, 1);
 cards_setcost(4, 6, STONE, 3);
 cards_setproduction(4, 6, VP, 7);
 
 cards_setname(5, 0, "The Temple of Artemis in Ephesus", PAPER);
 cards_setcost(5, 1, STONE, 2);
 cards_setproduction(5, 1, VP, 3);
 cards_setcost(5, 2, WOOD, 2);
 cards_setproduction(5, 2, GOLD, 9);
 cards_setcost(5, 3, PAPER, 2);
 cards_setproduction(5, 3, VP, 7);
 cards_setcost(5, 4, STONE, 2);
 cards_setproduction(5, 4, VP, 2);
 cards_setproduction(5, 4, GOLD, 4);
 cards_setcost(5, 5, WOOD, 2);
 cards_setproduction(5, 5, VP, 3);
 cards_setproduction(5, 5, GOLD, 4);
 cards_setcost(5, 6, GLASS, 1);
 cards_setcost(5, 6, CLOTH, 1);
 cards_setcost(5, 6, PAPER, 1);
 cards_setproduction(5, 6, VP, 5);
 cards_setproduction(5, 6, GOLD, 4);

 cards_setname(6, 0, "The Hanging Gardens of Babylon", CLAY);
 cards_setcost(6, 1, CLAY, 2);
 cards_setproduction(6, 1, VP, 3);
 cards_setcost(6, 2, WOOD, 3);
 cards_setproduction(6, 2, TABLET, 1);
 cards_setproduction(6, 2, COMPASS, 1);
 cards_setproduction(6, 2, GEAR, 1);
 cards_setcost(6, 3, CLAY, 4);
 cards_setproduction(6, 3, VP, 7);
 cards_setcost(6, 4, CLAY, 1);
 cards_setcost(6, 4, CLOTH, 1);
 cards_setproduction(6, 4, VP, 3);
 cards_setcost(6, 5, WOOD, 2);
 cards_setcost(6, 5, GLASS, 1);
 cards_setmessage(6, 5, "Can now play 7th age card rather than discarding it.");
 cards_setcost(6, 6, CLAY, 3);
 cards_setcost(6, 6, PAPER, 1);
 cards_setproduction(6, 6, TABLET, 1);
 cards_setproduction(6, 6, COMPASS, 1);
 cards_setproduction(6, 6, GEAR, 1);

 cards_setname(7, 0, "The Statue of Zeus in Olympia", WOOD);
 cards_setcost(7, 1, WOOD, 2);
 cards_setproduction(7, 1, VP, 3);
 cards_setcost(7, 2, STONE, 2);
 cards_setmessage(7, 2, "Once per age can build a card for free.");
 cards_setcost(7, 3, ORE, 2);
 cards_setproduction(7, 3, VP, 7);
 cards_setcost(7, 4, WOOD, 2);
 cards_setmessage(7, 4, "Can trade 1 coin for resources with adjacent players.");
 cards_setcost(7, 5, STONE, 2);
 cards_setproduction(7, 5, VP, 5);
 cards_setcost(7, 6, ORE, 2);
 cards_setcost(7, 6, CLOTH, 1);
 cards_setmessage(7, 6, "Can copy one Guild card build by an adjacent player.");

 cards_setname(8, 0, "The Mausoleum of Halicarnassus", CLOTH);
 cards_setcost(8, 1, CLAY, 2);
 cards_setproduction(8, 1, VP, 3);
 cards_setcost(8, 2, ORE, 3);
 cards_setmessage(8, 2, "Can look through all discards since the beginning of the game, pick one, and build it for free.");
 cards_setcost(8, 3, CLOTH, 2);
 cards_setproduction(8, 3, VP, 7);
 cards_setcost(8, 4, ORE, 2);
 cards_setproduction(8, 4, VP, 2);
 cards_setmessage(8, 4, "Can look through all discards since the beginning of the game, pick one, and build it for free.");
 cards_setcost(8, 5, CLAY, 3);
 cards_setproduction(8, 5, VP, 1);
 cards_setmessage(8, 5, "Can look through all discards since the beginning of the game, pick one, and build it for free.");
 cards_setcost(8, 6, CLOTH, 1);
 cards_setcost(8, 6, PAPER, 1);
 cards_setcost(8, 6, GLASS, 1);
 cards_setmessage(8, 6, "Can look through all discards since the beginning of the game, pick one, and build it for free.");

 cards_setname(9, 0, "The Pyramids of Giza", STONE);
 cards_setcost(9, 1, STONE, 2);
 cards_setproduction(9, 1, VP, 3);
 cards_setcost(9, 2, WOOD, 3);
 cards_setproduction(9, 2, VP, 5);
 cards_setcost(9, 3, STONE, 4);
 cards_setproduction(9, 3, VP, 7);
 cards_setcost(9, 4, WOOD, 2);
 cards_setproduction(9, 4, VP, 3);
 cards_setcost(9, 5, STONE, 3);
 cards_setproduction(9, 5, VP, 5);
 cards_setcost(9, 6, CLAY, 3);
 cards_setproduction(9, 6, VP, 5);
 cards_setcost(9, 7, STONE, 4);
 cards_setcost(9, 7, PAPER, 1);
 cards_setproduction(9, 7, VP, 7);
 
}
