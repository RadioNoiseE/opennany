/*
 * read in profile.xml while allowing user to specify the host server
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uchar.h>
#include <locale.h>
#include "expat.h"

#define bf 10240
#define al 36
// #define an 6
#define lw 2

char *pt="openconnect --protocol=anyconnect --quiet --background ";

int cx=0;
// int cn=an;
int ct=0;

typedef struct rs {
  char *ky;
  char *vl;
} ars;
// ars *ar = malloc(sizeof(ars) * an);
ars arr[al];
ars *ar = arr;

void XMLCALL se(void *d,const char *s,const char **a) {
  int *dp=d;
  if (*dp==2&&!strcmp(s,"HostEntry")) { cx=1; ct++; /* if (ct++==cn) ar=realloc(ar,sizeof(ars)*(cn*=2)); */ }
  else if (*dp==3&&cx==1) {
    if (!strcmp(s,"HostName")) cx=2;
    else if (!strcmp(s,"HostAddress")) cx=3;
  }
  ++*dp;
  return;
}

void XMLCALL ee(void *d,const char *s) {
  int *dp=d;
  if (*dp!=4) { cx=0; if (*dp==3&&!strcmp(s,"HostEntry")) ar++; }
  else cx=1;
  --*dp;
  return;
}

void XMLCALL ch(void *d,const XML_Char *s,int l) {
  if (cx==2) { ar->ky=malloc((l+1)*sizeof(XML_Char)); strncpy(ar->ky,s,l); cx=1; }
  else if (cx==3) { ar->vl=malloc((l+1)*sizeof(XML_Char)); strncpy(ar->vl,s,l); cx=1; }
  return;
}

int main(int ac,char **av) {
  if (ac<2) { printf("%s: no input file\n",*av); return -1; }
  if (ac>=3) { printf("%s: too many options\n",*av); return -1; }
  XML_Parser xp=XML_ParserCreate("UTF-8");
  if (!xp) { printf("xml: can't allocate memory\n"); return -1; }
  FILE *px=fopen(*++av,"r");
  if (!px) { printf("%s: can't open file\n",*av); return -1; }
  int f,d=0;
  XML_SetUserData(xp,&d);
  XML_SetElementHandler(xp,se,ee);
  XML_SetCharacterDataHandler(xp,ch);
  char b[bf];
  do {
    int l;
    f=(l=fread(b,sizeof(char),sizeof(b),px))<sizeof(b); if (f) l++;
    if (XML_Parse(xp,b,l,f)!=XML_STATUS_ERROR) { printf("xml: invalid input, error %s at line %ld\n",XML_ErrorString(XML_GetErrorCode(xp)),XML_GetCurrentLineNumber(xp)); /* return -1; */ }
  } while (!f);
  printf("%s: extracting servers\n",*--av);
  setlocale(LC_CTYPE,"en_US.UTF-8");
  while (d++<ct) {
    printf("[%*d] %*s\n",lw,d-1,wcswidth(arr[d-1].ky),arr[d-1].ky);
    free(arr[d-1].ky);
  }
  printf("Server:");
  scanf("%d",&f);
  if (f<=ct) { char *cm=malloc((strlen(pt)+strlen(arr[f].vl)*sizeof(char))); if ((d=system(strcat(strcat(cm,pt),arr[f].vl)))!=0) printf("%s: syscall failed with return code %d\n",*av,d); else printf("%s: connected\n",*av); free(cm); }
  else printf("%s: index out of range\n",*av);
  XML_ParserFree(xp);
  while (ct-->0) free(arr[ct].vl);
  // free(ar);
  return 0;
}
