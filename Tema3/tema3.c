// Dicu Florin - 312
// Imi pare rau dar nu am mai avut timp sa pun comentarii
#include "lib.h"
#include "utils.h"
// cauta eficient dupa id
TArb searchID(char *id, TArb g)
{
	TArb l = g->firstChild;
	while(l != NULL && strcmp(id, l->info->id) != 0)
	{
		int len = strlen(l->info->id);
		if(strncmp(id, l->info->id, len) == 0)
			l = l->firstChild;
		else
			l = l->nextSibling;
	}
	return l;
}
// verifica daca nodul respecta conditia din selector
int verifySelector(char *selector, TArb node)
{
	char auxSelector[MAX_INPUT_LEN], *p;
	strcpy(auxSelector, selector);
	if(selector[0] == '.')
	{
		p = strtok(auxSelector, ".");
		TAttr l = node->info->otherAttributes;
		for( ; l != NULL; l = l->next)
			if(strcmp(l->name, "class") == 0 && strcmp(l->value, p) == 0)
				return 1;
		return 0;
	}
	else if(strstr(selector, "."))
	{
		char tag[MAX_TAG_COMP_LEN];
		p = strtok(auxSelector, ".");
		strcpy(tag, p);
		p = strtok(NULL, ".");
		if(strcmp(node->info->type, tag) != 0)
			return 0;
		TAttr l = node->info->otherAttributes;
		for( ; l != NULL; l = l->next)
			if(strcmp(l->name, "class") == 0 && strcmp(l->value, p) == 0)
				return 1;
		return 0;
	}
	else if(strstr(selector, ">"))
	{
		char tagParent[MAX_TAG_COMP_LEN];
		p = strtok(auxSelector, ">");
		strcpy(tagParent, p);
		p = strtok(NULL, ">");
		if(strcmp(node->info->type, p) != 0)
			return 0;
		if(node->parent == NULL)
			return 0;
		if(strcmp(node->parent->info->type, tagParent) != 0)
			return 0;
		return 1;
	}
	else if(strstr(selector, " "))
	{
		char tagGrandParent[MAX_TAG_COMP_LEN];
		p = strtok(auxSelector, " ");
		strcpy(tagGrandParent, p);
		p = strtok(NULL, " ");
		if(strcmp(node->info->type, p) != 0)
			return 0;
		while(node->parent != NULL)
		{
			if(strcmp(node->parent->info->type, tagGrandParent) == 0)
				return 1;
			node = node->parent;
		}
		return 0;
	}
	else if(strcmp(node->info->type, selector) == 0)
			return 1;
	return 0;
	
}
// afiseaza codul HTML
void format(TArb g, int niv, FILE *f)
{
	if(g == NULL)
		return;
	int i;
	for(i = 0; i < niv; i++)
		fprintf(f, "	");
	fprintf(f, "<%s", g->info->type);
	if(g->info->style)
	{
		fprintf(f, " style=\"%s: %s;", g->info->style->name, g->info->style->value);
		TAttr l = g->info->style->next;
		for( ; l != NULL; l = l->next)
			fprintf(f, " %s: %s;", l->name, l->value);
		fprintf(f, "\"");
	}
	TAttr p = g->info->otherAttributes;
	for( ; p != NULL; p = p->next)
		fprintf(f, " %s=\"%s\"", p->name, p->value);
	if(g->info->isSelfClosing == 0)
	{
		fprintf(f, ">\n");
		if(strlen(g->info->contents) != 0)
		{
			for(i = 0; i < niv + 1; i++)
				fprintf(f, "	");
			fprintf(f, "%s\n", g->info->contents);
		}
		TArb l = g->firstChild;
		for( ; l != NULL; l = l->nextSibling)
			format(l, niv+1, f);
		for(i = 0; i < niv; i++)
			fprintf(f, "	");
		fprintf(f, "</%s>\n", g->info->type);
	}
	else
		fprintf(f, "/>\n");
}
// reface id-urile dupa delete
void remakeID(TArb node, int idInt)
{
	if(node == NULL)
		return;
	TArb l;
	for(l = node->firstChild, idInt = 1; l != NULL;idInt++, l = l->nextSibling)
	{
		char id[MAX_TAG_COMP_LEN];
		strcpy(l->info->id, l->parent->info->id);
		strcat(l->info->id, ".");
		sprintf(id, "%d", idInt);
		strcat(l->info->id, id);
		remakeID(l, idInt + 1);
	}
}
// elibereaza arborele
void freeTree(TArb g)
{
	if(g == NULL) return;
	freeTree(g->firstChild);
	freeTree(g->nextSibling);
	freeNode(g);
}
// sterge recursiv
void deleteRecursively(TArb node)
{
	TArb p, l = node->parent->firstChild;
	for(p = NULL ; l != node; p = l, l = l->nextSibling);
	if(p == NULL)
		l->parent->firstChild = l->nextSibling;
	else
		p->nextSibling = l->nextSibling;
	remakeID(node->parent, 1);
	freeTree(node->firstChild);
	freeNode(node);
}
//suprascrie style-ul
void overrideStyle(TArb node, char *style)
{
	char auxStyle[MAX_INPUT_LEN];
	strcpy(auxStyle, style);
	TAttr last;
	freeAttr(&node->info->style);
	char name[MAX_TAG_COMP_LEN], value[MAX_TAG_COMP_LEN], *p;
	p = strtok(auxStyle, ":;");
	while(p)
	{
		strcpy(name, p);
		p = strtok(NULL, ":;");
		strcpy(value, p);
		if(node->info->style == NULL)
		{
			node->info->style = allocTAttrCell(name, value);
			last = node->info->style;
		}
		else
		{
			last->next = allocTAttrCell(name, value);
			last = last->next;
		}
		p = strtok(NULL, ":;");
	}
}
//adauga celule noi la style
void appendStyle(TArb node, char *style)
{
	char auxStyle[MAX_INPUT_LEN];
	strcpy(auxStyle, style);
	TAttr l;
	char name[MAX_TAG_COMP_LEN], value[MAX_TAG_COMP_LEN], *p;
	p = strtok(auxStyle, ":;");
	while(p)
	{	
		strcpy(name, p);
		p = strtok(NULL, ":;");
		strcpy(value, p);
		if(node->info->style == NULL)
			node->info->style = allocTAttrCell(name, value);
		else
		{
			for(l = node->info->style; l->next != NULL; l = l->next)
				if(strcmp(l->name, name) == 0)
				{
					strcpy(l->value, value);
					break;
				}
			if(l->next == NULL && strcmp(l->name, name) == 0)
				strcpy(l->value, value);
			else if(l->next == NULL)
				l->next = allocTAttrCell(name, value);
		}
		p = strtok(NULL, ":;");
	}
}

int BFS_delete(TArb g, char *selector)
{
	void *c = InitQ(sizeof(TArb));
	void *el;
	int nrChanges = 0;
	IntrQ(c, g);
	while(ExtrQ(c, &el))
	{
		if(verifySelector(selector, (TArb)el) == 1)
		{
			deleteRecursively((TArb)el);
			nrChanges++;
		}
		else
		{
			TArb l = ((TArb)el)->firstChild;
			for( ; l != NULL; l = l->nextSibling)
				IntrQ(c, l);
		}
	}
	free(c);
	return nrChanges;
}

int BFS(TArb g, editNode f, char *style, char *selector) // s ar putea sa fie *g
{
	void *c = InitQ(sizeof(TArb));
	void *el;
	int nrChanges = 0;
	IntrQ(c, g);
	while(ExtrQ(c, &el))
	{
		TArb l = ((TArb)el)->firstChild;
		for( ; l != NULL; l = l->nextSibling)
			IntrQ(c, l);
		if(verifySelector(selector, (TArb)el) == 1)
		{
			f((TArb)el, style);
			nrChanges++;
		}
	}
	free(c);
	return nrChanges;
}

int addID(TArb g, char *id, char *tag)
{
	TArb l;
	l = searchID(id, g);
	if(l == NULL)
		return 1;
	else
		readChar(tag, &l);
	return 0;
}


int main(int argc, char const *argv[])
{
	TArb g = NULL;
	FILE *fin, *fout;
	fin = fopen(argv[1], "rt");  // fisierul din care se citeste graful
	if(!fin)
		return 1;
	fout = fopen(argv[3], "wt");  // fisierul in care se scrie graful
	if(!fout)
	{
		fclose(fin);
		return 1;
	}
	readTree(fin, &g);
	fclose(fin);
	fin = fopen(argv[2], "rt");
	int nr_lines;
	fscanf(fin, "%d", &nr_lines);
	char cmd[MAX_INPUT_LEN];
	fscanf(fin, "%s", cmd);
	if(cmd[strlen(cmd) - 1] == '\n')
		cmd[strlen(cmd) - 1] = '\0';
	while(!feof(fin))
	{
		if(strstr(cmd, "format"))
			format(g, 0, fout);
		else if(strstr(cmd, "add"))
		{
			char *tagHTML, *idParent, id[MAX_TAG_COMP_LEN], tag[MAX_TAG_COMP_LEN];
			fscanf(fin, "%s", id);
			idParent = strtok(id, "=");
			idParent = strtok(NULL, "=");
			fscanf(fin, "%s", tag);
			while(tag[strlen(tag) - 1] != '\"')
			{
				char aux[MAX_INPUT_LEN];
				fscanf(fin, "%s", aux);
				strcat(tag, aux);
			}
			tagHTML = strtok(tag, "\"");
			tagHTML = strtok(NULL, "\"");
			if(addID(g, idParent, tagHTML) == 1)
				fprintf(fout, "Add tag failed: node with id %s not found!\n", idParent);
		}
		else if(strstr(cmd, "deleteRecursively"))
		{
			int ok = 0;
			char *selector, cmdSelector[MAX_INPUT_LEN];
			fscanf(fin, "%s", cmdSelector);
			while(cmdSelector[strlen(cmdSelector) - 1] != '\"')
			{
				char aux[MAX_INPUT_LEN];
				fscanf(fin, "%s", aux);
				strcat(cmdSelector, " ");
				strcat(cmdSelector, aux);
			}
			selector = strtok(cmdSelector, "\"");
			selector = strtok(NULL, "\"");
			if(selector[0] == '#')
			{
				selector = strtok(selector, "#");
				TArb l = searchID(selector, g);
				if(l == NULL)
					fprintf(fout, "Delete recursively failed: no node found for selector %s!\n", selector);
				else
				{
					ok++;
					deleteRecursively(l);
				}
			}
			else
				ok = BFS_delete(g, selector);
			if(ok == 0)
				fprintf(fout, "Delete recursively failed: no node found for selector %s!\n", selector);
		}
		else if(strstr(cmd, "overrideStyle"))
		{
			int ok;
			char *selector, cmdSelector[MAX_INPUT_LEN];
			char *style, cmdStyle[MAX_INPUT_LEN];
			fscanf(fin, "%s", cmdSelector);
			while(cmdSelector[strlen(cmdSelector) - 1] != '\"')
			{
				char aux[MAX_INPUT_LEN];
				fscanf(fin, "%s", aux);
				strcat(cmdSelector, " ");
				strcat(cmdSelector, aux);
			}
			selector = strtok(cmdSelector, "\"");
			selector = strtok(NULL, "\"");
			fscanf(fin, "%s", cmdStyle);
			while(cmdStyle[strlen(cmdStyle) - 1] != '\"')
			{
				char aux[MAX_INPUT_LEN];
				fscanf(fin, "%s", aux);
				strcat(cmdStyle, aux);
			}
			style = strtok(cmdStyle, "\"");
			style = strtok(NULL, "\"");
			if(selector[0] == '#')
			{
				selector = strtok(selector, "#");
				TArb l = searchID(selector, g);
				if(l == NULL)
					fprintf(fout, "Override style failed: no node found for selector %s!\n", selector);
				else
					overrideStyle(l, style);
			}
			else
				ok = BFS(g, overrideStyle, style, selector);
			if(ok == 0)
				fprintf(fout, "Override style failed: no node found for selector %s!\n", selector);
		}
		else if(strstr(cmd, "appendStyle"))
		{
			int ok;
			char *selector, cmdSelector[MAX_INPUT_LEN];
			char *style, cmdStyle[MAX_INPUT_LEN];
			fscanf(fin, "%s", cmdSelector);
			while(cmdSelector[strlen(cmdSelector) - 1] != '\"')
			{
				char aux[MAX_INPUT_LEN];
				fscanf(fin, "%s", aux);
				strcat(cmdSelector, " ");
				strcat(cmdSelector, aux);
			}
			selector = strtok(cmdSelector, "\"");
			selector = strtok(NULL, "\"");
			fscanf(fin, "%s", cmdStyle);
			while(cmdStyle[strlen(cmdStyle) - 1] != '\"' || cmdStyle[strlen(cmdStyle) - 2] == '=')
			{
				char aux[MAX_INPUT_LEN];
				fscanf(fin, "%s", aux);
				strcat(cmdStyle, aux);
			}
			style = strtok(cmdStyle, "\"");
			style = strtok(NULL, "\"");
			if(selector[0] == '#')
			{
				selector = strtok(selector, "#");
				TArb l = searchID(selector, g);
				if(l == NULL)
					fprintf(fout, "Append to style failed: no node found for selector %s!\n", selector);
				else
					appendStyle(l, style);
			}
			else
				ok = BFS(g, appendStyle, style, selector);
			if(ok == 0)
				fprintf(fout, "Append to style failed: no node found for selector %s!\n", selector);
		}
		fscanf(fin, "%s", cmd);
		if(cmd[strlen(cmd) - 1] == '\n')
			cmd[strlen(cmd) - 1] = '\0';
	}
	fclose(fout);
	fclose(fin);
	freeTree(g);
	return 0;
}