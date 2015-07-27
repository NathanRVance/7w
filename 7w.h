#define CARDSPERERA 30

#define NUMRESOURCES 8
#define WOOD  0
#define STONE 1
#define CLAY  2
#define ORE   3
#define CLOTH 4
#define GLASS 5
#define PAPER 6
#define GOLD  7

#define NUMPRODUCTS 13 //includes resources
#define COMPASS 8
#define GEAR  9
#define TABLET 10
#define SHIELD 11
#define VP    12

#define RESOURCE 0
#define INDUSTRY 1
#define STRUCTURE 2
#define COMMERCIAL 4
#define MILITARY 5
#define SCIENTIFIC 6
#define GUILD 7

#define LEFT   0
#define RIGHT  1
#define UP     2
#define DOWN   3
#define ENTER  4

#define COLORWOOD 32
#define COLORSTONE 0
#define COLORCLAY 31
#define COLORORE 0
#define COLORCLOTH 35
#define COLORGLASS 36
#define COLORPAPER 33
#define COLORGOLD 33
#define COLORCOMPASS 0
#define COLORGEAR 0
#define COLORTABLET 31
#define COLORSHIELD 31
#define COLORVP 34

#define COLORRESOURCE 36
#define COLORINDUSTRY 0
#define COLORSTRUCTURE 34
#define COLORCOMMERCIAL 33
#define COLORMILITARY 31
#define COLORSCIENTIFIC 32
#define COLORGUILD 35

#define INDEF 8

#define RED 31
#define ESC '\1'

//Here we place all declarations:
void trade_set(int player, int trade[3][GOLD]);
int* trade_gettradables(int player, int direction, int ret[GOLD + 1]);
int data_getgold(int p);
int* data_gettradables(int p, int ret[GOLD+1]);
int get_trade(int player, int type, int direction);
int* get_science(int player, int science[3]);
int data_potentialvps(int p, int era, int card);
int data_gettotvps(int p);
int military_might(int player);
int data_getnext(int p); //next recipient of the hand
int data_getwonderstages(int p);
void data_discard(int p, int card);
void data_build(int p, int card);
int weight_buildcard(int era, int card, int player);
int* weight_buildwonder(int player, int ret[3]);
int* ai_trade(int player, int era, int card, int ret[3]);
void write_trade(int player, int tradel, int trader);
int* getdeck(int era, int numplayers, int *ret);
void shuffle(int *deck, int n);
int* trade_buffer();
void war();
void special_action(int player, int wonder, int stage);
void write_purchase(int player, int era, int card, int type);
void haltError(char *message, int num);
void endgame();
void halt();
int* data_getplayerdat(int p);
int science(int player);
int* cards_getcost(int era, int card, int ret[NUMRESOURCES]);
int gettypecolor(int type);
int* cards_getcoupons(int era, int card, int ret[4]);
int* cards_getcouponed(int era, int card, int ret[4]);
char* cards_specialmessage(int era, int card, char ret[200]);
int* get_special(int era, int card, int player);
void io_init();
void cards_init();
void data_init(int num_players);
void player_turn(int player);
void ai_turn(int player);
void data_endturn();
void data_setai(int p);
int* pregame();
int* data_gethand(int p, int ret[7]);
int data_getera();
void data_buildwonder(int p, int card);
int data_numcards(int p);
int data_canafford(int p, int era, int card);
int data_getnextwonderstage(int p);
int data_hasbuiltname(int p, int era, int card);
int wonder_numstages(int player);
int data_numbuilt(int p);
void postmessage(char* message);
void clearmessage();
int postoptions(int x, int y);
int trade_routine(int x, int y, int player);
void trade_commit(int player);
void data_spendfreebuild();
int postyn(int x, int y, char *message);
int* get_intarray(int size);
int* data_getdiscards(int ret[150]);
void data_deletediscard(int era, int card);
void posthelp();
int data_isai(int p);
void data_setfreebuild(int p);
int view_refresh(int focus, int cursor, int player, int mode);
int data_getdefeats(int p);
int data_hasbuilt(int p, int era, int card);
void data_addgold(int amnt, int p);
void io_printcolor(int x, int y, int color, char *s);
int* print_wonder(int x, int y, int player, int cursor, int ret[2]);
int print_wondersmall(int x, int y, int player, int selected, int dir);
int io_printhand(int x, int y, int player, int cursor);
int io_printcard(int x, int y, int wonder, int stage, int player);
void printmessage(int x, int y, int width);
int data_geteast(int p);
int data_getwest(int p);
int write_history(int player, int x, int y);
int data_getdir(int dir, int p);
void data_adddefeat(int p);
void data_addvictory(int p);
int data_numplayers();
int data_haswonderstage(int p, int wonder, int stage);
int* cards_getproduction(int era, int card, int ret[NUMPRODUCTS]);
char* cards_getname(int wonder, int stage, char ret[40]);
char* getname(int res);
int cards_gettype(int wonder, int stage);
int data_getwonder(int p);
int data_getwonderside(int p);
void io_printborder(int x, int y, int width);
int io_printtext(int xorigin, int y, int width, char* text);
void io_printname(int x, int y, int era, int card);
int data_getwonderstages(int p);
int data_getgold(int p);
char* cat(char a[], char b[]);
char* itoa(int i);
int io_getkey();
void io_clearscreen();
int* data_getbuilt(int p);
void io_printplain(int x, int y, char *s);
int* data_getdefinites(int p, int ret[GOLD]);
int** data_getindefinites(int p);
int data_gettotvps(int p);
char* get_chararray(int size);
void data_freebuild(int p, int e, int card);
void data_distributewonders();
int print_cards(int x, int y, int *cards, int cursor);
void Log(char *message);
int io_printdiscards(int x, int y, int player, int cursor);
