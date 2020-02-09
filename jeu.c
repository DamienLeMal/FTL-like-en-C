/*

	main 
		generate_map
			niveau
				navigation
					printMap

*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

struct Scenario{
	int id;
	char nom[30];
};
typedef struct Scenario Scenario;

//même struct pour les ennemis
struct Vaisseau{
	int id;
	char nom[30];
	int weapon1;
	int weapon2;
	int weapon3;
	int hull;
	int shield;
	//pour les ennemis, correspondra à la valeur du loot
	int fuel;
	int flouze;
};
typedef struct Vaisseau Vaisseau;

struct Weapon{
	int id;
	char nom[30];
	int atk;
	int nbr_fire;
	int accuracy;
};
typedef struct Weapon Weapon;

//Donne un point de vue d'ensemble du type de chaque etape pour les dessiner
struct World{
	int type[99];
	int state[99];
	int nbrEtape;
};
typedef struct World World;

//Definit chaque etape par son type et les etapes accessibles depuis celle-ci
struct Etape{
	int nbr_choix;
	int milieu;
	int next[2];
	int state;
};
typedef struct Etape Etape;

//Definit l'agencement des etapes selon un pattern prédéterminé
struct Route{
	int road[99];
	int nbr;
};
typedef struct Route Route;
//random
int random_nbr (int min, int max){
	int nbr = 0;
	nbr = (min + (rand () % (max + 1 - min)));
	return nbr;
};

void temp_test_color(){
	printf("\033[0;31m■■■");
    printf("\033[1;31m■■■");
    printf("\033[0;32m■■■");
    printf("\033[1;32m■■■");
    printf("\033[0;33m■■■");
    printf("\033[1;33m■■■");
    printf("\033[0;34m■■■");
    printf("\033[1;34m■■■");
    printf("\033[0;35m■■■");
    printf("\033[1;35m■■■");
    printf("\033[0;33m■■■");
    printf("\033[1;36m■■■");
}


//display en couleurs s'il vous plait
void display(int index, int sub)	{
	switch(index) {
		case 1 :
			//Titre
			printf("  _________                            __      __               \t"
				   " /   _____/__________    ____  ____   /  \\    /  \\_____ _______ \t"
				   " \\_____  \\\\____ \\__  \\ _/ ___\\/ __ \\  \\   \\/\\/   /\\__  \\\\_  __ \\\t"
				   " /        \\  |_> > __ \\\\  \\__\\  ___/   \\        /  / __ \\|  | \\/\t"
				   "/_______  /   __(____  /\\___  >___  >   \\__/\\  /  (____  /__|   \t"
				   "        \\/|__|       \\/     \\/    \\/         \\/        \\/       ");

		//scenario
		case 2 :
			switch (sub) {
				//intro
				case 1:
					//printf();
					break;
			}
			break;
	}
}

void printMap(World * w){
	char milieuPrint[99][99];

	for (int i = 0; i < w->nbrEtape; i++){
		switch (w->type[i]){
			case 1:
				switch (w->state[i]) {
					case 0:
						strcpy(milieuPrint[i], "\033[0m○\033[0m");
						break;
					case 1:
						strcpy(milieuPrint[i], "\033[1;33m■\033[0m");
						break;
					case 2:
						strcpy(milieuPrint[i], "\033[0m►\033[0m");
						break;
				}
				break;
			case 2:
				switch (w->state[i]) {
					case 0:
						strcpy(milieuPrint[i], "\033[0m○\033[0m");
						break;
					case 1:
						strcpy(milieuPrint[i], "\033[0;32m■\033[0m");
						break;
					case 2:
						strcpy(milieuPrint[i], "\033[0m►\033[0m");
						break;
				}
				break;
			case 3:
				switch (w->state[i]) {
					case 0:
						strcpy(milieuPrint[i], "\033[0m○\033[0m");
						break;
					case 1:
						strcpy(milieuPrint[i], "\033[0;35m■\033[0m");
						break;
					case 2:
						strcpy(milieuPrint[i], "\033[0m►\033[0m");
						break;
				}
				break;
			case 4:
				switch (w->state[i]) {
					case 0:
						strcpy(milieuPrint[i], "\033[0m○\033[0m");
						break;
					case 1:
						strcpy(milieuPrint[i], "\033[0;34m■\033[0m");
						break;
					case 2:
						strcpy(milieuPrint[i], "\033[0m►\033[0m");
						break;
				}
				break;
		}
	}
	printf("├───────────────────────────────────────────────────────────────┤\n"
		   "│   0      1      2      3      4      5      6      7      8   │\n"
		   "├───────────────────────────────────────────────────────────────┤\n"
		   "│·······························································│\n"
		   "│····················┌── %s ──── %s ──┐·····┌── %s ─┐··············│\n"
		   "│······┌── %s ──── %s ─┴── %s ─┐·······├─ %s ─┴── %s ─┴── %s ──┐······│\n"
		   "│·· %s ─┴── %s ──── %s ──── %s ─┴── %s ─┬┘·····┌── %s ─┐·······├─ %s ··│\n"
		   "│··································└── %s ─┴── %s ─┴── %s ──┘······│\n"
		   "│·······························································│\n"
		   "└───────────────────────────────────────────────────────────────┘\n",
		   milieuPrint[5],milieuPrint[8],milieuPrint[12],milieuPrint[1],milieuPrint[3],
		   milieuPrint[6],milieuPrint[10],milieuPrint[13],milieuPrint[16],milieuPrint[0],
		   milieuPrint[2],milieuPrint[4],milieuPrint[7],milieuPrint[9],milieuPrint[14],
		   milieuPrint[18],milieuPrint[11],milieuPrint[15],milieuPrint[17]);
}

//menu vaisseau
	//voir ressources
	//voir équipage et compétences
	//voir niveau des systèmes
	//voir armes équipées et leurs caractéristiques

//equiper armes lorsqu'il y en a une nouvelle

//menu display combat
	//voir Etat Coque + Bouclier
		/*
		___________________________________
		║║║║║║║║║║║║║║║║║║║║║║║║║║║║║║║║║║║
		¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
		*/
	//voir Energie disponible
	//voir actions possibles
	
//Combat (struct toi, struct mechant)
	//while
		//Afficher menu combat
		//Choix
		//impact sur l'ennemis
		//choix ennemis
		//impact sur toi
	//end While
	//Regeneration du shield

//Menu shop
	//voir items en vente ( choisis aléatoirement en fonction d'un pool propre à l'avancée et au lieu)
	//voir caractéristiques des armes et objets
	
//Menu scenario
void scenario(int niveau, int milieu, int index){
	switch (niveau){
		case 1 :
			switch (milieu){
				case 1 :
					switch (index){
						case 1:	
							//print
							//choix ou arboressence de choix
								//output -> combat, shop, récolte ou perte
							break;
					}
					break;
			}
			break;
	}
}
//Menu navigation
int navigation(World * w, Etape * e, int current){
	int choix = 0;
	int test = 0;
	int suiv = 0;
	e->state = 2;
	w->state[current] = 2;
	//display carte prégénérée au début du niveau, int etape pour definir le choix possible
	printMap(w);
	//choix du milieu (entre 1 et 2 choix possibles) peut être : près d'une etoile, planete, asteroide, vide spacial, chacun de ces millieu ayant ses opportunités et difficultés
	if (current == w->nbrEtape){
		test = 1;
	}
	while (test == 0){
		printf("Quelle sera la destination ? \n");
		scanf("%d",&choix);
		if (choix == 2){
			if (e->next[1] == 0){
				printf("Choix Invalide\n");
			}else{
				test = 1;
			}
		}else if (choix == 1){
			test = 1;
		}else{
			printf("Choix Invalide\n");
		}
	}
	//pool de scenario possible en fonction de l'avancée et du choix -> scenario(niveau, choix, random);
	e->state = 0;
	w->state[current] = 0;
	suiv = e->next[choix - 1];
	printf("%d\n",suiv );
	return (suiv);
}


void niveau(Route * route){
	int currentEtape = 0;

	//attribution des valeurs générées dans Etape
	Etape etape[99];
	World world;
	for (int i = 0; i < route->nbr; i++){
		etape[i/2].next[0] = route->road[i-1];
		etape[i/2].next[1] = route->road[i];
		printf("%d : %d %d\n",i/2, etape[i/2].next[0],etape[i/2].next[1]);
	}
	for (int i = 0; i < route->nbr/2; i++){
		etape[i].milieu = random_nbr(1,4);
		world.type[i] = etape[i].milieu;
		etape[i].state = 1;
		world.state[i] = etape[i].state;
	}
	world.nbrEtape = route->nbr;
	for (int i = 0; i < 8; i++){
		currentEtape = navigation(&world,&etape[currentEtape],currentEtape);
		//navigation
		//check end
	}
}


void generateMap(){
	int r = 0;
	Route route[99];
	route[1] = ( struct Route ) {{1,2,3,0,4,0,5,6,7,0,8,0,9,0,9,0,10,0,10,11,12,13,14,15,16,0,16,0,17,0,17,0,18,0,18,0,0,0},38};
	r = random_nbr(1,1);
	niveau(&route[1]);
	//Niveau --> Debut d'un cycle
}


int main(){
	srand(time(NULL));
	int end = 0;
	generateMap();
	//cycle
	while (end == 0){
		//generate map
	}
return 0;
}