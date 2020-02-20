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

	//même struct pour les ennemis
	struct Vaisseau{
		int id;
		char nom[30];
		int weapon1;
		int weapon2;
		int weapon3;
		int hull;
		int maxHull;
		int shield;
		int maxShield;
		int energy;
		int maxEnergy;
		//pour les ennemis, correspondra à la valeur du loot
		int fuel;
		int flouze;
		//0 pour les ennemis
		int sys[99];

		char color[5];
	};
	typedef struct Vaisseau Vaisseau;

	struct Crew{
		int level;
		int xp;
	};
	typedef struct Crew Crew;

	struct Weapon{
		int id;
		char nom[30];
		int atk;
		int nbr_fire;
		int accuracy;
		int price;
		char priceChar[5];
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

	Vaisseau atlanta = {1,"SS Atlanta", 1, 0, 0, 57, 60, 14, 20, 10, 20, 12, 600, {1,1,1,0,0}, "0;44"};

	Vaisseau ennemis[99] = {{0,"│ Eclaireur Pyrien │", 1, 0, 0, 40, 40, 20, 20, 5, 10, 5, 12, {0}, "0;101"},{1,"│ Explorer IV │", 1, 0, 0, 80, 80, 10, 10, 10, 20, 14, 22, {0}, "0;101"}};

	Crew james = {0, 1};
	Crew peter = {0, 1};
	Crew victor = {0, 1};
	Crew qtal = {0, 1};

	Weapon armes[99] = {{0, "", 0, 0, 0, 0, ""},{1, "Cannon à Ions", 20, 2, 80, 200, "200"}};

	char clr[] = "1;91";

	//random
	int random_nbr (int min, int max){
		int nbr = 0;
		nbr = (min + (rand () % (max + 1 - min)));
		return nbr;
	}

	void printCustomChar(char array[], int max_lenght, int dir, char btw[]){
		int lenght = strlen(array);
		int limit = max_lenght - lenght;
		if (dir == 0){
			printf("%s ", &array[0]);
		}
		for (int i = 0; i <= limit; i++) {
			printf("%s", &btw[0]);
		}
		if (dir == 1){
			printf(" %s",&array[0]);
		}
	}
	void printSystemShop(int index, int sub){
		switch (index){
			case 3:
				printf("│ [%d] \e[40mExtension d'arme 2\e[0m -------------------------------- 500 § │\n",sub);
				break;
			case 4:
				printf("│ [%d] \e[40mExtension d'arme 3\e[0m -------------------------------- 500 § │\n",sub);
				break;
		}
	}
	void printMenu (int index, char nbr[3]) {
		switch (index){
			//0 -> rien
			//-1 à 30 -> Weapon
			//101 à 130 -> Weapon Stats
			//31 à -> Systèmes
			case -1 : 
				printf("│ [%s] \e[3mNon assigné\e[0m      │                                        │\n", &nbr[0]);
				break;
			case 1:
				printf("│ [%s] Cannon à Ions    │ \e[3mCe cannon électrique est capable de    \e[0m│\n"
					   "│                      │ \e[3mtranspercer n'importe quel blindage !  \e[0m│\n", &nbr[0]);
				break;
			//id systems + 30
			case 31:
				printf("│%s\e[40mGénérateur:\e[0m \e[3mPermet de récupérer ponctuellement de l'énergie\e[0m │\n",&nbr[0]);
				break;
			case 32:
				printf("│%s\e[40mBouclier:\e[0m \e[3mSe régénère fréquemment, protège des tirs ennemies\e[0m│\n",&nbr[0]);
				break;
			case 33:
				printf("│%s\e[40mExtension d'arme 1:\e[0m \e[3mRajoute un emlacement pour une arme\e[0m     │\n",&nbr[0]);
				break;
			case 34:
				printf("│%s\e[40mExtension d'arme 2:\e[0m \e[3mRajoute un emlacement pour une arme\e[0m     │\n",&nbr[0]);
				break;
			case 35:
				printf("│%s\e[40mExtension d'arme 3:\e[0m \e[3mRajoute un emlacement pour une arme\e[0m     │\n",&nbr[0]);
				break;
			case 101 :
			printf("┌───────────────────┐\n"
				   "│   Cannon à Ions   │\n"
				   "├───────────────────┤\n"
				   "│      Attaque      │\n" 
				   "├───────────────────┤\n"
				   "│   ♦  ○  ○  ○  ○   │\n"
				   "├───────────────────┤\n"
				   "│     Precision     │\n"
				   "├───────────────────┤\n"
				   "│   ♦  ♦  ♦  ♦  ○   │\n"
				   "├───────────────────┤\n"
				   "│   Nombre de Tirs  │\n"
				   "├───────────────────┤\n"
				   "│   ♦  ♦  ○  ○  ○   │\n"
				   "├───────────────────┤\n"
				   "│ [0] Retour        │\n"
				   "└───────────────────┘\n");
				break;
		}
	}

	//display en couleurs s'il vous plait
	void display(int index, int sub)	{
		switch(index) {
			case 1 :
				//Titre
				printf("┌───────────────────────────────────────────────────────────────┐\n"
					   "│\e[1;33m  _________                           __      __               \e[0m│\n"
					   "│\e[1;33m /   _____/__________    ____  ____  /  \\    /  \\_____ _______ \e[0m│\n"
					   "│\e[1;33m \\_____  \\\\____ \\__  \\ _/ ___\\/ __ \\ \\   \\/\\/   /\\__  \\\\_  __ \\\e[0m│\n"
					   "│\e[1;33m /        \\  |_> > __ \\\\  \\__\\  ___/  \\        /  / __ \\|  | \\/\e[0m│\n"
					   "│\e[1;33m/_______  /   __(____  /\\___  >___  >  \\__/\\  /  (____  /__|   \e[0m│\n"
					   "│\e[1;33m        \\/|__|       \\/     \\/    \\/        \\/        \\/       \e[0m│\n"
					   "│                                                               │\n"
					   "├───────────────────────────────────────────────────────────────┤\n"
					   "│                                                               │\n"
					   "│                  Bienvenu(e) dans \e[0;33mSPACE WAR\e[0m !                 │\n"
					   "│                                                               │\n"
					   "│ Vous êtes le commandant  du  \e[0;36mSS Atlanta\e[0m, un vaisseau  dont la │\n"
					   "│ mission est de sauver la Vie d'une disparition certaine . . . │\n"
					   "│                                                               │\n"
					   "│ En l'an XX456-b, des  astronomes Tumeriens  ont decouvert que │\n"
					   "│ l'univers  s'apprêtait a commencer un  \e[0;31mBig Bang\e[0m, depuis,  les │\n"
					   "│ espèces  intelligentes  de  l'univers tout entier se démènent │\n"
					   "│ afin de prévenir la catastrophe.                              │\n"
					   "│                                                               │\n"
					   "│ La \e[1;33mMachine\e[0m  du  défunt  \e[1;35mdocteur Bagrat\e[0m  a  prouvé qu'il était │\n"
					   "│ possible de  \e[1;36mvoyager entre les dimensions\e[0m  à l'aide d'un trou │\n"
					   "│ noir  permettant  ainsi de s'enfuir dans  une dimension où le │\n"
					   "│ \e[0;31mBig Bang\e[0m n'a jamais eu lieu. . . Cependant, craignant que son │\n"
					   "│ invention tombe entre de mauvaises mains, le \e[1;35mdocteur Bagrat\e[0m a │\n"
					   "│ dissimulé  les  \e[1;32mplans\e[0m  de  sa  \e[1;33mMachine\e[0m  aux  quatre coins  de │\n"
					   "│ l'univers . . .                                               │\n"
					   "│                                                               │\n"
					   "│ Votre ami,  le petit fils du \e[1;35mdocteur Bagrat\e[0m vous a envoyé les │\n"
					   "│ coordonnées où il suspecte que son aïeul y a caché une partie │\n"
					   "│ des \e[1;32mplans\e[0m.                                                    │\n"
					   "│                                                               │\n"
					   "│ Votre  mission est de retracer les pas du \e[1;35mdocteur Bagrat\e[0m afin │\n"
					   "│ de mettre la main sur ces \e[1;32mplans\e[0m avant qu'il ne soit trop tard │\n"
					   "│ ou  que  les \e[1;32mplans\e[0m  ne  tombent  entre  les mains  de grandes │\n"
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
						printf("├───────────────────────────────────────────────────────────────┤\n"
							   "│                        SS Atlanta                             │\n"
							   "├─────────────────────────────┬─────────────────────────────────┤\n"
							   "│                             │                                 │\n"
							   "│ [1] Armes                   │ [2] Systèmes                    │\n"
							   "│                             │                                 │\n"
							   "├─────────────────────────────┼─────────────────────────────────┤\n"
							   "│                             │                                 │\n"
							   "│ [3] Equipage                │ [4] Navigation \e[0;30m(Suite)\e[0m          │\n"
							   "│                             │                                 │\n"
							   "├─────────────────────────────┼─────────────────────────────────┤\n");
						if (atlanta.fuel < 10) {
							printf("│ \e[0;35mFuel ------------------ %d F \e[0m",atlanta.fuel);
						}else if (atlanta.fuel < 100) {
							printf("│ \e[0;35mFuel ----------------- %d F \e[0m",atlanta.fuel);
						}else if (atlanta.fuel < 1000) {
							printf("│ \e[0;35mFuel ---------------- %d F \e[0m",atlanta.fuel);
						}
						if (atlanta.flouze < 10) {
							printf("│ \e[1;93mCrédits ------------------- %d § \e[0m│\n",atlanta.flouze);
						}else if (atlanta.flouze < 100) {
							printf("│ \e[1;93mCrédits ------------------ %d § \e[0m│\n",atlanta.flouze);
						}else if (atlanta.flouze < 1000) {
							printf("│ \e[1;93mCrédits ----------------- %d § \e[0m│\n",atlanta.flouze);
						}
						printf("└─────────────────────────────┴─────────────────────────────────┘\n");
						break;
					case 2:
						printf("├───────────────────────────────────────────────────────────────┤\n"
							   "│                           Armes                               │\n"
							   "├──────────────────────┬────────────────────────────────────────┤\n");
						printMenu(atlanta.weapon1, "1");
						printMenu(atlanta.weapon2, "2");
						printMenu(atlanta.weapon3, "3");
						printf("├──────────────────────┴────────────────────────────────────────┤\n"
							   "│ [0] Retour                                                    │\n"
							   "└───────────────────────────────────────────────────────────────┘\n");
						break;
					case 3:
						printf("├───────────────────────────────────────────────────────────────┤\n"
							   "│                         Systèmes                              │\n"
							   "├───────────────────────────────────────────────────────────────┤\n");
						break;
					case 4:
						printf("├───────────────────────────────────────────────────────────────┤\n");
						break;
					case 5:
						printf("│ [0] Retour                                                    │\n"
							   "└───────────────────────────────────────────────────────────────┘\n");
						break;
					case 6:
						printf("├───────────────────────────────────────────────────────────────┤\n"
							   "│                          Equipage                             │\n"
							   "├────────┬────────────┬───┬─────────────────────────────────────┤\n"
							   "│ James  │ Bouclier   │ %d │ ", james.level);
						int nombre = 35 - james.xp;
						for (int i = 0; i < james.xp; i++) {
							printf("\e[0;37m█\e[0m");
						}
						for (int i = 0; i < nombre; ++i){
							printf("\e[0;30m█\e[0m");
						}
						printf(" │\n");
						printf("│ Peter  │ Générateur │ %d │ ", peter.level);
						nombre = 35 - peter.xp;
						for (int i = 0; i < peter.xp; i++) {
							printf("\e[0;37m█\e[0m");
						}
						for (int i = 0; i < nombre; ++i){
							printf("\e[0;30m█\e[0m");
						}
						printf(" │\n");
						printf("│ Victor │ Artilleur  │ %d │ ", victor.level);
						nombre = 35 - victor.xp;
						for (int i = 0; i < victor.xp; i++) {
							printf("\e[0;37m█\e[0m");
						}
						for (int i = 0; i < nombre; ++i){
							printf("\e[0;30m█\e[0m");
						}
						printf(" │\n");
						printf("│ Qk'tal │ Pilote     │ %d │ ", qtal.level);
						nombre = 35 - qtal.xp;
						for (int i = 0; i < qtal.xp; i++) {
							printf("\e[0;37m█\e[0m");
						}
						for (int i = 0; i < nombre; ++i){
							printf("\e[0;30m█\e[0m");
						}
						printf(" │\n├────────┴────────────┴───┴─────────────────────────────────────┤\n");
						display(3,5);
						break;
					case 7:
						printf("├──────────────────────┼────────────────────────────────────────┤\n");
						break;

				}
				break;
			case 4: //Menu Shop
				switch (sub){
					case 1:
					printf("├───────────────────────────────────────────────────────────────┤\n");
						if (atlanta.fuel < 10) {
							printf("│ \e[0;35mFuel -------------------- %d \e[0m",atlanta.fuel);
						}else if (atlanta.fuel < 100) {
							printf("│ \e[0;35mFuel ------------------- %d \e[0m",atlanta.fuel);
						}else if (atlanta.fuel < 1000) {
							printf("│ \e[0;35mFuel ------------------ %d \e[0m",atlanta.fuel);
						}
						if (atlanta.flouze < 10) {
							printf("│ \e[1;93mCrédits ------------------- %d § \e[0m│\n",atlanta.flouze);
						}else if (atlanta.flouze < 100) {
							printf("│ \e[1;93mCrédits ------------------ %d § \e[0m│\n",atlanta.flouze);
						}else if (atlanta.flouze < 1000) {
							printf("│ \e[1;93mCrédits ----------------- %d § \e[0m│\n",atlanta.flouze);
						}
						printf("├───────────────────────────────────────────────────────────────┤\n"
							   "│                           Magasin                             │\n"
							   "├───────────────────────────────────────────────────────────────┤\n"
							   "│ Que désirez vous acheter ?                                    │\n"
							   "│ [1] Armes                                                     │\n"
							   "│ [2] Systèmes                                                  │\n"
							   "├───────────────────────────────────────────────────────────────┤\n"
							   "│ [3] Fuel x 2 ------------------------------------------- 20 § │\n"
							   "├───────────────────────────────────────────────────────────────┤\n"
							   "│ [4] Quitter                                                   │\n"
							   "└───────────────────────────────────────────────────────────────┘\n");
						break;
					case 2:
						printf("├───────────────────────────────────────────────────────────────┤\n"
							   "│                        Magasin : Armes                        │\n"
							   "├─────────────────┬───────────┬─────────────────────────────────┤\n");
						break;
					case 3:
						printf("├─────────────────┼───────────┼─────────────────────────────────┤\n");
						break;
					case 4:
						printf("├─────────────────┴───────────┴─────────────────────────────────┤\n");
						break;
					case 5:
						printf("├───────────────────────────────────────────────────────────────┤\n"
							   "│                      Magasin : Systèmes                       │\n"
							   "├───────────────────────────────────────────────────────────────┤\n");
						break;
					case 6:
						printf("│");
						break;
				}
				break;
			case 5:
				switch (sub){
					case 1:
						printf("├───────────────────────────────────────────────────────────────┤\n"
       						   "│·      ·   \e[0;103m░\e[0m  \e[0;103m░░▒▒▓▒█▓▓█▓▓█▓█████▓███▓██▓██▒▓▓░▒░░\e[0m  \e[0;103m▒\e[0m ·     ·  │\n"
       						   "│   ·         \e[0;103m░\e[0m \e[0;103m▒░▒░▓▒▓▒▓▓█▓▓██▓████▓███▓██▒▓▒░▒░▒ \e[0;103m▒\e[0m ·    ·     │\n"
       						   "│·     ·          \e[0;103m░░▒░▒▓▒▒▓▓█▒▓██▓▓█▓▓█▓▒▓▒░▒░▒░\e[0m         ·    · │\n"
       						   "│       ·         \e[0;103m░\e[0m   \e[0;103m░░▒░▒░▒▓▒▓█▒▓▒█▓▒▓▒█▓▒▓▒░░\e[0m \e[0;103m░\e[0m \e[0;103m░\e[0m   ·        │\n"
       						   "│   ·       ·       \e[0;103m▒\e[0m  \e[0;103m░░▒▒░▒░▒░▒▒░▒▓▒░▒▒░░\e[0m             ·    ·  │\n"
       						   "│       ·       ·     \e[0;103m░\e[0m   \e[0;103m▒▒░▒░░▒░▒░░▒░░░\e[0m   \e[0;103m▒\e[0m      ·      ·     │\n"
       						   "│   ·       ·    ▄▄▄·       \e[0;103m░\e[0m  \e[0;103m░\e[0m             ·          ·     · │\n"
       						   "│       ·      ▀▀█\e[%sm▒░\e[0m█▀                    ·    ▄█\e[%sm░░\e[0m█▄        ·  │\n"
       						   "│·    ·     ·   · █\e[%sm▒░\e[0m█▀  ·     ·      ▄▄█████▄█\e[%sm░\e[0m█▀▀█\e[%sm░\e[0m█▄\e[%sm▓▓▓\e[0m■ ·   │\n"
       						   "│   ·     ·  ·  ███\e[%sm▓▒░\e[0m█         ·  ·   · ▀■█▀█■▀■▄▄■▀▀█▄▄  ·    │\n"
       						   "│       ■■■■█████\e[%sm▓▓▒▒▒░\e[0m█   ·  ·      ·   ▄■█▄█■▄■▀▀■▄▄█▀▀·     ·│\n"
       						   "│·    ·  ·      ███\e[%sm▓▒░\e[0m█        ·    · ▀▀█████▀█\e[%sm░\e[0m█▄▄█\e[%sm░\e[0m█▀\e[%sm▓▓▓\e[0m■  ·  │\n"
       						   "│   ·   ·   ·     █\e[%sm▒░\e[0m█▄    ·      ·     ·    · ▀█\e[%sm░░\e[0m█▀        ·  │\n"
       						   "│     ·    ·   ▄▄█\e[%sm▒░\e[0m█▄   ·    ·    ·     ·   ·       ·      ·   │\n"
       						   "│·  ·   ·    ·   ▀▀▀·      ·      ·     ·    ·    ·       ·     │\n",&atlanta.color[0],&ennemis[0].color[0],&atlanta.color[0],&ennemis[0].color[0],&ennemis[0].color[0],&ennemis[0].color[0],&atlanta.color[0],&atlanta.color[0],&atlanta.color[0],&ennemis[0].color[0],&ennemis[0].color[0],&ennemis[0].color[0],&atlanta.color[0],&ennemis[0].color[0],&atlanta.color[0]);
						break;
					case 2:
						printf("├───────────────────────────────────────────────────────────────┤\n"
    						   "│·      ·         ·       ·       ·        ·     ·        ·     │\n"
    						   "│   ·      \e[%sm▄█▒▓▓▄\e[0m    \e[%sm■\e[0m            ·    ·       ·      ·         │\n"
    						   "│·     \e[%sm■\e[0m  \e[%sm▓▒░▒░▓▒█\e[0m        ·               ·     ·      ·      · │\n"
    						   "│       · \e[%sm█░▒█▒▒░▓\e[0m       ·       ·     ·         ·       ·      │\n"
    						   "│   ·  \e[%sm■\e[0m   \e[%sm▀▓░▒█▀\e[0m  ·        ·       ·       ·        ·      ·   │\n"
    						   "│       ·       ·      ·       ·   ·      ·   ·     ·      ·    │\n"
    						   "│   ·       ·    ▄▄▄·            ·    ·          ·      ·     · │\n"
    						   "│       ·      ▀▀█\e[%sm▒░\e[0m█▀       ·   ·     ·    ·   ▄█\e[%sm░░\e[0m█▄        · │\n"
    						   "│·    ·     ·   · █\e[%sm▒░\e[0m█▀  ·    ·        ▄▄█████▄█\e[%sm░\e[0m█▀▀█\e[%sm░\e[0m█▄\e[%sm▓▓▓\e[0m■ ·  │\n"
    						   "│   ·     ·  ·  ███\e[%sm▓▒░\e[0m█         ·   ·   · ▀■█▀█■▀■▄▄■▀▀█▄▄  ·   │\n"
    						   "│       ■■■■█████\e[%sm▓▓▒▒▒░\e[0m█   ·     ·        ▄■█▄█■▄■▀▀■▄▄█▀▀·    ·│\n"
    						   "│·    ·  ·      ███\e[%sm▓▒░\e[0m█        ·     · ▀▀█████▀█\e[%sm░\e[0m█▄▄█\e[%sm░\e[0m█▀\e[%sm▓▓▓\e[0m■ ·  │\n"
    						   "│   ·   ·   ·     █\e[%sm▒░\e[0m█▄    ·      ·      ·      ▀█\e[%sm░░\e[0m█▀  ·     · │\n"
    						   "│     ·    ·   ▄▄█\e[%sm▒░\e[0m█▄   ·   ·     ·     ·   ·       ·     ·    │\n"
    						   "│·  ·   ·    ·   ▀▀▀·      ·      ·      ·   ·    ·       ·     │\n",&clr[0],&clr[0],&clr[0],&clr[0],&clr[0],&clr[0],&clr[0],&atlanta.color[0],&ennemis[0].color[0],&atlanta.color[0],&ennemis[0].color[0],&ennemis[0].color[0],&ennemis[0].color[0],&atlanta.color[0],&atlanta.color[0],&atlanta.color[0],&ennemis[0].color[0],&ennemis[0].color[0],&ennemis[0].color[0],&atlanta.color[0],&ennemis[0].color[0],&atlanta.color[0]);
						break;
					case 3:
						printf("├───────────────────────────────────────────────────────────────┤\n"
    						   "│·      ·   ·     ·   ·   ·       ·        ·     ·        ·     │\n"
    						   "│   ·          ·    \e[1;90m■\e[0m          ·    ·       ·      ·\e[1;90m▀■\e[0m          │\n"
    						   "│·  \e[1;90m■\e[0m  ·     ·      ·     ·   \e[1;90m■▀\e[0m          ·     ·      ·      · │\n"
    						   "│       ·\e[1;90m▄██\e[0m    ·      ·       ·     ·          ·      ·\e[1;90m███\e[0m     │\n"
    						   "│   ·    \e[1;90m▀▀\e[0m   ·  \e[1;90m■\e[0m   ·        ·       ·   \e[1;90m██▄\e[0m ·        · \e[1;90m███\e[0m  · │\n"
    						   "│       ·       ·      ·       ·   ·    \e[1;90m ▀▀\e[0m   ·     ·      ·    │\n"
    						   "│   ·       ·    ▄▄▄·            ·      ·        ·      ·     · │\n"
    						   "│       ·      ▀▀█\e[%sm▒░\e[0m█▀      · \e[1;90m██\e[0m     ·     ·   ▄█\e[%sm░░\e[0m█▄       ·   │\n"
    						   "│·    ·     ·   · █\e[%sm▒░\e[0m█▀  ·     ·      ▄▄█████▄█\e[%sm░\e[0m█▀▀█\e[%sm░\e[0m█▄\e[%sm▓▓▓\e[0m■ ·   │\n"
    						   "│   ·     ·  ·  ███\e[%sm▓▒░\e[0m█          · ·   · ▀■█▀█■▀■▄▄■▀▀█▄▄   ·   │\n"
    						   "│       ■■■■█████\e[%sm▓▓▒▒▒░\e[0m█   ·    ·        ▄■█▄█■▄■▀▀■▄▄█▀▀·     ·│\n"
    						   "│·  \e[1;90m██\e[0m·  ·     ███\e[%sm▓▒░\e[0m█\e[0m      ·       · ▀▀█████▀█\e[%sm░\e[0m█▄▄█\e[%sm░\e[0m█▀\e[%sm▓▓▓\e[0m■  ·  │\n"
    						   "│   ·   ·   ·     █\e[%sm▒░\e[0m█▄    ·   \e[1;90m■\e[0m     ·   ·   · ▀█\e[%sm░░\e[0m█▀           │\n"
    						   "│     ·    ·   ▄▄█\e[%sm▒░\e[0m█▄   ·   ·     \e[1;90m██\e[0m·      ·   ·       ·    \e[1;90m■\e[0m  │\n"
    						   "│·  ·   ·    ·   ▀▀▀·      ·      ·  \e[1;90m▀\e[0m   ·   ·    ·       ·     │\n",&atlanta.color[0],&ennemis[0].color[0],&atlanta.color[0],&ennemis[0].color[0],&ennemis[0].color[0],&ennemis[0].color[0],&atlanta.color[0],&atlanta.color[0],&atlanta.color[0],&ennemis[0].color[0],&ennemis[0].color[0],&ennemis[0].color[0],&atlanta.color[0],&ennemis[0].color[0],&atlanta.color[0]);
						break;
					case 4:
						printf("├───────────────────────────────────────────────────────────────┤\n"
    						   "│·      ·   ·     ·   ·   ·       ·        ·     ·        ·     │\n"
    						   "│   ·          ·                ·    ·       ·      ·           │\n"
    						   "│·     ·     ·      ·     ·               ·     ·      ·      · │\n"
    						   "│       ·      ·        ·       ·     ·          ·       ·      │\n"
    						   "│   ·       ·      ·        ·       ·     ·   ·      ·      ·   │\n"
    						   "│       ·       ·      ·       ·   ·       ·      · ·      ·    │\n"
    						   "│   ·       ·    ▄▄▄·            ·    ·                ·     ·  │\n"
    						   "│       ·      ▀▀█\e[%sm▒░\e[0m█▀         ·     ·     ·   ▄█\e[%sm░░\e[0m█▄       ·   │\n"
    						   "│·    ·     ·   · █\e[%sm▒░\e[0m█▀  ·   ·        ▄▄█████▄█\e[%sm░\e[0m█▀▀█\e[%sm░\e[0m█▄\e[%sm▓▓▓\e[0m■   · │\n"
    						   "│   ·     ·  ·  ███\e[%sm▓▒░\e[0m█        ·   ·   · ▀■█▀█■▀■▄▄■▀▀█▄▄      ·│\n"
    						   "│       ■■■■█████\e[%sm▓▓▒▒▒░\e[0m█   ·     ·       ▄■█▄█■▄■▀▀■▄▄█▀▀·      │\n"
    						   "│·    ·  ·      ███\e[%sm▓▒░\e[0m█         ·   · ▀▀█████▀█\e[%sm░\e[0m█▄▄█\e[%sm░\e[0m█▀\e[%sm▓▓▓\e[0m■  ·  │\n"
    						   "│   ·   ·   ·     █\e[%sm▒░\e[0m█▄    ·       ·       ·   ▀█\e[%sm░░\e[0m█▀           │\n"
    						   "│     ·    ·   ▄▄█\e[%sm▒░\e[0m█▄   ·   ·       ·   ·   ·       ·     ·    │\n"
    						   "│·  ·   ·    ·   ▀▀▀·      ·      ·      ·   ·    ·       ·     │\n",&atlanta.color[0],&ennemis[0].color[0],&atlanta.color[0],&ennemis[0].color[0],&ennemis[0].color[0],&ennemis[0].color[0],&atlanta.color[0],&atlanta.color[0],&atlanta.color[0],&ennemis[0].color[0],&ennemis[0].color[0],&ennemis[0].color[0],&atlanta.color[0],&ennemis[0].color[0],&atlanta.color[0]);
						break;
				}
				break;
			case 6:
				switch (sub){
					case 1:
						printf("├──────────────────────────────┬────────────────────────────────┤\n"
							   "│                              │                                │\n"
							   "│ [1] Attaque                  │ [2] Esquive -------------- 2 F │\n"
							   "│                              │                                │\n"
							   "├──────────────────────────────┼────────────────────────────────┤\n"
							   "│                              │                                │\n"
							   "│ [3] Récup. bouclier ---- 5 E │ [4] Fin de Tour                │\n"
							   "│                              │                                │\n"
							   "├──────────────────────────────┼────────────────────────────────┤\n");
						if (atlanta.fuel < 10) {
							printf("│ \e[0;35mFuel ------------------- %d F \e[0m",atlanta.fuel);
						}else if (atlanta.fuel < 100) {
							printf("│ \e[0;35mFuel ------------------ %d F \e[0m",atlanta.fuel);
						}else if (atlanta.fuel < 1000) {
							printf("│ \e[0;35mFuel ----------------- %d F \e[0m",atlanta.fuel);
						}
						if (atlanta.energy < 10) {
							printf("│ \e[1;93mEnergie --------------- %d / %d \e[0m│\n",atlanta.energy, atlanta.maxEnergy);
						}else if (atlanta.energy < 100) {
							printf("│ \e[1;93mEnergie -------------- %d / %d \e[0m│\n",atlanta.energy, atlanta.maxEnergy);
						}
						printf("└──────────────────────────────┴────────────────────────────────┘\n");
						break;

				}
		}
	}
	void printMap (World * w, int monde) {
		char milieuPrint[99][99];
		system("clear");
		for (int i = 0; i < w->nbrEtape; i++){
			switch (w->type[i]){
				case 1:
					switch (w->state[i]) {
						case 0:
							strcpy(milieuPrint[i], "\e[0m○\e[0m");
							break;
						case 1:
							strcpy(milieuPrint[i], "\e[1;33m■\e[0m"); //Soleil
							break;
						case 2:
							strcpy(milieuPrint[i], "\e[0m►\e[0m");
							break;
					}
					break;
				case 2:
					switch (w->state[i]) {
						case 0:
							strcpy(milieuPrint[i], "\e[0m○\e[0m");
							break;
						case 1:
							strcpy(milieuPrint[i], "\e[0;32m■\e[0m"); //Planette
							break;
						case 2:
							strcpy(milieuPrint[i], "\e[0m►\e[0m");
							break;
					}
					break;
				case 3:
					switch (w->state[i]) {
						case 0:
							strcpy(milieuPrint[i], "\e[0m○\e[0m");
							break;
						case 1:
							strcpy(milieuPrint[i], "\e[0;35m■\e[0m"); //Asteroid
							break;
						case 2:
							strcpy(milieuPrint[i], "\e[0m►\e[0m");
							break;
					}
					break;
				case 4:
					switch (w->state[i]) {
						case 0:
							strcpy(milieuPrint[i], "\e[0m○\e[0m");
							break;
						case 1:
							strcpy(milieuPrint[i], "\e[0;34m■\e[0m"); //Vide
							break;
						case 2:
							strcpy(milieuPrint[i], "\e[0m►\e[0m");
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

	void displayAtlanta(){
		printf("				 ┌──────────────────────────────┐\n" 
			   "				 │ █▀█ ▀█▀ █   █▀█ █▄ █ ▀█▀ █▀█ │\n"
			   "				 │ █▀█  █  █▄▄ █▀█ █ ▀█  █  █▀█ │\n"
			   "┌────────────────┴──────────────────────────────┴───────────────┐\n"
			   "│");
		int nbr = (63 * atlanta.shield)/atlanta.maxShield;
		for (int i = 0; i < nbr; i++){
			printf("\e[0;94m▄");
		}
		for (int i = 0; i < 63 - nbr; i++){
			printf("\e[0;90m▄");
		}
		printf("\e[0m│\n│");
		nbr = (63 * atlanta.hull)/atlanta.maxHull;
		for (int i = 0; i < nbr; i++){
			printf("\e[0;93m▄\e[0m");
		}
		for (int i = 0; i < 63 - nbr; i++){
			printf("\e[0;90m▄\e[0m");
		}
		printf("│\n│");
		for (int i = 0; i < nbr; i++){
			printf("\e[0;93m▀\e[0m");
		}
		for (int i = 0; i < 63 - nbr; i++){
			printf("\e[0;90m▀\e[0m");
		}
		printf("│\n");

		//63 de long
		//\e[0;94m shield _____ \e[0;93m vie
	}


	void displayEnnemy(Vaisseau * v){
		int count = 0;
		//strcpy(lenV, (*v).nom);
		int length = strlen(v->nom) - 4;
		int space = 65/2 - length/2;
		printf("├");
		for (int i = 0; i<space - 1; i++){
		  printf("─");
		}
		printf("┬");
		for (int i = 0; i<length - 2; i++){
		  printf("─");
		}
		printf("┬");
		for (int i = 0; i<65 - space - length - 1; i++){
		  printf("─");
		}
		printf("┤\n│");
		for (int i = 0; i<space - 1; i++){
		  printf(" ");
		}
		printf("%s",v->nom);
		for (int i = 0; i<65 - space - length - 1; i++){
		  printf(" ");
		}
		printf("│\n├");
		for (int i = 0; i<space - 1; i++){
	  		printf("─");
		}
		printf("┴");
		for (int i = 0; i<length - 2; i++){
		  printf("─");
		}
		printf("┴");
		for (int i = 0; i<65 - space - length - 1; i++){
	   		printf("─");
	 	}
	 	printf("┤\n");
	 	int nbr = (57 * v->shield)/v->maxShield;
	 	printf("│   ");
		for (int i = 0; i < nbr; i++){
			printf("\e[0;94m▄");
		}
		for (int i = 0; i < 57 - nbr; i++){
			printf("\e[0;90m▄");
		}
		printf("\e[0m   │\n│   ");
		nbr = (57 * v->hull)/v->maxHull;
		for (int i = 0; i < nbr; i++){
			printf("\e[0;91m▄\e[0m");
		}
		for (int i = 0; i < 57 - nbr; i++){
			printf("\e[0;90m▄\e[0m");
		}
		printf("   │\n│   ");
		for (int i = 0; i < nbr; i++){
			printf("\e[0;91m▀\e[0m");
		}
		for (int i = 0; i < 57 - nbr; i++){
			printf("\e[0;90m▀\e[0m");
		}
		printf("   │\n");
	}

	void combat(int ennemyId, int etape){
		int tour = 1;
		int choix = 0;
		ennemis[ennemyId].hull = ennemis[ennemyId].maxHull;
		ennemis[ennemyId].shield = ennemis[ennemyId].maxShield;
		ennemis[ennemyId].energy = ennemis[ennemyId].maxEnergy/2;
		atlanta.energy = atlanta.maxEnergy/2;
		while ((atlanta.hull > 0) || (ennemis[ennemyId].hull > 0)){
			choix = 0;
			while (tour == 1){
				switch (choix){
					case 0:
						system("clear");
						displayAtlanta();
						displayEnnemy(&ennemis[ennemyId]);
						display(5,etape);
						display(6,1);
						scanf("%d",&choix);
						break;
					case 1:
						//attaque
						system("clear");
						displayAtlanta();
						displayEnnemy(&ennemis[ennemyId]);
						display(5,etape);
						display(3,2);
						scanf("%d",&choix);
						break;
					case 2:
						//esquive
						system("clear");
						break;
					case 3:
						//shield
						system("clear");
						break;
					case 4:
						//Fin de tour
						system("clear");
						tour = 0;
						choix = 0;
						break;
					default:
						printf("Choix Invalide !");
						scanf("%d",&choix);
						break;
				}
			}
			//Display encounter
		//menu display combat
			//voir Etat Coque + Bouclier
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
		}
		if (atlanta.hull <= 0){
			//print GameOver
			//Quit game
		}
	}

	void menuVaisseau() {
		int testMenu = 0;
		int choix = 0;
		//system("clear");
		displayAtlanta();
		display(3,1);
		scanf("%d",&choix);
		while (testMenu == 0){
			switch (choix) {
				case 1://Armes
					system("clear");
					displayAtlanta();
					display(3,2);
					scanf("%d",&choix);
					system("clear");
					displayAtlanta();
					switch (choix){
						case 0 : 
							choix = 0;
							system("clear");
							menuVaisseau();
							break;
						case 1 :
							printMenu(atlanta.weapon1 + 100, "0");
							break;
						case 2 :
							printMenu(atlanta.weapon2 + 100, "0");
							break;
						case 3 :
							printMenu(atlanta.weapon3 + 100, "0");
							break;
					}
					scanf("%d",&choix);
					system("clear");
					displayAtlanta();
					choix = 1;
					break;
				case 2:
					system("clear");
					displayAtlanta();
					display(3,3);
					for (int i; i<99; i++){
						if (atlanta.sys[i] == 1){
							printMenu(31 + i, "   ");
							display(3,4);
						}
					}
					display(3,5);
					scanf("%d",&choix);
					system("clear");
					menuVaisseau();
					break;
				case 3:
					system("clear");
					displayAtlanta();
					display(3,6);
					scanf("%d",&choix);
					system("clear");
					menuVaisseau();
					break;
				case 4:
					testMenu = 1;
					system("clear");
					break;
				default:
					printf("Choix Invalide\n");
					scanf("%d",&choix);
					break;
			}
		}
	}

	void changeWeapon(int id, int price){
		system("clear");
		int choix = 0;
		int test = 0;
		int test1 = 0;
		display(3,2);
		while (test == 0){
			scanf("%d",&choix);
			switch (choix){
				case 0:
					if (test1 == 0){
						test = 1;
						atlanta.flouze += price;
						system("clear");
					}else{
						test = 1;
						system("clear");
					}
					break;
				case 1:
					if (atlanta.weapon1 != 0){
						system("clear");
						atlanta.weapon1 = id;
						display(3,2);
						test1 = 1;
					}else{
						printf("Choix Invalide\n");
					}
					break;
				case 2:
					if (atlanta.weapon2 != 0){
						system("clear");
						atlanta.weapon2 = id;
						display(3,2);
						test1 = 1;
					}else{
						printf("Choix Invalide\n");
					}
					break;
				case 3:
					if (atlanta.weapon3 != 0){
						system("clear");
						atlanta.weapon3 = id;
						display(3,2);
						test1 = 1;
					}else{
						printf("Choix Invalide\n");
					}
					break;
				default:
					printf("Choix Invalide\n");
					break;
			}
		}
	}

	void menuShop(int pool){
		//autant de variables pour les arborescences parceque si l'on quitte la fonction, de nouveaux items seront générés
		int choix = 0;
		int choix1 = 0;
		int test = 0;
		int test1 = 0;
		int test2 = 1;
		int test3 = 0;
		int level[][12] = {{1,1,1,1,1,1,1,1,3,3,4,4},
						   {0,0,0,0,0,0,0,0,0,0,0,0},
						   {0,0,0,0,0,0,0,0,0,0,0,0},
						   {0,0,0,0,0,0,0,0,0,0,0,0},
						   {0,0,0,0,0,0,0,0,0,0,0,0},
						   {0,0,0,0,0,0,0,0,0,0,0,0},
						   {0,0,0,0,0,0,0,0,0,0,0,0},
						   {0,0,0,0,0,0,0,0,0,0,0,0}};
		   //Items pool    //Weapons       ║Systems
		int randWeapon[3];
		int randSys[2];
		//Afficher 3 armes et 2 systèmes + constante de fuel
		for (int i = 0; i<3; i++){
			int testShop = 0;
			while (testShop == 0){
				randWeapon[i] = random_nbr(0,7);
				if ((randWeapon[i] != randWeapon[i-1]) && (randWeapon[i] != randWeapon[i-2])){
					testShop = 1;
				}
			}
		}
		for (int i = 0; i<2; i++){
			int testShop = 0;
			while (testShop == 0){
				randSys[i] = random_nbr(8,11);
				if (randSys[i] != randSys[i-1]){
					testShop = 1;
				}
			}
		}
		while (test == 0){
			test1 = 0;
			displayAtlanta();
			display(4,1);
			scanf("%d",&choix);
			system("clear");
			while (test1 == 0){
				choix1 = 0;
				test = 0;
				test1 = 0;
				test2 = 0;
				test3 = 1;
				switch (choix){
					case 1:
						displayAtlanta();
						display(4,2);
						printf("│ ");
						printCustomChar(armes[level[pool][randWeapon[0]]].nom, 15, 0, " ");
						printf("│ [1] Infos │ [2] ");	
						printCustomChar(armes[level[pool][randWeapon[0]]].priceChar, 23, 1, "-");
						printf(" § │\n");
						display(4,3);
						printf("│ ");
						printCustomChar(armes[level[pool][randWeapon[1]]].nom, 15, 0, " ");
						printf("│ [3] Infos │ [4] ");	
						printCustomChar(armes[level[pool][randWeapon[1]]].priceChar, 23, 1, "-");
						printf(" § │\n");
						display(4,3);
						printf("│ ");
						printCustomChar(armes[level[pool][randWeapon[2]]].nom, 15, 0, " ");
						printf("│ [5] Infos │ [6] ");	
						printCustomChar(armes[level[pool][randWeapon[0]]].priceChar, 23, 1, "-");
						printf(" § │\n");
						display(4,4);
						if (atlanta.flouze < 10) {
							printf("│ \e[1;93mCrédits ------------------------------------------------- %d § \e[0m│\n",atlanta.flouze);
						}else if (atlanta.flouze < 100) {
							printf("│ \e[1;93mCrédits ------------------------------------------------ %d § \e[0m│\n",atlanta.flouze);
						}else if (atlanta.flouze < 1000) {
							printf("│ \e[1;93mCrédits ----------------------------------------------- %d § \e[0m│\n",atlanta.flouze);
						}else if (atlanta.flouze < 10000){
							printf("│ \e[1;93mCrédits ---------------------------------------------- %d § \e[0m│\n",atlanta.flouze);
						}
						printf("├───────────────────────────────────────────────────────────────┤\n");
						display(3,5);
						while (test2 == 0){
							scanf("%d",&choix1);
							switch (choix1) {
								case 0:
									test1 = 1;
									test2 = 1;
									system("clear");
									break;
								case 1:
									system("clear");
									while (test3 != 0){
										printMenu(level[pool][randWeapon[0]] + 100, "0");
										scanf("%d",&test3);
										system("clear");
									}
									test2 = 1;
									choix1 = 0;
									break;
								case 2:
									if (atlanta.flouze >= armes[level[pool][randWeapon[0]]].price){
										atlanta.flouze -= armes[level[pool][randWeapon[0]]].price;
										//function set weapon
										changeWeapon(armes[level[pool][randWeapon[0]]].id, armes[level[pool][randWeapon[0]]].price);
										test2 = 1;
										choix1 = 0;
									}else{
										printf("Pas assez de Crédits !\n");
									}
									break;
								case 3:
									system("clear");
									while (test3 != 0){
										printMenu(level[pool][randWeapon[1]] + 100, "0");
										scanf("%d",&test3);
										system("clear");
									}
									test2 = 1;
									choix1 = 0;
									break;
								case 4:
									if (atlanta.flouze >= armes[level[pool][randWeapon[1]]].price){
										atlanta.flouze -= armes[level[pool][randWeapon[1]]].price;
										//function set weapon
										changeWeapon(armes[level[pool][randWeapon[1]]].id, armes[level[pool][randWeapon[0]]].price);
									}else{
										printf("Pas assez de Crédits !\n");
										test2 = 1;
										choix1 = 0;
									}
									break;
								case 5:
									system("clear");
									while (test3 != 0){
										printMenu(level[pool][randWeapon[2]] + 100, "0");
										scanf("%d",&test3);
										system("clear");
									}
									test2 = 1;
									choix1 = 0;
									break;
								case 6:
									if (atlanta.flouze >= armes[level[pool][randWeapon[2]]].price){
										atlanta.flouze -= armes[level[pool][randWeapon[2]]].price;
										//function set weapon
										changeWeapon(armes[level[pool][randWeapon[2]]].id, armes[level[pool][randWeapon[0]]].price);
										test2 = 1;
										choix1 = 0;
									}else{
										printf("Pas assez de Crédits !\n");
									}
									break;
							}
						}
						break;
					case 2:
						displayAtlanta();
						display(4,5);
						for (int i = 0; i<2; i++){
							printSystemShop(level[pool][randSys[i]],i+1);
						}
						display(3,4);
						display(3,5);
						scanf("%d",&choix1);
						switch (choix1){
							case 0:
								choix = 0;
								test1 = 1;
								system("clear");
								break;
							case 1:
								if (atlanta.flouze >= 500){
									atlanta.sys[level[pool][randSys[0]]] = 1;
									atlanta.flouze -= 500;
									system("clear");
								}else{
									printf("Pas assez de Crédits !\n");
								}
								break;
							case 2:
								if (atlanta.flouze >= 500){
									atlanta.sys[level[pool][randSys[1]]] = 1;
									atlanta.flouze -= 500;
									system("clear");
								}else{
									printf("Pas assez de Crédits !\n");
								}
								break;
						}
						break;
					case 3:
						if (atlanta.flouze >= 20){
							atlanta.flouze -= 20;
							atlanta.fuel += 2;
						}else{
							printf("Pas assez de Crédits !\n");
						}
						choix = 0;
						test1 = 1;
						break;
					case 4:
						test = 1;
						test1 = 1;
						break;
					default:
						choix = 0;
						test1 = 1;
						break;
				}
			}
		}
		//voir items en vente ( choisis aléatoirement en fonction d'un pool propre à l'avancée et au lieu)
		//voir caractéristiques des armes et objets
		printf("quitté");
	}
	//Menu scenario
	void scenario(int niveau, int milieu, int index){
		int choix = 0;
		switch (niveau){
			case 1 :
				switch (milieu){
					case 1 :
						switch (index){
							case 1:
								combat(0,milieu);
								//scanf("%d",&choix);
								//print
								//choix ou arboressence de choix
									//output -> combat, shop, récolte ou perte
								//menu vaisseau
								//Id du milieu : World.type[milieu]
								break;
						}
						break;
					case 2:
						strcpy(&clr[0], "1;92");
						switch (index){
							case 1:
								combat(0,milieu);
								break;
						}
						break;
					case 3:
						combat(0,milieu);
						break;
					case 4:
						combat(0,milieu);
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
		return (suiv);
	}

	void niveau(Route * route, int monde){
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
			currentEtape = navigation(&world,&etape[currentEtape],currentEtape,monde);
			//navigation
			int alea = random_nbr(1,1);
			scenario(1,world.type[currentEtape],alea);
			//check end
		}
	}

	void generateMap(int monde){
		int r = 0;
		Route route[99];
		route[1] = ( struct Route ) {1,{1,2,3,0,4,0,5,6,7,0,8,0,9,0,9,0,10,0,10,11,12,13,14,15,16,0,16,0,17,0,17,0,18,0,18,0,0,0},38};
		route[2] = ( struct Route ) {2,{1,2,3,0,4,5,6,7,7,0,8,9,10,11,12,0,12,0,13,0,14,0,14,15,16,0,16,0,17,0,18,19,19,0,20,21,20,21,22,0,23,0,23,0,23,0,0,0},48};
		route[3] = (struct Route ) {3,{1,2,3,4,4,5,6,0,6,7,8,9,10,11,12,0,13,0,13,0,14,15,15,0,16,17,16,17,18,0,19,0,19,0,20,0,21,0,22,23,23,0,24,0,24,0,24,0,0,0},50};
		route[4] = (struct Route ) {4,{1,0,2,3,4,0,5,6,7,8,9,0,9,0,10,0,11,0,11,0,12,0,12,0,13,14,15,0,15,0,0,0},32};
		r = random_nbr(1,4);
		niveau(&route[r], monde);
		//Niveau --> Debut d'un cycle
	}


	int main(){
		srand(time(NULL));
		int end = 0;
		//menuVaisseau();
		//menuShop(0);
		//displayAtlanta();
		generateMap(1);
		//cycle
		/*while (end == 0){
			//generate map
			
		}*/
	return 0;
	}