// Dicu Florin - 312
#include "lib.h"
#include "utils.h"
TAttr allocTAttrCell(char name[MAX_TAG_COMP_LEN], char value[MAX_TAG_COMP_LEN])
{
	TAttr aux = malloc(sizeof(TNodAttr));
	if(!aux)
		return NULL;
	aux->name = calloc(sizeof(char), MAX_TAG_COMP_LEN);
	if(!aux->name)
	{
		free(aux); return NULL;
	}
	aux->value = calloc(sizeof(char), MAX_TAG_COMP_LEN);
	if(!aux->value)
	{
		free(aux->name); free(aux); return NULL;
	}
	strcpy(aux->name, name);
	strcpy(aux->value, value);
	aux->next = NULL;
	return aux;
}

TArb allocTArbCell()
{
	TArb aux = malloc(sizeof(TNodArb));
	if(!aux)
		return NULL;
	aux->firstChild = NULL;
	aux->nextSibling = NULL;
	aux->info = malloc(sizeof(TNodInfo));
	if(!aux->info)
	{
		free(aux); return NULL;
	}
	aux->info->type = calloc(sizeof(char), MAX_TAG_COMP_LEN);
	if(!aux->info->type)
	{
		free(aux->info); free(aux); return NULL;
	}
	aux->info->contents = calloc(sizeof(char), MAX_TAG_COMP_LEN);
	if(!aux->info->contents)
	{
		free(aux->info->type); free(aux->info); free(aux); return NULL;
	}
	aux->parent = NULL;
	aux->info->style = NULL;
	aux->info->otherAttributes = NULL;
	return aux;
}

void readTree(FILE *f, TArb *g)
{
	char c, *name, *value;
	name = calloc(sizeof(char), MAX_TAG_COMP_LEN);
	value = calloc(sizeof(char), MAX_TAG_COMP_LEN);
	TArb aux = NULL;
	TArb parent = *g;
	TParseState currentState = 1;
	while(!feof(f))
	{
		fscanf(f, "%c", &c);
		if(currentState == 2 && !isspace(c) && c != '>' && c != '/')
			aux = allocTArbCell();
		currentState = Interpret(g, aux, &parent, currentState, c, name, value);
	}
	free(name);
	free(value);
}

void readChar(char *p, TArb *g)
{
	char *name, *value;
	name = calloc(sizeof(char), MAX_TAG_COMP_LEN);
	value = calloc(sizeof(char), MAX_TAG_COMP_LEN);
	TArb aux = NULL;
	TArb parent = *g;
	TParseState currentState = 1;
	int i;
	for(i = 0; i < strlen(p); i++)
	{
		if(currentState == 2 && !isspace(p[i]) && p[i] != '>' && p[i] != '/')
			aux = allocTArbCell();
		currentState = Interpret(g, aux, &parent, currentState, p[i], name, value);
	}
	free(name);
	free(value);
}

void freeAttr(TAttr *a)
{
	TAttr l;
	for(l = NULL; *a != NULL; l = *a, *a = (*a)->next)
		if(l)
		{
			free(l->name);
			free(l->value);
			free(l);
		}
	if(l)
	{
		free(l->name);
		free(l->value);
		free(l);
	}
}

void freeNode(TArb node)
{
	if(node->info->style)
		freeAttr(&node->info->style);
	if(node->info->otherAttributes)
		freeAttr(&node->info->otherAttributes);
	free(node->info->id);
	free(node->info->type);
	free(node->info->contents);
	free(node->info);
	free(node);
}


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