// Dicu Florin - 312
#define MAX_TAG_COMP_LEN 1000  // lungimea maxima a unei componente din tag
#define MAX_INPUT_LEN 100 * 1024  // lungimea maxima a unei linii din fisierul de imput

typedef struct celula
{ void *info;
  struct celula * urm;
} TCelulaG, *TLG;

typedef struct
{ 
	size_t 	dime;
	TLG 	v;
	TLG		sv;
} TCoadaV, *AQV;

typedef void (*editNode)(TArb, char *);

TAttr allocTAttrCell(char name[MAX_TAG_COMP_LEN], char value[MAX_TAG_COMP_LEN]);
TArb allocTArbCell();
TParseState Interpret(TArb *g, TArb aux, TArb *parent, TParseState currentState,
                      char c, char *name, char *value);
void readTree(FILE *f, TArb *g);
TArb allocTArbCell();
TAttr allocTAttrCell(char name[MAX_TAG_COMP_LEN], char value[MAX_TAG_COMP_LEN]);
void readChar(char *p, TArb *g);
void freeNode(TArb node);
void freeAttr(TAttr *a);
void *InitQ(size_t d);
int IntrQ(void *a, void *ae);
int ExtrQ (void *a, void **ae);