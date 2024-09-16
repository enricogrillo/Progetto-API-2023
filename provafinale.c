#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>

struct nodo {
	int km;					//distanza dall'inizio dell'autostrada
	struct nodo *dx, *sx;	//puntatori ai nodi figli
	int *vetauto;			//vettore delle automobili
	int maxAuto;			//automobile con più autonomia
	int autoIndex;			//prima posizione libera nel vettore delle auto
	int aux;				//campo ausiliario utile per trovapercorso
};

struct nodo* creaNodo(int chilometro) {
	struct nodo* tmp = (struct nodo*) malloc(sizeof(struct nodo));
	//inizializza tutti i campi del nodo
	tmp->km = chilometro;
	tmp->sx = NULL;
	tmp->dx = NULL;
	tmp->vetauto = malloc(512*sizeof(int));
	tmp->maxAuto = -1;
	tmp->autoIndex = 0;
	tmp->aux = -1;
	
	return tmp;
}

//data una radice e un km, inserisce in quell'albero un nodo con tale km
struct nodo* inserisci_stazione(struct nodo* root, int chilometro, int *esito) {
	
	//se l'albero è vuoto, root deve puntare il nuovo nodo
	if(root == NULL) {
		*esito = 1;
		return creaNodo(chilometro);
	}
	
	//se la stazione è già presente, non posso aggiungere
	if(root->km == chilometro) {
		*esito = 0;
		return root;
	}
	
	//altrimenti, ricerca la posizione ideale ricorsivamente e inserisci
	if(chilometro < root->km)
		root->sx = inserisci_stazione(root->sx, chilometro, esito);
	else if(chilometro > root->km)
		root->dx = inserisci_stazione(root->dx, chilometro, esito);
	
	//infine, ritorna la radice dell'albero aggiornato
	return root;
}

//dato un km e una radice, cerca la stazione a quel km nell'albero con quella radice
struct nodo* cerca(struct nodo* root, int chilometro) {
	
	//ricerca banale: chiave nel root o albero vuoto
	if( root == NULL || root->km == chilometro)
		return root;
	
	//casi non banali: ricerca binaria ricorsiva
	if(root->km < chilometro)
		return cerca(root->dx, chilometro);
	return cerca(root->sx, chilometro);
}

//data una radice, un km e un'autonomia, inserisce l'auto con relativa 
//autonomia nella stazione al km dato all'interno dell'albero dato 
//e ti fa sapere se ci è riuscito
int inserisci_auto(struct nodo* root, int chilometro, int car) {
	struct nodo *staz;
	
	//cerchiamo la stazione al km dato
	staz = cerca(root, chilometro);
	
	//se nessuna radice analizzata ha quel km, non esiste la stazione
	if(staz == NULL)
		return 0;
	
	//se la stazione ha autonomia massima più bassa, la si aggiorna
	if(staz->maxAuto < car)
		staz->maxAuto = car;
	
	//inseriamo la macchina nella prima posizione libera
	staz->vetauto[staz->autoIndex] = car;
	
	//aggiorniamo la prima posizione libera
	(staz->autoIndex)++;
	
	return 1;
}

//dato un chilometro ed una radice, 
//rimuove la relativa stazione dall'albero e ti fa sapere se ha demolito
struct nodo* demolisci_stazione(struct nodo* root, int chilometro, int *esito) {
	
	//se l'albero è vuoto, non l'abbiamo trovata
	if(root == NULL) {
		*esito = 0;
		return root;
	}
	
	//cerchiamo ora il nodo da cancellare
	if(root->km > chilometro) {
		root->sx = demolisci_stazione(root->sx, chilometro, esito);
		return root;
	}
	else if(root->km < chilometro) {
		root->dx = demolisci_stazione(root->dx, chilometro, esito);
		return root;
	}
	
	/*a questo punto il nodo da cancellare è proprio la radice*/
	
	//se ha meno di due figli
	if(root->sx == NULL) {
		struct nodo* tmp = root->dx;
		free(root);
		*esito = 1;
		return tmp;
	}
	else if(root->dx == NULL) {
		struct nodo* tmp = root->sx;
		free(root);
		*esito = 1;
		return tmp;
	}
	
	//se ha due figli esattamente
	else {
		struct nodo* succParent = root;
		
		//trova il successore (minimo del sottoalbero dx)
		struct nodo* succ = root->dx;
		while(succ->sx != NULL) {
			succParent = succ;
			succ = succ->sx;
		}
		
		//riordiniamo i puntatori in zona successore
		if(succParent != root)
			succParent->sx = succ->dx;
		else
			succParent->dx = succ->dx;
		
		//salva i dati del successore nella nuova posizione
		root->km = succ->km;
		root->vetauto = succ->vetauto;
		root->maxAuto = succ->maxAuto;
		root->autoIndex = succ->autoIndex;
		root->aux = succ->aux;
		
		//demolisci il successore
		free(succ);
		*esito = 1;
		return root;
	}
}

int rottama_auto(struct nodo* root, int chilometro, int car) {
	struct nodo *staz;
	int cercamax=0;
	
	//se la stazione non c'è, non puoi rottamare
	staz = cerca(root, chilometro);
	if(staz == NULL)
		return 0;
	
	//se la massima auto della stazione non arriva a quella input, 
	//non possono esserci auto da cancellare in tale stazione
	if(staz->maxAuto < car)
		return 0;
	
	//se la stazione da cancellare è quella ad autonomia max, 
	//dopo dobbiamo ricordarci di trovare il nuovo max
	if(staz->maxAuto == car)
		cercamax = 1;
	
	//cerchiamo nel vettore di auto l'autonomia data
	int i = 0, trovato = 0;
	while(i<(staz->autoIndex)) {
		
		//se nella scansione troviamo il valore cercato
		if((staz->vetauto[i]) == car) {
			trovato = 1;
			
			//dobbiamo shiftare l'array da quel punto in poi di una posizione all'indietro
			for(int j=i+1; j<(staz->autoIndex); i++, j++)
				(staz->vetauto[i]) = staz->vetauto[j];
		}
		i++;
	}
	
	//se l'abbiamo trovato, abbiamo shiftato: occorre sistemare i dati
	if(trovato) {
		
		//shiftando, la dimensione del vettore diminuisce di 1
		(staz->autoIndex)--;
		
		//se abbiamo rimosso il massimo, dobbiamo trovarne un'altro
		if(cercamax) {
			int nuovomax = -1;
			for(int i=0;i<(staz->autoIndex);i++)
				if((staz->vetauto[i])>nuovomax)
					nuovomax = (staz->vetauto[i]);
			staz->maxAuto = nuovomax;
		}
	}
	return trovato;
}

// trova il minimo dell'albero con la radice passata
struct nodo* trovaMin(struct nodo* root) {
	struct nodo* curr;
	curr = root;
	
	// va a sinistra finchè si può
	while(curr->sx != NULL) {
		curr = curr->sx;
	}
	
	return curr;
}

//trova e restituisce il nodo successore del nodo che gli passiamo
struct nodo* successore(struct nodo* root, struct nodo* curr) {
	
	// se il nodo ha figlio dx, il succ è il minimo di quel sottoalbero
	if(curr->dx != NULL)
		return trovaMin(curr->dx);
	
	struct nodo *succ;
	succ = NULL;
	
	//partiamo dalla radice e cerchiamo il successore nell'albero
	while(root->km != curr->km) {
		if(curr->km < root->km) {
			succ = root;
			root = root->sx;
		}
		else
			root = root->dx;
	}
	
	return succ;
}

void stampaPathSxDx(struct nodo* root, struct nodo* sorg, struct nodo* dest) {
	
	/* ricorsiva: raggiunge la sorgente partendo dalla destinazione 
	 * ripercorrendo la strada trovata prima */
	
	//se dest = sorg, il percorso è fatto da una sola stazione
	if(dest->km == sorg->km) {
		printf("%d ", dest->km);
		return;
	}
	
	// altrimenti, richiamiamo con sorgente uguale, e destinazione diventa
	// il nodo più a sinistra che poteva raggiungere la destinazione
	stampaPathSxDx(root, sorg, cerca(root, dest->aux));
	
	// come nelle visite post-order, si stampa dopo, sfruttando la pila di sistema
	printf("%d ", dest->km);
	
	return;
}

void pathSxDx(struct nodo* root, struct nodo* sorg, struct nodo* dest) {
	
	/* PARTE 1: scorrere tutte le stazioni e indicare la stazione più a 
	 * sinistra che possa raggiungerle */
	
	struct nodo* curr;
	struct nodo* x;
	curr = sorg;
	x = successore(root, curr);
	(dest->aux) = (-1);
	
	//ripetiamo almeno una volta, finchè x non arriva alla destinazione
	do {
		
		// se curr può raggiungere la stazione x, lo scriviamo in x
		// e mandiamo avanti il puntatore x al successore
		if((curr->maxAuto) >= ((x->km) - (curr->km))) {
			x->aux = curr->km;
			if(x != dest)
				x = successore(root, x);
		}
			
		// altrimenti, avanziamo con la stazione curr
		else
			curr = successore(root, curr);
		
		// se le due stazioni si sovrappongono, non ci sono percorsi validi
		if(curr == x) {
			printf("nessun percorso");
			return;
		}
		
	} while(dest->aux == -1);
	
	/* PARTE 2: se siamo arrivati qui, vuol dire che un percorso 
	 * l'abbiamo trovato, quindi dobbiamo stamparlo */
	stampaPathSxDx(root, sorg, dest);
	
	return;
}

// trova il massimo dell'albero con la radice passata
struct nodo* trovaMax(struct nodo* root) {
	struct nodo* curr;
	curr = root;
	
	// va a destra finchè si può
	while(curr->dx != NULL) {
		curr = curr->dx;
	}
	
	return curr;
}

//trova e restituisce il nodo predecessore del nodo che gli passiamo
struct nodo* predecessore(struct nodo* root, struct nodo* curr) {
	
	// se il nodo ha figlio sx, il pred è il massimo di quel sottoalbero
	if(curr->sx != NULL)
		return trovaMax(curr->sx);
	
	struct nodo *pred;
	pred = NULL;
	
	//partiamo dalla radice e cerchiamo il predecessore nell'albero
	while(root->km != curr->km) {
		if(curr->km > root->km) {
			pred = root;
			root = root->dx;
		}
		else
			root = root->sx;
	}
	
	return pred;
}

void pathDxSx(struct nodo* root, struct nodo* sorg, struct nodo* dest) {
	
	/* PARTE 1: scorrere le stazioni dalla sorgente alla destinazione
	 * e associare ad ogni nodo la distanza minima (in hop) della sorgente
	 * da esso
	 */	
	
	// puntatore di appoggio per scorrere le stazioni
	struct nodo* curr;
	curr = sorg;
	
	// prendo il predecessore di curr e lo metto in x 
	// (in ordine di percorso, è la stazione dopo)
	struct nodo* x;
	x = predecessore(root, curr);
	
	// inizializzo a 0 la distanza della sorgente da sè stessa, ovviamente
	(sorg->aux) = 0;
	
	// per capire quando fermarmi, inizializzo dest->aux a -1
	(dest->aux) = -1;
	
	//ripetiamo almeno una volta, finchè x non arriva alla destinazione
	do {
		
		// se curr può raggiungere la stazione x, aggiorniamo gli hop
		// per raggiungere x e mandiamo avanti il puntatore x al predecessore
		if((curr->maxAuto) >= ((curr->km) - (x->km))) {
			
			// se può raggiungerlo, ci vuole 1 hop più del curr per arrivare alla sorgente
			x->aux = (curr->aux) + 1;
			if(x != dest)
				x = predecessore(root, x);
		}
			
		// altrimenti, avanziamo con la stazione curr
		else
			curr = predecessore(root, curr);
		
		// se le due stazioni si sovrappongono, non ci sono percorsi validi
		if(curr == x) {
			printf("nessun percorso");
			return;
		}
		
	} while(dest->aux == -1);
	
	/* PARTE 2: dati gli hop minimi, ricostruire il percorso ideale */
	
	// scansiono le stazioni in verso opposto, partendo dalla dest
	curr = dest;
	
	//vettore dinamico per memorizzare i dati da stampare
	int pila[dest->aux];
	int i = 0;
	
	// variabile distanza minima che serve dopo
	int mindist;
	
	// puntatore per la prossima stazione candidata
	struct nodo* prox_curr;
	prox_curr = NULL;
	
	// finchè la sorgente dista più di 1 dal curr, vado avanti
	while(curr->aux > 1) {
		
		// aggiungo il km del curr alla pila, dopo dovrò stampare tutto
		pila[i] = curr->km;
		i++;
		
		// devo scorrere ogni volta tutte le stazioni per le distanze minime
		x = successore(root, curr);
		
		// la distanza minima trovata all'inizio è infinita
		mindist = INT_MAX;
		
		// finchè x non arriva alla sorgente (guardo tutte le stazioni)
		while(x!=sorg) {
			
			// se x può raggiungere il corrente
			if(x->maxAuto >= (x->km)-(curr->km)) {
				
				// se x ha una distanza migliore di quella trovata finora
				if(x->aux < mindist) {
					
					// se è raggiungibile dalla sorgente, siamo arrivati
					if(x->aux == 1) {
						
						// aggiungiamo x e la sorgente alla pila
						pila[i] = x->km;
						i++;
						pila[i] = sorg->km;
						i++;
						
						// stampiamo la pila e torniamo
						for(int j = i-1; j>=0; j--)
							printf("%d ", pila[j]);
						return;
					}
					
					// ho trovato una distanza migliore, bisogna tenerne conto
					mindist = x->aux;
					
					// x si candida a prossima stazione del percorso migliore
					prox_curr = x;
				}
			}
			// a questo punto vado avanti con x
			x = successore(root, x);
		}
		// il candidato prossimo curr diventa l'effettivo curr
		curr = prox_curr;
	}
	
	return;
}

int main() {
	
	//creo il puntatore alla radice e lo inizializzo
	struct nodo *root;
	root = NULL;
	
	//creo e inizializzo le variabili utili per il funzionamento
	char input[19];			// stringa per il comando di input
	int km;					// variabile per chilometri in input
	int N;					// per contare i cicli
	int car;				// per l'autonomia delle auto da inserire
	int esito;				// per vedere come vanno le operazioni
	int s;
	
	while(scanf("%s", input) != EOF) {
		
		if(strcmp(input, "aggiungi-stazione") == 0) {
			
			/* AGGIUNGI STAZIONE */
			
			//acquisire il km e inserire la stazione nell'albero
			s=scanf("%d", &km);						
			root = inserisci_stazione(root, km, &esito);
			
			//se l'esito è negativo, non ho aggiunto la stazione
			if(esito == 0)
				printf("non aggiunta\n");
			
			//altrimenti, posso aggiungere le auto
			else {
				s=scanf("%d", &N);				
				for(int i=0;i<N;i++) {
					s=scanf("%d", &car);
					esito = inserisci_auto(root, km, car);
				}
				printf("aggiunta\n");
			}
			
		}
		else if(strcmp(input, "demolisci-stazione") == 0) {
			
			/* DEMOLISCI STAZIONE */
			
			// acquisire il km
			s=scanf("%d", &km);
			
			// demolire la stazione
			root = demolisci_stazione(root, km, &esito);
			
			// verificare l'esito della demolizione
			if(esito == 1)
				printf("demolita\n");
			else
				printf("non demolita\n");
			
		}
		else if(strcmp(input, "aggiungi-auto") == 0) {
			
			/* AGGIUNGI AUTO */
			
			// acquisire il km
			s=scanf("%d", &km);
			
			//acquisire l'autonomia dell'auto da aggiungere
			s=scanf("%d", &car);
			
			// aggiungere l'auto
			esito = inserisci_auto(root, km, car);
			
			// verificare l'esito dell'operazione
			if(esito == 1)
				printf("aggiunta\n");
			else
				printf("non aggiunta\n");
			
		}
		else if(strcmp(input, "rottama-auto") == 0) {
			
			/* ROTTAMA AUTO */
			
			// acquisire il km
			s=scanf("%d", &km);
			
			//acquisire l'autonomia dell'auto da rottamare
			s=scanf("%d", &car);
			
			// rottamare l'auto
			esito = rottama_auto(root, km, car);
			
			// verificare l'esito dell'operazione
			if(esito == 1)
				printf("rottamata\n");
			else
				printf("non rottamata\n");
			
		}
		else if(strcmp(input, "pianifica-percorso") == 0) {
			
			/* PIANIFICA PERCORSO */
			
			// dichiarare ed acquisire i due numeri di input
			int a, b;
			
			s=scanf("%d", &a); 
			s=scanf("%d", &b);
			
			//distinguere la direzione del percorso da pianificare
			if(a<b) {
				pathSxDx(root, cerca(root, a), cerca(root, b));
				printf("\n");
			}
			
			if(a>b) {
				pathDxSx(root, cerca(root, a), cerca(root, b));
				printf("\n");
			}
			
		}
	} 
	
	s = s+1;
	
	return 0;
}





