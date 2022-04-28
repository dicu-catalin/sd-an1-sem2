// DICU FLORIN CATALIN - 312CB
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#ifndef _DOWNLOAD_RESOURCE_
#define _DOWNLOAD_RESOURCE_
#define MAX_URL_NAME_LEN 50
#define MAX_CMD_NAME_LEN 256

typedef struct celula
{ void *info;
  struct celula * urm;
} TCelulaG, *TLG;

typedef struct{
    char name[100];
    unsigned long dimension;
    unsigned long currently_downloaded;
} Resource;

typedef struct{
	char url[MAX_URL_NAME_LEN];
	int num_res;
	Resource *resources;
} TWeb;

typedef struct
{ 
	size_t 	dime;
	TLG 	vf;		
} TStiva, *ASt;

typedef struct
{ 
	size_t 	dime;
	TLG 	v;
	TLG		sv;
} TCoadaV, *AQV;

typedef struct{
	TWeb *current_page;
	void *back_stack;
	void *forward_stack;
} Ttab;

typedef void (*TFree)(void *);

#define V(a) 	(((AQV)(a))->v)
#define VIDA(a) 		(V(a) == NULL)


Resource* get_page_resources(const char *URL_Name, int *n);
void ResetS(void *s, TFree DistrEl);
void DistrS(void *s, TFree DistrEl);
void *InitS(size_t d);
int Push(void *a, void *ae);
int Pop(void *a, void **ae);
void *InitQ(size_t d);
int IntrQ(void *a, void *ae);
int ExtrQ (void *a, void **ae);
int ConcatQ(void *ad, void *as);
char* get_url_web(void *a);
int verify_open_page(void *a);
#endif /* _DOWNLOAD_RESOURCE_ */
