#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

static void replace(char a[], char b[], int x, int y);

#define POOLSIZE 10000
static int ipool[POOLSIZE];
static int *ipoolp = ipool;
int* get_intarray(int size)
{
 if(ipoolp + size > ipool + POOLSIZE) ipoolp = ipool;
 int* ret = ipoolp;
 ipoolp += size;
 return ret;
}

static char cpool[POOLSIZE];
static char *cpoolp = cpool;
char* get_chararray(int size)
{
 if(cpoolp + size > cpool + POOLSIZE) cpoolp = cpool;
 char* ret = cpoolp;
 cpoolp += size;
 return ret;
}

/* In string a[], replace nth occerence of x[] with y[],
   or all occerences if n == 0. Note '.' is wild and
   the last . replaced will be returned. */
char util_strreplace(char a[], char x[], char y[], int n) {
 int replaceAll = ! n;
 int i, j, lengthY;
 char dot = '\0';
 for(lengthY = 0; y[lengthY] != '\0'; lengthY++);
 for(i = 0; a[i] != '\0'; i++) {
  for(j = 0;; j++) {
   if(a[i+j] != x[j] && x[j] != '\0' && x[j] != '.') // '.' is wild
    break;
   if (x[j] == '.') dot = a[i+j];
   if (x[j] == '\0') {    //it's a match
    n--;
    if(replaceAll) {      //replace it
     replace(a, y, i, j);
     i += lengthY - 1;
    }
    else if(n == 0) {     //replace it
     replace(a, y, i, j);
     return dot;
    }
    break;
   }
  if(a[i+j] == '\0') //end of string
   return dot;   
  }
 }
}

/* In string a[], replace characters x - y with b[] */
static void replace(char a[], char b[], int x, int y)
{
 int i, j;
 for(i = 0; a[i] != '\0'; i++);
 char save[i-y+1];
 for(i = x+y, j = 0; (save[j] = a[i]) != '\0'; i++, j++);
 for(i = x, j = 0; (a[i] = b[j]) != '\0'; i++, j++);
 for(i = i, j = 0; (a[i] = save[j]) != '\0'; i++, j++);
}

/* Concatinates a and b and returns a different
   string. Only use when returned string can be recycled. */
char* cat(char a[], char b[]) {
 int len = strlen(a) + strlen(b) + 1;
/* if(len > RET) return "error";
 if(len + retindex > RET) retindex = 0;
 char* p = ret + retindex;
*/
 char *p = get_chararray(len);
 int i, j;
 i = j = 0;
 while((p[i++] = a[j++]) != '\0');
 i--;
 j = 0;
 while((p[i++] = b[j++]) != '\0');
 return p;
}

char* itoa(int i)
{
 char *num = get_chararray(5);
 int j;
 for(j = 0; j < 5; num[j++] = ' ');
 num[3] = '0';
 int o = 3;
 int negative = 0;
 if(i < 0) {
  negative = 1;
  i *= -1;
 }
 while(o >= 0 && i > 0) {
  num[o--] = i % 10 + '0';
  i /= 10;
 }
 if(o >= 0 && negative) num[o] = '-';
 num[4] = '\0';
 for(i = 0; num[i] == ' '; i++);
 return &num[i];
}

//sleep in tenths of a second
int bettersleep(int ds)
{
 struct timespec tim, tim2;
 tim.tv_sec = ds/10;
 tim.tv_nsec = (ds%10)*100000000L;
 return nanosleep(&tim , &tim2);
}
