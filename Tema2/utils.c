// DICU FLORIN CATALIN - 312CB
#include "utils.h"

Resource* get_page_resources(const char *URL_Name, int *n){
    int i, j, name_len, url_name_len = strlen(URL_Name);

    unsigned int resource_nr = 0;
    uint32_t size = 0;
    if(URL_Name == NULL){
        return NULL;
    }

    for(i = 0; i < url_name_len; i++){
        resource_nr += URL_Name[i];
    }

    resource_nr %= 13;
    Resource *result = (Resource *)calloc(resource_nr, sizeof(Resource));
    *n = resource_nr;
    for(i = 0; i < resource_nr; i++){
        sprintf(result[i].name, "%s-(%d)", URL_Name, i);
        size = 0;
	name_len = strlen(result[i].name);

        for(j = 0; j < name_len; j++){
            size += result[i].name[j];
        }
        /* Some randomness */
        size ^= size << 3;
        size += size >> 5;
        size ^= size << 4;
        size += size >> 17;
        size ^= size << 25;
        size += size >> 6;

        /* 100MB Maximum size */
        result[i].dimension = size % 104857601;
        result[i].currently_downloaded = 0;
    }

    return result;
}
// initializeaza stiva
void *InitS(size_t d)
{
	ASt a = (ASt)malloc(sizeof (TStiva));
	if(!a) return NULL;
	a->vf = NULL;
	a->dime = d;
	return (void *)a;
}
// introduce un element la inceputul stivei
int Push(void *a, void *ae)     
{ 
	TLG aux = malloc(sizeof(TCelulaG));
	if(!aux) 
		return 0;
	aux->urm = NULL;
	aux->info = ae;
	aux->urm = ((ASt)(a))->vf;
	((ASt)(a))->vf = aux;
  	return 1;
}
// extrage primul element din lista
int Pop(void *a, void **ae)      
{
	TLG aux;
	if(((ASt)(a))->vf == NULL) 
		return 0;

  	*ae = ((ASt)(a))->vf->info;
  	aux = ((ASt)(a))->vf;
  	((ASt)(a))->vf = ((ASt)(a))->vf->urm;
  	free(aux);
  	return 1;
}
// elibereaza toate celulele din lista stivei, iar varful va fi NULL
void ResetS(void *s, TFree DistrEl)
{
	void *el;
	while(Pop(s, &el))
		DistrEl(el);
	((ASt)(s))->vf = NULL;
}
// elibereaza toate celulele din lista stivei si stiva
void DistrS(void *s, TFree DistrEl)
{
	void *el;
	while(Pop(s, &el))
		DistrEl(el);
	free(s);
}
// initializeaza
void *InitQ(size_t d)
{
	AQV a = (AQV)malloc(sizeof(TCoadaV));
	if(!a) 
		return NULL;
	a->dime = d;
	a->v = NULL;
	a->sv = NULL;
	return (void*)a;
}
// introduce un element la sfarsitul cozii si muta sfaristul ei
int IntrQ(void *a, void *ae)   
{ 
	TLG aux = malloc(sizeof(TCelulaG));
	if(!aux)	return 0;
	aux->urm = NULL;
	aux->info = ae;
	if(((AQV)(a))->v == NULL)
	{
		((AQV)(a))->v = aux;
		((AQV)(a))->sv = aux;
	}
	else
	{
		((AQV)(a))->sv->urm = aux;
		((AQV)(a))->sv = aux;
	}
  	
  	return 1;
}
// extrage primul element din coada
int ExtrQ (void *a, void **ae)  
{ 
	TLG aux;
	if(((AQV)(a))->v == NULL) 
		return 0;
	
	*ae = ((AQV)(a))->v->info;
	aux = ((AQV)(a))->v;
	((AQV)(a))->v = ((AQV)(a))->v->urm;
	free(aux);
  	return 1;
}
// elibereaza celulele listei si coada
void DistrQ(void *c, TFree DistrEl)
{
	void *el;
	while(ExtrQ(c, &el))
	{
		DistrEl(el);
	}
	free(c);
}
// leaga la finalul unei cozi, o alta coada
int ConcatQ(void *ad, void *as)
{
	if(((AQV)(as))->v == NULL)
		return 1;
	if(((AQV)(ad))->v == NULL)
	{
		((AQV)(ad))->v = ((AQV)(as))->v;
		((AQV)(ad))->sv = ((AQV)(as))->sv;
		((AQV)(as))->v = NULL;
		((AQV)(as))->sv = NULL;
		return 1;
	}
	((AQV)(ad))->sv->urm = ((AQV)(as))->v;
	((AQV)(ad))->sv = ((AQV)(ad))->sv;
	((AQV)(as))->v = NULL;
	((AQV)(as))->sv = NULL;
	return 1;
}
// intoarce url-ul paginii deschise intr-ul tab
char* get_url_web(void *a)
{
	return ((Ttab *)a)->current_page->url;
}
// verifica daca intr-un tab este deschisa o pagina
int verify_open_page(void *a)
{
	if(((Ttab *)a)->current_page)
		return 1;
	return 0;
}