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
	int route;
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
	int id;
	int road[99];
	int nbr;
};
typedef struct Route Route;

Vaisseau atlanta = {1,"SS Atlanta", 1, 0, 0 , 60, 20, 10, 0};

Vaisseau ennemis[99];

Weapon cannonIon = {1, "Cannon à Ions", 20, 2, 80};
//random
int random_nbr (int min, int max){
	int nbr = 0;
	nbr = (min + (rand () % (max + 1 - min)));
	return nbr;
};

void temp_test_color(){
	printf("\033[0;31m■■■");//rouge
    printf("\033[1;31m■■■");//rouge pâle
    printf("\033[0;32m■■■");//vert
    printf("\033[1;32m■■■");//vert pâle
    printf("\033[0;33m■■■");//jaune foncé
    printf("\033[1;33m■■■");//jaune clair
    printf("\033[0;34m■■■");//Bleu
    printf("\033[1;34m■■■");//Bleu clair
    printf("\033[0;35m■■■");//Violet
    printf("\033[1;35m■■■");//Violet pâle
    printf("\033[0;33m■■■");//Ocre
    printf("\033[0;36m■■■");//Cyan
    printf("\033[1;36m■■■");//Cyan fluo
}

void printCustomChar(char array[]){
	printf("%s\n",&array[0]);//fonctionne !!
}
//display en couleurs s'il vous plait
void display(int index, int sub)	{
	switch(index) {
		case 1 :
			//Titre
			printf("┌───────────────────────────────────────────────────────────────┐\n"
				   "│\033[1;33m  _________                           __      __               \033[0m│\n"
				   "│\033[1;33m /   _____/__________    ____  ____  /  \\    /  \\_____ _______ \033[0m│\n"
				   "│\033[1;33m \\_____  \\\\____ \\__  \\ _/ ___\\/ __ \\ \\   \\/\\/   /\\__  \\\\_  __ \\\033[0m│\n"
				   "│\033[1;33m /        \\  |_> > __ \\\\  \\__\\  ___/  \\        /  / __ \\|  | \\/\033[0m│\n"
				   "│\033[1;33m/_______  /   __(____  /\\___  >___  >  \\__/\\  /  (____  /__|   \033[0m│\n"
				   "│\033[1;33m        \\/|__|       \\/     \\/    \\/        \\/        \\/       \033[0m│\n"
				   "│                                                               │\n"
				   "├───────────────────────────────────────────────────────────────┤\n"
				   "│                                                               │\n"
				   "│                  Bienvenu(e) dans \033[0;33mSPACE WAR\033[0m !                 │\n"
				   "│                                                               │\n"
				   "│ Vous êtes le commandant  du  \033[0;36mSS Atlanta\033[0m, un vaisseau  dont la │\n"
				   "│ mission est de sauver la Vie d'une disparition certaine . . . │\n"
				   "│                                                               │\n"
				   "│ En l'an XX456-b, des  astronomes Tumeriens  ont decouvert que │\n"
				   "│ l'univers  s'apprêtait a commencer un  \033[0;31mBig Bang\033[0m, depuis,  les │\n"
				   "│ espèces  intelligentes  de  l'univers tout entier se démènent │\n"
				   "│ afin de prévenir la catastrophe.                              │\n"
				   "│                                                               │\n"
				   "│ La \033[1;33mMachine\033[0m  du  défunt  \033[1;35mdocteur Bagrat\033[0m  a  prouvé qu'il était │\n"
				   "│ possible de  \033[1;36mvoyager entre les dimensions\033[0m  à l'aide d'un trou │\n"
				   "│ noir  permettant  ainsi de s'enfuir dans  une dimension où le │\n"
				   "│ \033[0;31mBig Bang\033[0m n'a jamais eu lieu. . . Cependant, craignant que son │\n"
				   "│ invention tombe entre de mauvaises mains, le \033[1;35mdocteur Bagrat\033[0m a │\n"
				   "│ dissimulé  les  \033[1;32mplans\033[0m  de  sa  \033[1;33mMachine\033[0m  aux  quatre coins  de │\n"
				   "│ l'univers . . .                                               │\n"
				   "│                                                               │\n"
				   "│ Votre ami,  le petit fils du \033[1;35mdocteur Bagrat\033[0m vous a envoyé les │\n"
				   "│ coordonnées où il suspecte que son aïeul y a caché une partie │\n"
				   "│ des \033[1;32mplans\033[0m.                                                    │\n"
				   "│                                                               │\n"
				   "│ Votre  mission est de retracer les pas du \033[1;35mdocteur Bagrat\033[0m afin │\n"
				   "│ de mettre la main sur ces \033[1;32mplans\033[0m avant qu'il ne soit trop tard │\n"
				   "│ ou  que  les \033[1;32mplans\033[0m  ne  tombent  entre  les mains  de grandes │\n"
				   "│ puissances  qui  en  userait  pour  contrôler l'univers . . . │\n"
				   "│                                                               │\n"
				   "└───────────────────────────────────────────────────────────────┘\n");
			break;
		//scenario
		case 2 :
			switch (sub) {
				//intro
				case 1:
					//printf();
					break;
			}
			break;
		//Menu Vaisseau
		case 3:
			switch (sub) {
				case 1:
					printf("┌──────────────────────────────────────────────────────────────┐\n"
						   "│                       SS Atlanta                             │\n"
						   "├────────────────────────────┬─────────────────────────────────┤\n"
						   "│                            │                                 │\n"
						   "│ [1] Armes                  │ [2] Systèmes                    │\n"
						   "│                            │                                 │\n"
						   "├────────────────────────────┼─────────────────────────────────┤\n"
						   "│                            │                                 │\n"
						   "│ [3] Equipage               │ [4] Navigation                  │\n"
						   "│                            │                                 │\n"
						   "├────────────────────────────┼─────────────────────────────────┤\n");
					if (atlanta.fuel < 10) {
						printf("│ Fuel ------------------- %d ",atlanta.fuel);
					}else if (atlanta.fuel < 100) {
						printf("│ Fuel ------------------ %d ",atlanta.fuel);
					}else if (atlanta.fuel < 1000) {
						printf("│ Fuel ----------------- %d ",atlanta.fuel);
					}
					if (atlanta.flouze < 10) {
						printf("│ Crédits ------------------- %d § │\n",atlanta.flouze);
					}else if (atlanta.flouze < 100) {
						printf("│ Crédits ------------------ %d § │\n",atlanta.flouze);
					}else if (atlanta.flouze < 1000) {
						printf("│ Crédits ----------------- %d § │\n",atlanta.flouze);
					}
					printf("└────────────────────────────┴─────────────────────────────────┘\n");
					break;
				case 2:
					printf("┌──────────────────────────────────────────────────────────────┐"
						   "│                          Armes                               │"
						   "├─────────────────────┬────────────────────────────────────────┤"
						   "│ [1] Item Name       │ Description in italic, describe use    │"
						   "└─────────────────────┴────────────────────────────────────────┘");
					printCustomChar("Bonjour Monde");
					break;
			}
	}
}
void printMap(World * w, int monde){
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
	printf("┌───────────────────────────────────────────────────────────────┐\n"
		   "│                      ---─── Monde %d ───---                    │\n"
		   "├───────────────────────────────────────────────────────────────┤\n"
		   "│   0      1      2      3      4      5      6      7      8   │\n"
		   "├───────────────────────────────────────────────────────────────┤\n",monde);
	switch (w->route){
		case 1:	
			printf("│·······························································│\n"
		   		   "│····················┌── %s ──── %s ──┐·····┌── %s ─┐··············│\n"
		   		   "│······┌── %s ──── %s ─┴── %s ─┐·······├─ %s ─┴── %s ─┴── %s ──┐······│\n"
		   		   "│·· %s ─┴── %s ──── %s ──── %s ─┴── %s ─┬┘·····┌── %s ─┐·······├─ %s ··│\n"
		   		   "│··································└── %s ─┴── %s ─┴── %s ──┘······│\n"
		   		   "│·······························································│\n",
		   			milieuPrint[5],milieuPrint[8],milieuPrint[12],milieuPrint[1],milieuPrint[3],
		   			milieuPrint[6],milieuPrint[10],milieuPrint[13],milieuPrint[16],milieuPrint[0],
		   			milieuPrint[2],milieuPrint[4],milieuPrint[7],milieuPrint[9],milieuPrint[14],
		   			milieuPrint[18],milieuPrint[11],milieuPrint[15],milieuPrint[17]);
			break;
		case 2:
			printf("│·······························································│\n"
				   "│····················┌── %s ─┬── %s ──┬─ %s ──── %s ─┬── %s ──┐······│ \n"
				   "│······┌── %s ──── %s ─┴┐·····└── %s ─┬┘·····┌── %s ─┴── %s ──┼─ %s ··│\n"
				   "│·· %s ─┴── %s ─┬── %s ──┴─ %s ──┐·····└── %s ─┴┬─ %s ──── %s ──┘······│\n"
				   "│·············└── %s ─┬── %s ──┴─ %s ──┬─ %s ──┘····················│\n"
				   "│····················└── %s ──── %s ──┘···························│\n",
				   milieuPrint[6],milieuPrint[10],milieuPrint[14],milieuPrint[17],milieuPrint[20],
		   		   milieuPrint[1],milieuPrint[3],milieuPrint[11],milieuPrint[18],milieuPrint[21],
		   		   milieuPrint[23],milieuPrint[0],milieuPrint[2],milieuPrint[4],milieuPrint[7],
		   		   milieuPrint[15],milieuPrint[19],milieuPrint[22],milieuPrint[5],milieuPrint[8],
		   		   milieuPrint[12],milieuPrint[16],milieuPrint[9],milieuPrint[13]);
			break;
		case 3:
			printf("│··································┌── %s ──── %s ──── %s ──┐······│\n"
		   		   "│·············┌── %s ──┐·····┌── %s ─┴┐····················├─ %s ··│\n"
		   		   "│······┌── %s ─┴┬─ %s ─┬┴─ %s ─┴── %s ──┴─ %s ──┐·····┌── %s ──┤······│\n"
		   		   "│·· %s ─┴── %s ─┬┘·····└── %s ──── %s ─┐·······├─ %s ─┴┬─ %s ──┘······│\n"
		   		   "│·············└── %s ─┬── %s ──┐·····├── %s ──┘······│·············│\n"
		   		   "│····················└── %s ──┴─ %s ─┴── %s ──── %s ──┘·············│\n",
		   		   milieuPrint[14],milieuPrint[18],milieuPrint[21],milieuPrint[3],milieuPrint[10],
		   		   milieuPrint[24],milieuPrint[1],milieuPrint[4],milieuPrint[6],milieuPrint[11],
		   		   milieuPrint[15],milieuPrint[22],milieuPrint[0],milieuPrint[2],milieuPrint[7],
		   		   milieuPrint[12],milieuPrint[19],milieuPrint[23],milieuPrint[5],milieuPrint[8],
		   		   milieuPrint[16],milieuPrint[9],milieuPrint[13],milieuPrint[17],milieuPrint[20]);
			break;
		case 4:
			printf("│·······························································│\n"
				   "│···························┌── %s ──── %s ──┐····················│\n"
				   "│·············┌── %s ──── %s ─┴── %s ──┬─ %s ──┴─ %s ─┬── %s ──┬─ %s ··│\n"
				   "│·· %s ──── %s ─┴── %s ─┬── %s ──┬─ %s ──┘············└── %s ──┘······│\n"
				   "│····················└── %s ──┘··································│\n"
				   "│·······························································│\n",
				   milieuPrint[7],milieuPrint[10],milieuPrint[2],milieuPrint[4],milieuPrint[8],
		   		   milieuPrint[11],milieuPrint[12],milieuPrint[13],milieuPrint[15],milieuPrint[0],
		   		   milieuPrint[1],milieuPrint[3],milieuPrint[5],milieuPrint[9],milieuPrint[14],milieuPrint[6]);
			break;
		/*case 5:
			printf("\n"
				   "\n"
				   "\n"
				   "\n"
				   "\n"
				   "\n",);
			break;
		case 6:
			printf("\n"
				   "\n"
				   "\n"
				   "\n"
				   "\n"
				   "\n",);
			break;
	*/}
	printf("└───────────────────────────────────────────────────────────────┘\n");
}

void menu_vaisseau() {
	int test = 0;
	int choix = 0;
	while (test == 0){
		system("clear");
		display(3,1);
		scanf("%d",&choix);
		switch (choix) {
			case 1://Armes
				display(3,2);
				scanf("%d",&choix);
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				break;
		}
	}
}
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
							//menu vaisseau
							break;
					}
					break;
			}
			break;
	}
}
//Menu navigation
int navigation(World * w, Etape * e, int current, int monde){
	int choix = 0;
	int test = 0;
	int suiv = 0;
	e->state = 2;
	w->state[current] = 2;
	//display carte prégénérée au début du niveau, int etape pour definir le choix possible
	printMap(w, monde);
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

	world.route = route->id;
	for (int i = 0; i < route->nbr; i++){
		etape[i/2].next[0] = route->road[i-1];
		etape[i/2].next[1] = route->road[i];
	}
	for (int i = 0; i < route->nbr/2; i++){
		etape[i].milieu = random_nbr(1,4);
		world.type[i] = etape[i].milieu;
		etape[i].state = 1;
		world.state[i] = etape[i].state;
	}
	world.nbrEtape = route->nbr;
	for (int i = 1; i < 9; i++){
		currentEtape = navigation(&world,&etape[currentEtape],currentEtape,i);
		//navigation
		int alea = random_nbr(1,10);
		
		scenario(i,currentEtape,alea);
		//check end
	}
}


void generateMap(){
	int r = 0;
	Route route[99];
	route[1] = ( struct Route ) {1,{1,2,3,0,4,0,5,6,7,0,8,0,9,0,9,0,10,0,10,11,12,13,14,15,16,0,16,0,17,0,17,0,18,0,18,0,0,0},38};
	route[2] = ( struct Route ) {2,{1,2,3,0,4,5,6,7,7,0,8,9,10,11,12,0,12,0,13,0,14,0,14,15,16,0,16,0,17,0,18,19,19,0,20,21,20,21,22,0,23,0,23,0,23,0,0,0},48};
	route[3] = (struct Route ) {3,{1,2,3,4,4,5,6,0,6,7,8,9,10,11,12,0,13,0,13,0,14,15,15,0,16,17,16,17,18,0,19,0,19,0,20,0,21,0,22,23,23,0,24,0,24,0,24,0,0,0},50};
	route[4] = (struct Route ) {4,{1,0,2,3,4,0,5,6,7,8,9,0,9,0,10,0,11,0,11,0,12,0,12,0,13,14,15,0,15,0,0,0},32};
	r = random_nbr(1,4);
	niveau(&route[r]);
	//Niveau --> Debut d'un cycle
}


int main(){
	srand(time(NULL));
	int end = 0;
	//display(1,1);
	menu_vaisseau();


	//generateMap();
	//cycle
	while (end == 0){
		//generate map
	}
return 0;
}