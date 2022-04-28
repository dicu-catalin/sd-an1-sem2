// DICU FLORIN CATALIN - 312CB
#include "utils.h"
// eibereaza memoria unei pagini web
void DistrWeb(void *el)
{
	TWeb *aux = (TWeb *)el;
	free(aux->resources);
	free(aux);
}
// elibereaza memoria unui tab(stiva back, forward si current page)
void DistrTab(void *el)
{
	Ttab *aux = (Ttab *)el;
	void *ae;
	while(ExtrQ(aux->back_stack, &ae))
		DistrWeb(ae);
	free(aux->back_stack);
	while(ExtrQ(aux->forward_stack, &ae))
		DistrWeb(ae);
	free(aux->forward_stack);
	if(aux->current_page != NULL)
		DistrWeb(aux->current_page);
	free(aux);
}
// elibereaza memoria pentru toate taburile deschise
void DistrAllTabs(TLG *tabs)
{
	TLG l, ant;
	for(l = *tabs, ant = NULL; l != NULL; ant = l, l = l->urm)
		if(ant){
			DistrTab(ant->info);
			free(ant);
		}
	if(ant)
	{
		DistrTab(ant->info);
		free(ant);
	}
	*tabs = NULL;
}
// distruge lista downloadarilor finalizate
void DistrDownloaded(TLG *downloaded)
{
	TLG l, ant;
	for(l = *downloaded, ant = NULL; l != NULL; ant = l, l = l->urm)
		if(ant)
		{
			free(ant->info);
			free(ant);
		}
	if(ant)
	{
		free(ant->info);
		free(ant);	
	}
	*downloaded = NULL;
}

// deschide un nou tab
void newtab(TLG *tabs, Ttab **current_tab)
{
	TLG l, aux;
	// aloc celula care va fi adaugata in lista
	aux = malloc(sizeof(TCelulaG));
	if(!aux) return;
	// aloc tabul si stivele de back si forward
	Ttab *infoTab = malloc(sizeof(Ttab));
	infoTab->current_page = NULL;
	infoTab->back_stack = InitS(sizeof(TWeb));
	infoTab->forward_stack = InitS(sizeof(TWeb));
	*current_tab = infoTab;
	// leg celula de celelalte celule care contin tab-uri
	aux->info = infoTab;
	aux->urm = NULL;
	if(*tabs == NULL)
		*tabs = aux;
	else
	{
		for(l = *tabs; l->urm != NULL; l = l->urm);
		l->urm = aux;
	}
}
// simuleaza trecerea timpului
void wait(int bandwidth, TLG *downloaded, void *downloads, int seconds)
{
	if(bandwidth == 0)
		return;
	void *el;
	TLG l, aux;
	Resource *elRes;
	unsigned long dim_to_download, remained;
	// calculez dimensiunea pe care o poate descarca
	dim_to_download = seconds * bandwidth;
	while(dim_to_download > 0)
	{
		// extrag prima resursa care trebuie downloadata
		if(!Pop(downloads, &el))
			return;
		elRes = (Resource *)el;
		// calculez cat a ramas de descarcat dn resursa extrasa
		remained = elRes->dimension - elRes->currently_downloaded;
		/* in cazul in care dimensiunea ramasa de descarcat din elementul 
		extras este mai mare decat dimensiunea pe care o putem descarca
		se va adauga la currently_downloaded si se va introduce inapoi
		in stiva de elemente care trebuie descarcate*/
		if(dim_to_download < remained)
		{
			((Resource *)el)->currently_downloaded += dim_to_download;
			dim_to_download = 0;
			Push(downloads, el);
		}
		/* altfel, se va calcula cat mai poate downloada, iar elementul extras
		va fi introdus intr-o lista de descarcari finalizate*/
		else
		{
			dim_to_download -= remained;
			elRes->currently_downloaded = elRes->dimension;
			aux = malloc(sizeof(TCelulaG));
			aux->urm = NULL;
			aux->info = elRes;
			if(!*downloaded)
				*downloaded = aux;
			else
			{
				for(l = *downloaded; l->urm != NULL; l = l->urm);
				l->urm = aux;
			}
		}
	}
}
// schimba tabul curent
void change_tab(TLG tabs, Ttab **current_tab, int tab_index)
{
	TLG l = tabs;
	int i = 0;
	for( ; i != tab_index; i++, l = l->urm)
		if(l->urm == NULL)
			break;
	if(l != NULL)
		*current_tab = l->info;
}
// deschide o noua pagina web in tabul curent
void openPage(Ttab *current_tab, char *url, void* history, int bandwidth,
			  TLG *downloaded, void *downloading)
{
	// daca exista deja o pagina web deschisa o adaug in stiva de back
	if(current_tab->current_page)
		Push(current_tab->back_stack, current_tab->current_page);
	TWeb *aux = malloc(sizeof(TWeb));
	aux->resources = get_page_resources(url, &aux->num_res);
	strcpy(aux->url, url);
	// resetez stiva de forward
	ResetS(current_tab->forward_stack, DistrWeb);
	current_tab->current_page = aux;
	// adaug url-ul paginii web in istoricul global
	IntrQ(history, url);
	wait(bandwidth, downloaded, downloading, 1);
}
// sterge primele nr_entries din istoric
void del_history(void *history, int nr_entries)
{
	void *el;
	int i;
	// sterge tot istoricul
	if(nr_entries == 0)
	{
		while(ExtrQ(history, &el))
			free(el);
		return;
	}
	// extrage primele nr_entries intrari din istoric
	for(i = 0; i < nr_entries; i++)
	{
		if(!ExtrQ(history, &el))
			break;
		free(el);
	}
}
// afiseaza paginile web de pe taburile deschise
void print_open_tabs(TLG tabs, FILE *g)
{
	TLG l;
	int i = 0;
	for(l = tabs; l != NULL; i++, l = l->urm)
		if(verify_open_page(l->info) == 1)
			fprintf(g, "(%d: %s)\n", i, get_url_web(l->info));
		else
			fprintf(g, "(%d: empty)\n", i);
}
// schimba pagina curenta la ultima pagina web accesata
void back(Ttab *current_tab, FILE *g)
{
	if(!current_tab->back_stack)
	{
		fprintf(g, "can’t go back, no pages in stack\n");
		return;
	}
	Push(current_tab->forward_stack, current_tab->current_page);
	Pop((void*)current_tab->back_stack, (void**)&current_tab->current_page);
}
// schimba pagina curenta la pagina web de la care s-a intors
void forward(Ttab *current_tab, FILE *g)
{
	if(!current_tab->forward_stack)
	{
		fprintf(g, "can’t go forward, no pages in stack\n");
		return;
	}
	Push(current_tab->back_stack, current_tab->current_page);
	Pop(current_tab->forward_stack, (void **)&current_tab->current_page);
}
// afiseaza istoricul global
void printHistory(void *history, FILE *g)
{
	void *el, *historyAux;
	historyAux = InitQ(sizeof(char) * MAX_URL_NAME_LEN);
	while(ExtrQ(history, &el))
	{
		fprintf(g, "%s\n", (char *)el);
		IntrQ(historyAux, el);
	}
	ConcatQ(history, historyAux);
	free(historyAux);
}
// afiseaza resursele descarcabile de pe o pagina web
void list_dl(Ttab *current_tab, FILE *g)
{
	if(!current_tab->current_page || current_tab->current_page->num_res == 0)
		return;
	Resource *resources = current_tab->current_page->resources;
	int i;
	for(i = 0; i < current_tab->current_page->num_res; i++)
		fprintf(g, "[%d - \"%s\" : %lu]\n", i,
				resources[i].name, resources[i].dimension);
}
// sterge ultimul tab deschis
void deltab(TLG *tabs, Ttab **current_tab)
{
	TLG l, ant;
	for(l = *tabs, ant = NULL; l->urm != NULL; ant = l, l = l->urm);
	if(l == NULL)
		return;
	/* in cazul in care tabul curent este tabul care trebuie inchis, tabul
	curent va fi penultimul tab din lista*/
	if(l->info == *current_tab)
		*current_tab = ant->info;
	// elibereaza memoria ocupata de tabul care trebuie sters
	if(l == *tabs){
		DistrTab(l->info);
		free(l);
		*tabs = NULL;
	}
	else
	{
		DistrTab(l->info);
		ant->urm = NULL;
		free(l);
	}
}
// descarca resursa "index_resursa" din vectorul de resurse al paginii curente
void download(Ttab *current_tab, int index_resursa, void *downloading)
{
	Resource *aux = malloc(sizeof(Resource));
	// verific daca este o pagina deschisa
	if(!current_tab->current_page)
	{
		free(aux);
		return;
	}
	*aux = current_tab->current_page->resources[index_resursa];
	void *el, *downloadingAux;
	// daca coada de downloadari este goala, adaug resursa in coada
	if(VIDA(downloading))
	{
		IntrQ(downloading, aux);
		return;
	}
	downloadingAux = InitQ(sizeof(Resource));
	unsigned long remainingAux, remainingEl;
	remainingAux = aux->dimension - aux->currently_downloaded;

	while(ExtrQ(downloading, &el)){
		/* extrag elemente din lista de downloadari pana gaseste locul in care
		trebuie adaugata resursa noua*/
		Resource *elRes = (Resource *)el;
		remainingEl = elRes->dimension - elRes->currently_downloaded;
		if(remainingAux > remainingEl)
			IntrQ(downloadingAux, el);
		else
		{
			IntrQ(downloadingAux, aux);
			IntrQ(downloadingAux, el);
			break;
		}
	}
	if(VIDA(downloading) && remainingAux > remainingEl)
		IntrQ(downloadingAux, aux);
	// refac cooada de descarcari
	ConcatQ(downloadingAux, downloading);
	ConcatQ(downloading, downloadingAux);
	free(downloadingAux);
}
// afiseaza resursele in curs de downloadare si resursele downloadate
void printDownloads(void *downloads, TLG downloaded, FILE *g)
{
	void *el, *downloadsAux;
	downloadsAux = InitQ(sizeof(Resource));
	while(ExtrQ(downloads, &el))
	{
		unsigned long remaining;
		Resource aux;
		aux = *(Resource*)(el);
		remaining = aux.dimension - aux.currently_downloaded;
		fprintf(g, "[\"%s\" : %ld/%ld]\n", aux.name, remaining, aux.dimension);
		IntrQ(downloadsAux, el);
	}
	ConcatQ(downloads, downloadsAux);
	for( ; downloaded != NULL; downloaded = downloaded->urm)
	{
		fprintf(g, "[\"%s\" : completed]\n", 
			   ((Resource*)(downloaded->info))->name);
	}
	free(downloadsAux);
}




int main(int argc, char const *argv[])
{
	FILE *f, *g;
	char cmd[MAX_CMD_NAME_LEN];
	int bandwidth = 1024, tab_index, nr_entries, index_resursa, seconds;
	f = fopen(argv[1], "rt");
	g = fopen(argv[2], "wt");

	TLG tabs, downloaded;
	tabs = NULL;
	downloaded = NULL;
	Ttab *current_tab;
	newtab(&tabs, &current_tab);
	void *history, *downloading;
	downloading = InitQ(sizeof(Resource));
	history = InitQ(sizeof(char) * MAX_URL_NAME_LEN);
	while(!feof(f))
	{
		fscanf(f, "%s\n", cmd);
		if(cmd[strlen(cmd) - 1] == '\n')
			cmd[strlen(cmd) - 1] = '\0';
		
		if(strcmp(cmd, "set_band") == 0)
			fscanf(f, "%d\n", &bandwidth);

		if(strcmp(cmd, "newtab") == 0)
			newtab(&tabs, &current_tab);

		if(strcmp(cmd, "deltab") == 0)
			deltab(&tabs, &current_tab);

		if(strcmp(cmd, "change_tab") == 0)
		{
			fscanf(f, "%d", &tab_index);
			change_tab(tabs, &current_tab, tab_index);
		}

		if(strcmp(cmd, "print_open_tabs") == 0)
			print_open_tabs(tabs, g);

		if(strcmp(cmd, "goto") == 0)
		{
			char *url = malloc(sizeof(char) * MAX_URL_NAME_LEN);
			fscanf(f, "%s\n", url);
			if(url[strlen(url) - 1] == '\n')
				url[strlen(url) - 1] = '\0';
			openPage(current_tab, url, history, bandwidth, &downloaded,
					 downloading);
		}

		if(strcmp(cmd, "back") == 0)
			back(current_tab, g);

		if(strcmp(cmd, "forward") == 0)
			forward(current_tab, g);
				
		if(strcmp(cmd, "history") == 0)
			printHistory(history, g);

		if(strcmp(cmd, "del_history") == 0)
		{
			fscanf(f, "%d", &nr_entries);
			del_history(history, nr_entries);
		}
		if(strcmp(cmd, "list_dl") == 0)
			list_dl(current_tab, g);

		if(strcmp(cmd, "downloads") == 0)
			printDownloads(downloading, downloaded, g);

		if(strcmp(cmd, "download") == 0)
		{
			fscanf(f, "%d", &index_resursa);
			download(current_tab, index_resursa, downloading);
		}

		if(strcmp(cmd, "wait") == 0)
		{
			fscanf(f, "%d", &seconds);
			wait(bandwidth, &downloaded, downloading, seconds);
		}
	}
	DistrAllTabs(&tabs);
	DistrDownloaded(&downloaded);
	del_history(history, 0);
	free(history);
	void *el;
	while(ExtrQ(downloading, &el))
		free(el);
	free(downloading);
	fclose(f);
	fclose(g);
	return 0;
}