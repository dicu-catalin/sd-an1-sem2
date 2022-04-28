#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include "structures.h"

#define sep " "

t_db Init_DB(char db_name[MAX_DB_NAME_LEN])
{
	t_db dataB;
	strcpy(dataB.name, db_name);
	dataB.tables = malloc(sizeof(t_table));
	if(!dataB.tables){
		printf("tabelele nu s-au putut aloca");
		//return NULL;
	}
	dataB.tables = NULL;
	return dataB;
}

void freeLineINT(t_intLine *l)
{
	t_intCell *cAnt;
	while(l->cells)
	{
		cAnt = l->cells;
		l->cells = l->cells->next;
		free(cAnt);
	}
	free(l);
}

void freeLineFLOAT(t_floatLine *l)
{
	t_floatCell *cAnt;
	while(l->cells)
	{
		cAnt = l->cells;
		l->cells = l->cells->next;
		free(cAnt);
	}
	free(l);
}

void freeLineSTRING(t_stringLine *l)
{
	t_stringCell *cAnt;
	while(l->cells)
	{
		cAnt = l->cells;
		l->cells = l->cells->next;
		free(cAnt->value);
		free(cAnt);
	}
	free(l);
}

int delete_table(t_table **tables, char name_t[MAX_TABLE_NAME_LEN])
{
	t_table *t, *tAnt;
	if(!*tables)
	{
		printf("Table \"%s\" not found in database.\n", name_t);
		return 1;
	}
	for(t = *tables, tAnt = NULL; t != NULL;tAnt = t, t = t->next)
		if(strcmp(t->name, name_t) == 0)
			break;
	if(t == NULL)
	{
		printf("Table \"%s\" not found in database.\n", name_t);
		return 1;
	}
	// distrug celulele daca exista
	if(t->type == 0 && t->lines != NULL)
	{
		t_intLine *l, *lAnt;
		l = (t_intLine*)t->lines;
		for(lAnt = NULL ; l != NULL; )
		{
			lAnt = l;
			l = l->next;
			freeLineINT(lAnt);
		}
	}
	else if(t->type == 1 && t->lines != NULL)
	{
		t_floatLine *l, *lAnt;
		l = (t_floatLine*)t->lines;
		for(lAnt = NULL ; l != NULL; )
		{
			lAnt = l;
			l = l->next;
			freeLineFLOAT(lAnt);
		}
	}
	else if(t->type == 2 && t->lines != NULL)
	{
		t_stringLine *l, *lAnt;
		l = (t_stringLine*)t->lines;
		for(lAnt = NULL ; l != NULL; )
		{
			lAnt = l;
			l = l->next;
			freeLineSTRING(lAnt);
		}
	}
	t_column *colAnt;
	while(t->columns)
	{
		colAnt = t->columns;
		t->columns = t->columns->next;
		free(colAnt);
	}
	free(t->columns);
	free(t->lines);
	if(tAnt == NULL)
		*tables = (*tables)->next;
	else
		tAnt->next = t->next;
	free(t);
	return 0;
}


int createTable(t_table **table, char name_t[MAX_TABLE_NAME_LEN],
				  char type[10], char *p)
{
	t_table *ant_t, *t;
	//verifica daca tabelul exista in database
	if(*table != NULL)
		for(t = *table ; t != NULL; ant_t = t, t = t->next)
			if(strcmp(t->name, name_t) == 0)
			{
				printf("Table \"%s\" already exists.\n", name_t);
				return 1;
			}

	t_table *tAux = malloc(sizeof(t_table));
	if(!tAux)
		return 1;
	tAux->columns = malloc(sizeof(t_column));
	if(!tAux->columns)
	{
		free(tAux);
		return 1;
	}
	
	tAux->columns = NULL;
	while(p)
	{
		t_column *cAux = malloc(sizeof(t_column)), *lastColumn;
		if(!cAux)
		{
			free(tAux->columns); free(tAux); return 1;
		}
		strcpy(cAux->name, p);
		cAux->next = NULL;
		if(!tAux->columns)
			tAux->columns = cAux;
		else
			lastColumn->next = cAux;
		lastColumn = cAux;
		p = strtok(NULL, " ");
	}

	strcpy(tAux->name, name_t);
	if(*table)
		ant_t->next = tAux;
	else
		*table = tAux;
	tAux->next = NULL;

	if(strcmp(type, "STRING") == 0){
		tAux->type = STRING;
		tAux->lines = malloc(sizeof(t_stringLine));
		if(!tAux->lines)
		{
			free(tAux->columns); free(tAux); return 1;
		}
		tAux->lines = NULL;
	}

	else if(strcmp(type, "INT") == 0){
		tAux->type = INT;
		tAux->lines = malloc(sizeof(t_intLine));
		if(!tAux->lines)
		{
			free(tAux->columns); free(tAux); return 1;
		}
		tAux->lines = NULL;
	}
	else if(strcmp(type, "FLOAT") == 0){
		tAux->type = FLOAT;
		tAux->lines = malloc(sizeof(t_floatLine));
		if(!tAux->lines)
		{
			free(tAux->columns); free(tAux); return 1;
		}
		tAux->lines = NULL;
	}
	else{
		printf("Unknown data type: \"%s\".\n", type);
		delete_table(table, name_t);
		return 1;
	}

	return 1;
}


int clearTable(t_table *tables, char name_t[MAX_TABLE_NAME_LEN])
{
	t_table *t, *tAnt;
	if(!tables)
	{
		printf("Table \"%s\" not found in database.\n", name_t);
		return 1;
	}
	for(t = tables, tAnt = NULL; t != NULL;tAnt = t, t = t->next)
		if(strcmp(t->name, name_t) == 0)
			break;
	if(t == NULL)
	{
		printf("Table \"%s\" not found in database.\n", name_t);
		return 1;
	}
	// distrug celulele daca exista
	if(t->type == 0 && t->lines != NULL)
	{
		t_intLine *l, *lAnt;
		l = (t_intLine*)t->lines;
		for(lAnt = NULL ; l != NULL; )
		{
			lAnt = l;
			l = l->next;
			freeLineINT(lAnt);
		}
	}
	else if(t->type == 1 && t->lines != NULL)
	{
		t_floatLine *l, *lAnt;
		l = (t_floatLine*)t->lines;
		for(lAnt = NULL ; l != NULL; )
		{
			lAnt = l;
			l = l->next;
			freeLineFLOAT(lAnt);
		}
	}
	else if(t->type == 2 && t->lines != NULL)
	{
		t_stringLine *l, *lAnt;
		l = (t_stringLine*)t->lines;
		for(lAnt = NULL ; l != NULL; )
		{
			lAnt = l;
			l = l->next;
			freeLineSTRING(lAnt);
		}
	}
	tables->lines = NULL;
	return 0;
}

void deleteDB(t_db *dataB)
{
	t_table *tAnt, *t = (*dataB).tables;
	for(tAnt = NULL ; t != NULL; tAnt = t, t = t->next)
	{
		if(tAnt)
			free(tAnt);
		// distrug celulele daca exista
		if(t->type == 0 && t->lines != NULL)
		{
			t_intLine *l, *lAnt;
			l = (t_intLine*)t->lines;
			for(lAnt = NULL ; l != NULL; )
			{
				lAnt = l;
				l = l->next;
				freeLineINT(lAnt);
			}
		}
		else if(t->type == 1 && t->lines != NULL)
		{
			t_floatLine *l, *lAnt;
			l = (t_floatLine*)t->lines;
			for(lAnt = NULL ; l != NULL; )
			{
				lAnt = l;
				l = l->next;
				freeLineFLOAT(lAnt);
			}
		}
		else if(t->type == 2 && t->lines != NULL)
		{
			t_stringLine *l, *lAnt;
			l = (t_stringLine*)t->lines;
			for(lAnt = NULL ; l != NULL; )
			{
				lAnt = l;
				l = l->next;
				freeLineSTRING(lAnt);
			}
		}
		t_column *colAnt;
		while(t->columns)
		{
			colAnt = t->columns;
			t->columns = t->columns->next;
			free(colAnt);
		}
		free(t->columns);
		free(t->lines);
		}
	free(tAnt);
	free((*dataB).tables); 
}

int addLine(t_table **tables, char name_t[MAX_TABLE_NAME_LEN], 
			 char *p)
{
	t_table *t;
	for(t = *tables ; t != NULL; t = t->next)
		if(strcmp(t->name, name_t) == 0)
			break;
	if(t == NULL)
	{
		printf("Table \"%s\" not found in database.\n", name_t);
		return 1;
	}

	if(t->type == 0)
	{
		t_intLine *l, *lAux = malloc(sizeof(t_intLine));
		lAux->next = NULL;
		lAux->cells = NULL;
		
		while(p)
		{
			t_intCell *cAux, *cLast;
			cAux = malloc(sizeof(t_intCell));
			cAux->next = NULL;
			cAux->value = atoi(p);
			if(lAux->cells == NULL)
				lAux->cells = cAux;
			else
				cLast->next = cAux;
			cLast = cAux;
			p = strtok(NULL, " ");
		}
		if(t->lines == NULL){
			t->lines = lAux;
			return 0;
		}
		else{
			for(l = t->lines; l->next != NULL; l = l->next);
			l->next = lAux;
			return 0;
		}
	}
	else if(t->type == 1)
	{
		t_floatLine *l, *lAux = malloc(sizeof(t_floatLine));
		lAux->next = NULL;
		lAux->cells = NULL;
		
		while(p)
		{
			t_floatCell *cAux, *cLast;
			cAux = malloc(sizeof(t_floatCell));
			cAux->next = NULL;
			char val[MAX_COLUMN_NAME_LEN];
			strcpy(val, p);
			cAux->value = atof(val);
			if(lAux->cells == NULL)
				lAux->cells = cAux;
			else
				cLast->next = cAux;
			cLast = cAux;

			p = strtok(NULL, " ");
		}
		if(t->lines == NULL){
			t->lines = lAux;
			return 0;
		}
		else{
			for(l = t->lines; l->next != NULL; l = l->next);
			l->next = lAux;
			return 0;
		}
	}
	else 
	{
		t_stringLine *l, *lAux = malloc(sizeof(t_stringLine));
		lAux->next = NULL;
		lAux->cells = NULL;
		
		while(p)
		{
			t_stringCell *cAux, *cLast;
			cAux = malloc(sizeof(t_stringCell));
			cAux->value = malloc(sizeof(char) * strlen(p));
			cAux->next = NULL;
			strcpy(cAux->value, p);
			if(lAux->cells == NULL)
				lAux->cells = cAux;
			else
				cLast->next = cAux;
			cLast = cAux;

			p = strtok(NULL, " ");
		}
		if(t->lines == NULL){
			t->lines = lAux;
			return 0;
		}
		else{
			for(l = t->lines; l->next != NULL; l = l->next);
			l->next = lAux;
			return 0;
		}
	}
}

void AfiCellsINT(t_intLine *l)
{
	t_intCell *c;
	for(c = l->cells; c != NULL; c = c->next)
		printf("%-30d", c->value);
	printf("\n");
}

void AfiCellsFLOAT(t_floatLine *l)
{
	t_floatCell *c;
	for(c = l->cells; c != NULL; c = c->next)
		printf("%-30f", c->value);
	printf("\n");
}

void AfiCellsSTRING(t_stringLine *l)
{
	t_stringCell *c;
	for(c = l->cells; c != NULL; c = c->next)
		printf("%-30s", c->value);
	printf("\n");
}

void AfiLinesINT(void *lines)
{
	t_intLine *l = (t_intLine*)lines;
	for( ; l != NULL ; l = l->next)
		AfiCellsINT(l);
}

void AfiLinesFLOAT(void *lines)
{
	t_floatLine *l = (t_floatLine*)lines;
	t_floatCell *c;
	for( ; l != NULL ; l = l->next)
		AfiCellsFLOAT(l);
}

void AfiLinesSTRING(void *lines)
{
	t_stringLine *l = (t_stringLine*)lines;
	t_stringCell *c;
	for( ; l != NULL ; l = l->next)
		AfiCellsSTRING(l);
}

int print_table(t_db dataB, char name_t[MAX_COLUMN_NAME_LEN])
{
	t_table *t;
	for(t = dataB.tables; t != NULL; t = t->next)
	{
		if(strcmp(t->name, name_t) == 0){
			printf("TABLE: %s\n", t->name);
			t_column *c;
			int nr_columns = 0;
			// afisez coloanele in cazul in care exista
			for(c = t->columns; c !=NULL; c = c->next)
			{
				printf("%s", c->name);
				int i = MAX_COLUMN_NAME_LEN - strlen(c->name);
				while(i > 0)
				{
					printf(" ");
					i--;
				}
				nr_columns++;  // pastrez nr de coloane pentru padding
			}
			printf("\n");
			// padding
			for(int i = 0; i < nr_columns; i++)
			{
				int j = 1;
				while(j < MAX_COLUMN_NAME_LEN)
				{
					printf("-");
					j++;
				}
				printf(" ");
			}
			printf("\n");
			if(t->lines != NULL && t->type == 0)
				AfiLinesINT(t->lines);
			else if(t->lines != NULL && t->type == 1)
				AfiLinesFLOAT(t->lines);
			else if(t->lines != NULL && t->type == 2)
				AfiLinesSTRING(t->lines);
			return 0;
		}
	}
	printf("Table \"%s\" not found in database.\n", name_t);
	return 1;
}

int print_DB(t_db dataB)
{
	// afiseaza numele bazei de date
	printf("DATABASE: %s\n", dataB.name);
	t_table *t;
	for(t = dataB.tables; t != NULL; t = t->next)
	{
		// afiseaza tabela in cazul in care exista
		printf("TABLE: %s\n", t->name);
		t_column *c;
		int nr_columns = 0;
		// afisez coloanele in cazul in care exista
		for(c = t->columns; c !=NULL; c = c->next)
		{
			printf("%s", c->name);
			int i = MAX_COLUMN_NAME_LEN - strlen(c->name);
			while(i > 0)
			{
				printf(" ");
				i--;
			}
			nr_columns++;  // pastrez nr de coloane pentru padding
		}
		printf("\n");
		// padding
		for(int i = 0; i < nr_columns; i++)
		{
			int j = 1;
			while(j < MAX_COLUMN_NAME_LEN)
			{
				printf("-");
				j++;
			}
			printf(" ");

		}
		printf("\n");
		if(t->lines != NULL && t->type == 0)
			AfiLinesINT(t->lines);
		else if(t->lines != NULL && t->type == 1)
			AfiLinesFLOAT(t->lines);
		else if(t->lines != NULL && t->type == 2)
			AfiLinesSTRING(t->lines);
	}
	return 0;
}

int cmpValINT(t_intCell *c, int val)
{
	return c->value - val;
}

int cmpValFLOAT(t_floatCell *c, float val)
{
	return c->value - val;
}

int cmpValSTRING(t_stringCell *c, char val[MAX_COLUMN_NAME_LEN])
{
	return strcmp(c->value, val);
}

typedef void (*TF)(t_table **, void *);

void search_print(t_table **t, void *lineToPrint)
{
	if((*t)->type == 0)
	{
		t_intLine *l;
		for(l = (*t)->lines; l != lineToPrint; l = l->next);
		AfiCellsINT(l);
	}
	
	if((*t)->type == 1)
	{
		t_floatLine *l;
		for(l = (*t)->lines; l != lineToPrint; l = l->next);
		AfiCellsFLOAT(l);
	}
	
	if((*t)->type == 2)
	{
		t_stringLine *l;
		for(l = (*t)->lines; l != lineToPrint; l = l->next);
		AfiCellsSTRING(l);
	}
}

void search_delete(t_table **t, void *lineToDel)
{
	if((*t)->type == 0)
	{
		t_intLine *l, *lAnt;
		for(lAnt = NULL, l = (*t)->lines; l != lineToDel; lAnt = l, l = l->next);
		if(lAnt = NULL){
			(*t)->lines = l->next;
			freeLineINT(l);
		}
		else
		{
			lAnt->next = l->next;
			freeLineINT(l);
		}
	}
	else if((*t)->type == 1)
	{
		t_floatLine *l, *lAnt;
		for(lAnt = NULL, l = (*t)->lines; l != lineToDel; lAnt = l, l = l->next);
		if(lAnt = NULL){
			(*t)->lines = l->next;
			freeLineFLOAT(l);
		}
		else
		{
			lAnt->next = l->next;
			freeLineFLOAT(l);
		}
	}
	else if((*t)->type == 2)
	{
		t_stringLine *l, *lAnt;
		for(lAnt = NULL, l = (*t)->lines; l != lineToDel; lAnt = l, l = l->next);
		if(lAnt = NULL){
			(*t)->lines = l->next;
			freeLineSTRING(l);
		}
		else
		{
			lAnt->next = l->next;
			freeLineSTRING(l);
		}
	}
}

int search(t_db dataB, char name_t[MAX_TABLE_NAME_LEN], char rel[3], TF FSearch,
		   char name_c[MAX_COLUMN_NAME_LEN], char val[MAX_COLUMN_NAME_LEN])
{
	t_table *t;
	for(t = dataB.tables; t != NULL; t = t->next)
		if(strcmp(t->name, name_t) == 0)
			break;
	
	if(t == NULL)
	{
		printf("Table \"%s\" not found in database.\n", name_t);
		return 1;
	}

	int nr_col = 0;
	t_column *c;
	for(c = t->columns; c != NULL; c = c->next){
		if(strcmp(c->name, name_c) == 0)
			break;
		nr_col++;
	}

	if(!c)
	{
		printf("Table \"%s\" does not contain column \"%s\".\n", name_t, name_c);
		return 1;
	}

	int i;
	if(t->type == 0)
	{
		int valINT = atoi(val);
		t_intLine *l;
		t_intCell *cell;
		for(l = t->lines ; l != NULL; l = l->next){
			for(i = 0, cell = l->cells ; i < nr_col; i++, cell = cell->next);
			if((strstr(rel, "=") && cmpValINT(cell, valINT) == 0) ||
			   (strstr(rel, "<") && cmpValINT(cell, valINT) < 0) ||
			   (strstr(rel, ">") && cmpValINT(cell, valINT) > 0))
				FSearch(&t, l);
		}
	}
	else if(t->type == 1)
	{
		float valFLOAT = atof(val);
		t_floatLine *l;
		t_floatCell *cell;
		for(l = t->lines ; l != NULL; l = l->next){
			for(i = 0, cell = l->cells ; i < nr_col; i++, cell = cell->next);
			if((strstr(rel, "=") == 0 && cmpValFLOAT(cell, valFLOAT) == 0) ||
			   (strstr(rel, "<") == 0 && cmpValFLOAT(cell, valFLOAT) < 0) ||
			   (strstr(rel, ">") == 0 && cmpValFLOAT(cell, valFLOAT) > 0))
				FSearch(&t, l);
		}
	}
	else if(t->type == 2)
	{
		t_stringLine *l;
		t_stringCell *cell;
		for(l = t->lines ; l != NULL; l = l->next){
			for(i = 0, cell = l->cells ; i < nr_col; i++, cell = cell->next);
			if((strstr(rel, "=") == 0 && cmpValSTRING(cell, val) == 0) ||
			   (strstr(rel, "<") == 0 && cmpValSTRING(cell, val) < 0) ||
			   (strstr(rel, ">") == 0 && cmpValSTRING(cell, val) > 0))
				FSearch(&t, l);
		}
	}
}

int main()
{
	t_db dataB;
	char cmd[MAX_CMD_LEN], *p, cmdWrong[MAX_CMD_LEN];
	fgets(cmd, MAX_CMD_LEN, stdin);
	if(cmd[strlen(cmd) - 1] == '\n')
		cmd[strlen(cmd)-1] = '\0';
	strcpy(cmdWrong, cmd);
	while(!strstr(cmd, "DELETE_DB"))
	{
		p = strtok(cmd, " ");

		if(strstr(p, "INIT_DB"))
		{
			char name_db[MAX_DB_NAME_LEN];
			p = strtok(NULL, " ");
			strcpy(name_db, p);
			dataB = Init_DB(name_db);
		}
		else if(strstr(cmd, "DELETE"))
		{
			char name_t[MAX_TABLE_NAME_LEN];
			p = strtok(NULL, " ");
			strcpy(name_t, p);
			p = strtok(NULL, " ");
			if(!p)
				delete_table(&dataB.tables, name_t);
			else
			{
				char name_c[MAX_COLUMN_NAME_LEN], rel[3],
					 val[MAX_COLUMN_NAME_LEN];
				strcpy(name_c, p);
				p = strtok(NULL, " ");
				strcpy(rel, p);
				p = strtok(NULL, " ");
				strcpy(val, p);
				search(dataB, name_t, rel, search_delete, name_c, val);
			}
		}
		else if(strstr(p, "CREATE"))
		{
			p = strtok(NULL, " ");
			char name_t[MAX_TABLE_NAME_LEN], type[10];
			strcpy(name_t, p);
			p = strtok(NULL, " ");
			strcpy(type, p);
			p = strtok(NULL, " ");
			createTable(&dataB.tables, name_t, type, p);
		}
		else if(strstr(p, "PRINT_DB"))
			print_DB(dataB);
		else if(strcmp(p, "PRINT") == 0)
		{
			char name_t[MAX_TABLE_NAME_LEN];
			p = strtok(NULL, " ");
			strcpy(name_t, p);
			print_table(dataB, name_t);
		}
		else if(strstr(p, "SEARCH"))
		{
			char name_t[MAX_TABLE_NAME_LEN], name_c[MAX_COLUMN_NAME_LEN];
			char rel[3], val[MAX_COLUMN_NAME_LEN];
			p = strtok(NULL, " ");
			strcpy(name_t, p);
			p = strtok(NULL, " ");
			strcpy(name_c, p);
			p = strtok(NULL, " ");
			strcpy(rel, p);
			p = strtok(NULL, " ");
			strcpy(val, p);
			search(dataB, name_t, rel, search_print, name_c, val);
		}
		else if(strstr(p, "ADD"))
		{
			char name_t[MAX_TABLE_NAME_LEN];
			p = strtok(NULL, " ");
			strcpy(name_t, p);
			p = strtok(NULL, " ");
			addLine(&dataB.tables, name_t, p);
		}
		else if(strstr(p, "CLEAR"))
		{
			char name_t[MAX_TABLE_NAME_LEN];
			p = strtok(NULL, " ");
			strcpy(name_t, p);
			clearTable(dataB.tables, name_t);
		}
		else
			printf("Unknown command: \"%s\".\n", cmdWrong);
		fgets(cmd, MAX_CMD_LEN, stdin);
		if(cmd[strlen(cmd) - 1] == '\n')
			cmd[strlen(cmd)-1] = '\0';
		strcpy(cmdWrong, cmd);
	}
	deleteDB(&dataB);
}