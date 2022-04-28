// Dicu Florin - 312
#include "lib.h"
#include "utils.h"
/*
 *  Pentru a ușura citirea codului HTML din fișier, vi se pune la dispoziție
 *  următoarea funcție. Aceasta primeste un singur caracter (fișierul va fi
 *  citit caracter cu caracter în afara acestei funcții, iar ea va fi apelată
 *  de fiecare dată) și, în funcție de caracter, trece dintr-o stare în alta.
 *  Comentariile din cod arată o sugestie (neobligatorie) despre cum ar trebui
 *  completată funcția pentru a construi arborele. 
 * 
 *  Este, cu alte cuvinte, un automat de stări, unde starea inițială este PARSE_CONTENTS.
 */

TParseState Interpret(TArb *g, TArb aux, TArb *parent, TParseState currentState,
                      char c, char *name, char *value)
{
    TParseState nextState = PARSE_ERROR;
    switch (currentState)
    {
    case PARSE_CONTENTS:
        if (c == '<')           {   nextState = PARSE_OPENING_BRACKET;  }
        else                    {   nextState = PARSE_CONTENTS; 
                                    /* TODO append to `info->contents` string */
                                    if(c != '>' && c != '\n')
                                        strncat(aux->info->contents, &c, 1); }
        break;
    case PARSE_OPENING_BRACKET:
        if (isspace(c))         {   nextState = PARSE_OPENING_BRACKET;
          }
        else if (c == '>')      {   nextState = PARSE_ERROR;            }
        else if (c == '/')      {   nextState = PARSE_CLOSING_TAG;      }
        else                    {   nextState = PARSE_TAG_TYPE; 
                                    /* TODO create tag node */
                                    strncat(aux->info->type, &c, 1); }
        break;
    case PARSE_TAG_TYPE:
        if (isspace(c))         {   nextState = PARSE_REST_OF_TAG;      }
        else if (c == '>')      {   nextState = PARSE_CONTENTS;         
                                    aux->parent = *parent;
                                    aux->info->isSelfClosing = 0;
                                    int idInt = 1;
                                    TArb p;
                                    if(*g == NULL && *parent == NULL)
                                    {
                                        printf("ceva\n");
                                        aux->info->id = NULL;
                                        *g = aux;
                                        *parent = aux;
                                    }
                                    else if((*parent)->parent == NULL)
                                    {
                                        aux->info->id = calloc(sizeof(char), 2);
                                        char id[MAX_TAG_COMP_LEN];
                                        TArb l = aux->parent->firstChild;
                                        for(p = NULL ; l != NULL;p = l, l = l->nextSibling)
                                            idInt++;
                                        sprintf(id, "%d", idInt);
                                        strcat(aux->info->id, id);
                                        if(p == NULL)
                                            aux->parent->firstChild = aux;
                                        else
                                            p->nextSibling = aux;
                                        *parent = aux;
                                    }
                                    else
                                    {
                                        aux->info->id = calloc(sizeof(char), (strlen(aux->parent->info->id) + 4));
                                        char id[MAX_TAG_COMP_LEN];
                                        TArb l = aux->parent->firstChild;
                                        for(p = NULL ; l != NULL;p = l, l = l->nextSibling)
                                            idInt++;
                                        sprintf(id, "%d", idInt);
                                        strcpy(aux->info->id, (*parent)->info->id);
                                        strcat(aux->info->id, ".");
                                        strcat(aux->info->id, id);
                                        if(p == NULL)
                                            aux->parent->firstChild = aux;
                                        else
                                            p->nextSibling = aux;
                                        *parent = aux;
                                    } }
        else                    {   nextState = PARSE_TAG_TYPE; 
                                    /* TODO append to `info->type` string */
                                    strncat(aux->info->type, &c, 1); }
        break;
    case PARSE_CLOSING_TAG:
        if (c == '>')           {   nextState = PARSE_CONTENTS;
                                    *parent = (*parent)->parent;
                                    /* TODO insert tag node in `children` list */ }
        else                    {   nextState = PARSE_CLOSING_TAG;      }
        break;
    case PARSE_REST_OF_TAG:
        if (isspace(c))         {   nextState = PARSE_REST_OF_TAG;      }
        else if (c == '>')      {   nextState = PARSE_CONTENTS;         
                                    aux->parent = *parent;  //FACE LEGATURA
                                    aux->info->isSelfClosing = 0;
                                    int idInt = 1;
                                    TArb p;
                                    if(*g == NULL && *parent == NULL)
                                    {
                                        aux->info->id = NULL;
                                        *g = aux;
                                        *parent = aux;
                                    }
                                    else if((*parent)->parent == NULL)
                                    {
                                        aux->info->id = calloc(sizeof(char), 2);
                                        char id[MAX_TAG_COMP_LEN];
                                        TArb l = aux->parent->firstChild;
                                        for(p = NULL ; l != NULL;p = l, l = l->nextSibling)
                                            idInt++;
                                        sprintf(id, "%d", idInt);
                                        strcat(aux->info->id, id);
                                        if(p == NULL)
                                            aux->parent->firstChild = aux;
                                        else
                                            p->nextSibling = aux;
                                        *parent = aux;
                                    }
                                    else
                                    {
                                        aux->info->id = calloc(sizeof(char), (strlen(aux->parent->info->id) + 10));
                                        char id[MAX_TAG_COMP_LEN];
                                        TArb l = aux->parent->firstChild;
                                        for(p = NULL ; l != NULL;p = l, l = l->nextSibling)
                                            idInt++;
                                        strcpy(aux->info->id, (*parent)->info->id);
                                        sprintf(id, "%d", idInt);
                                        strcat(aux->info->id, ".");
                                        strcat(aux->info->id, id);
                                        if(p == NULL)
                                            aux->parent->firstChild = aux;
                                        else
                                            p->nextSibling = aux;
                                        *parent = aux;
                                    }
                                }
        else if (c == '/')      {   nextState = PARSE_SELF_CLOSING;     }
        else                    {   nextState = PARSE_ATTRIBUTE_NAME; strncat(name, &c, 1);  }
        break;
    case PARSE_ATTRIBUTE_NAME:
        if (isspace(c))         {   nextState = PARSE_ATTRIBUTE_NAME;   }
        else if (c == '=')      {   nextState = PARSE_ATTRIBUTE_EQ;     }
        else                    {   nextState = PARSE_ATTRIBUTE_NAME; 
                                    /* TODO append to a temporary "name" string */
                                    strncat(name, &c, 1); }
        break;
    case PARSE_ATTRIBUTE_EQ:
        if (c == '\"')          {   nextState = PARSE_ATTRIBUTE_VALUE;  }
        break;
    case PARSE_ATTRIBUTE_VALUE:
        if (isspace(c))         {   nextState = PARSE_ATTRIBUTE_VALUE; 
                                    if(strcmp(name, "style") != 0)
                                        strncat(value, &c, 1); }
        else if (c == '\"')     {   nextState = PARSE_REST_OF_TAG; 
                                    /* TODO create attribute from the temporary strings*/
                                    TAttr lAttr;
                                    if(strcmp(name, "style") == 0)
                                    {
                                        char *p = strtok(value, ":;");
                                        while(p)
                                        {
                                            strcpy(name, p);
                                            p = strtok(NULL, ":;");
                                            TAttr auxAttr = allocTAttrCell(name, p);
                                            if(aux->info->style == NULL)
                                                aux->info->style = auxAttr;
                                            else
                                            {
                                                for(lAttr = aux->info->style; lAttr->next != NULL; lAttr = lAttr->next);
                                                lAttr->next = auxAttr;
                                            }
                                            p = strtok(NULL, ";:");
                                        }
                                    }
                                    else
                                    {
                                        TAttr auxAttr = allocTAttrCell(name, value);
                                        if(aux->info->otherAttributes == NULL)
                                            aux->info->otherAttributes = auxAttr;
                                        else
                                        {
                                            for(lAttr = aux->info->otherAttributes; lAttr->next != NULL; lAttr = lAttr->next);
                                            lAttr->next = auxAttr;
                                        }
                                    }
                                    memset(name, 0, strlen(name));
                                    memset(value, 0, strlen(value));      }
        else                    {   nextState = PARSE_ATTRIBUTE_VALUE; 
                                    /* TODO append to a temporary "value" string */
                                    strncat(value, &c, 1); }
        break;
    case PARSE_SELF_CLOSING:
        if (c == '>')           {   nextState = PARSE_CONTENTS; 
                                    aux->info->isSelfClosing = 1;
                                    aux->parent = *parent;
                                    int idInt = 1;
                                    TArb p;
                                    if(*g == NULL && *parent == NULL)
                                    {
                                        aux->info->id = NULL;
                                        *g = aux;
                                    }
                                    else if(*parent == *g)
                                    {
                                        aux->info->id = calloc(sizeof(char), 2);
                                        char id[MAX_TAG_COMP_LEN];
                                        TArb l = aux->parent->firstChild;
                                        for(p = NULL ; l != NULL;p = l, l = l->nextSibling)
                                            idInt++;
                                        sprintf(id, "%d", idInt);
                                        strcat(aux->info->id, ".");
                                        strcat(aux->info->id, id);
                                        if(p == NULL)
                                            aux->parent->firstChild = aux;
                                        else
                                            p->nextSibling = aux;
                                    }
                                    else
                                    {
                                        aux->info->id = calloc(sizeof(char), strlen(aux->parent->info->id));
                                        char id[MAX_TAG_COMP_LEN];
                                        TArb l = aux->parent->firstChild;
                                        for(p = NULL ; l != NULL;p = l, l = l->nextSibling)
                                            idInt++;
                                        sprintf(id, "%d", idInt);
                                        strcat(aux->info->id, ".");
                                        strcat(aux->info->id, id);
                                        if(p == NULL)
                                            aux->parent->firstChild = aux;
                                        else
                                            p->nextSibling = aux;
                                    }
                                    /* TODO insert self-closing tag node in `children` list*/ }
        else                    {   nextState = PARSE_ERROR;            }
        break;
    default:
        break;
    }

    /* For debugging */

//    printf("%d", nextState);
//    if (c == '\n') printf("\n");


    return nextState;
}
