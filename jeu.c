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
		int shieldGain;
		int energy;
		int maxEnergy;
		int energyGain;
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
		int value;
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
		int used;
		int nrj;
	};
	typedef struct Weapon Weapon;
	
	//Definit l'agencement des etapes selon un pattern prédéterminé
	struct Route{
		int id;
		int road[99];
		int nbr;
	};
	typedef struct Route Route;
					//id ;nom ;wp1;wp2;wp3;hull;mxHull;shield;mxShield;shieldGain;nrj;mxNrj;nrjGain;sp ; F ; § ; sys ; clr
	Vaisseau atlanta = {1,"SS Atlanta", 1, 0, 0, 50, 50, 20, 20, 0, 10, 20, 0, 12, 600, {1,1,1,0,0}, "0;44"};

	Vaisseau ennemis[99] = {{0,"│ Eclaireur Pyrien │", 1, 0, 0, 40, 40, 10, 10, 5, 10, 10, 5, 4, 11, {0}, "0;101"},
							{1,"│ Satellite │", 1, 0, 0, 40, 40, 20, 20, 5, 10, 10, 5, 2, 12, {0}, "43"},
							{2,"│ Eclaireur Pyrien │", 2, 0, 0, 40, 40, 10, 10, 5, 10, 10, 5, 1, 15, {0}, "0;101"},
							{3,"│ Vagabond │", 2, 0, 0, 20, 20, 40, 40, 5, 10, 10, 5, 2, 13, {0}, "0;101"},
							{4,"│ Drone de Recherches │", 3, 0, 0, 40, 40, 10, 10, 5, 10, 10, 5, 1, 11, {0}, "0;101"},
							{5,"│ Eclaireur Pyrien │", 3, 0, 0, 40, 40, 10, 10, 5, 10, 10, 5, 3, 14, {0}, "0;101"},
							{6,"│ Tellordiens │", 4, 0, 0, 15, 15, 30, 30, 10, 10, 10, 5, 2, 12, {0}, "0;101"},
							{7,"│ Eclaireur Pyrien │", 4, 0, 0, 40, 40, 10, 10, 5, 10, 10, 5, 4, 5, {0}, "0;101"}
						};

	Crew james = {0, 1, 5};
	Crew peter = {0, 1, 5};
	Crew victor = {0, 1, 5};
	Crew qtal = {0, 1, 5};

	Weapon armes[99] = {{0, "", 0, 0, 0, 0, "", -1, 0},{1, "Cannon à Ions  ", 10 /*2 digits*/, 2, 80, 150, "150", 0, 3},{2, "Vieux Cannon", 20 , 1, 60, 100, "100", 0, 3},{3, "Gatling", 10 , 5, 40, 160, "160", 0, 3},{4, "Vieux Missile", 15 , 1, 100, 150, "150", 0, 3},//tier D
													   {5, "Cannon à Pulsion", 20, 3, 50, 200, "220", 0, 4},{6, "Gatling à Proton", 20, 5, 40, 200, "200", 0, 4},{7, "Laser ", 40, 1, 60, 200, "200", 0, 4},{8, "Cannon XM1", 15, 2, 80, 190, "190", 0, 4},//tier C
													   //{5, "Cannon à impulsion", 20, 3, 50, 200, "220", 0, 4},{6, "Gatling à Proton", 20, 5, 40, 200, "200", 0, 4},{7, "Laser ", 40, 1, 60, 200, "200", 0, 4},{8, "Cannon XM1", 15, 2, 80, 190, "190", 0, 4},
													};

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
			printf("%s", &array[0]);
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
				printf("│ [%d] \e[40mExtension d'arme 2\e[0m -------------------------------- 300 § │\n",sub);
				break;
			case 4:
				printf("│ [%d] \e[40mExtension d'arme 3\e[0m -------------------------------- 300 § │\n",sub);
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
				printf("│ [%s] \e[1;92mCannon à Ions\e[0m    │ \e[3mCe cannon électrique est capable de    \e[0m│\n"
					   "│  Coût Energie : 3    │ \e[3mtranspercer n'importe quel blindage !  \e[0m│\n", &nbr[0]);
				break;
			case 2:
				printf("│ [%s] \e[1;92mVieux Cannon\e[0m     │ \e[3mCe cannon date d'une autre époque mais \e[0m│\n"
					   "│  Coût Energie : 3    │ \e[3mil fonctionne encore !                 \e[0m│\n", &nbr[0]);
				break;
			case 3:
				printf("│ [%s] \e[1;92mGatling\e[0m          │ \e[3mUne arme normalement terrestre qui     \e[0m│\n"
					   "│  Coût Energie : 3    │ \e[3mfonctionne tout de même dans l'espace  \e[0m│\n", &nbr[0]);
				break;
			case 4:
				printf("│ [%s] \e[1;92mVieux Missile\e[0m    │ \e[3mCe lance missile ne fait que peu de    \e[0m│\n"
					   "│  Coût Energie : 3    │ \e[3mdégats mais il ne rate jamais sa cible \e[0m│\n", &nbr[0]);
				break;
			case 5:
				printf("│ [%s] \e[0;32mCannon à Pulsion\e[0m │ \e[3mCe cannon fonctionne par pulsions      \e[0m│\n"
					   "│  Coût Energie : 4    │ \e[3mélectro-magnétiques                    \e[0m│\n", &nbr[0]);
				break;
			case 6:
				printf("│ [%s] \e[0;32mGatling à Photon\e[0m │ \e[3mCette gatling fonctionne grâce à la    \e[0m│\n"
					   "│  Coût Energie : 4    │ \e[3mlumière émise par ses propres tirs !   \e[0m│\n", &nbr[0]);
				break;
			case 7:
				printf("│ [%s] \e[0;32mLase\e[0m             │ \e[3mCette arme produit un rayon laser      \e[0m│\n"
					   "│  Coût Energie : 4    │ \e[3mtrès puissant mais difficile à orienter\e[0m│\n", &nbr[0]);
				break;
			case 8:
				printf("│ [%s] \e[0;32mCannon XM1      \e[0m │ \e[3mPremier cannon de la gamme XM,         \e[0m│\n"
					   "│  Coût Energie : 4    │ \e[3mabordable et équilibré                \e[0m│\n", &nbr[0]);
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
					   "│   \e[1;92mCannon à Ions\e[0m   │\n"
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
			case 102 :
				printf("┌───────────────────┐\n"
					   "│    \e[1;92mVieux Cannon\e[0m   │\n"
					   "├───────────────────┤\n"
					   "│      Attaque      │\n" 
					   "├───────────────────┤\n"
					   "│   ♦  ○  ○  ○  ○   │\n"
					   "├───────────────────┤\n"
					   "│     Precision     │\n"
					   "├───────────────────┤\n"
					   "│   ♦  ♦  ♦  ○  ○   │\n"
					   "├───────────────────┤\n"
					   "│   Nombre de Tirs  │\n"
					   "├───────────────────┤\n"
					   "│   ♦  ○  ○  ○  ○   │\n"
					   "├───────────────────┤\n"
					   "│ [0] Retour        │\n"
					   "└───────────────────┘\n");
				break;
			case 103 :
				printf("┌───────────────────┐\n"
					   "│      \e[1;92mGatling\e[0m      │\n"
					   "├───────────────────┤\n"
					   "│      Attaque      │\n" 
					   "├───────────────────┤\n"
					   "│   ♦  ○  ○  ○  ○   │\n"
					   "├───────────────────┤\n"
					   "│     Precision     │\n"
					   "├───────────────────┤\n"
					   "│   ♦  ♦  ♦  ○  ○   │\n"
					   "├───────────────────┤\n"
					   "│   Nombre de Tirs  │\n"
					   "├───────────────────┤\n"
					   "│   ♦  ♦  ♦  ♦  ♦   │\n"
					   "├───────────────────┤\n"
					   "│ [0] Retour        │\n"
					   "└───────────────────┘\n");
				break;
			case 104 :
				printf("┌───────────────────┐\n"
					   "│   \e[1;92mVieux Missile\e[0m   │\n"
					   "├───────────────────┤\n"
					   "│      Attaque      │\n" 
					   "├───────────────────┤\n"
					   "│   ♦  ○  ○  ○  ○   │\n"
					   "├───────────────────┤\n"
					   "│     Precision     │\n"
					   "├───────────────────┤\n"
					   "│   ♦  ♦  ♦  ♦  ♦   │\n"
					   "├───────────────────┤\n"
					   "│   Nombre de Tirs  │\n"
					   "├───────────────────┤\n"
					   "│   ♦  ○  ○  ○  ○   │\n"
					   "├───────────────────┤\n"
					   "│ [0] Retour        │\n"
					   "└───────────────────┘\n");
				break;
			case 105 :
				printf("┌───────────────────┐\n"
					   "│ \e[0;32mCannon à Pulsion\e[0m│\n"
					   "├───────────────────┤\n"
					   "│      Attaque      │\n" 
					   "├───────────────────┤\n"
					   "│   ♦  ○  ○  ○  ○   │\n"
					   "├───────────────────┤\n"
					   "│     Precision     │\n"
					   "├───────────────────┤\n"
					   "│   ♦  ♦  ○  ○  ○   │\n"
					   "├───────────────────┤\n"
					   "│   Nombre de Tirs  │\n"
					   "├───────────────────┤\n"
					   "│   ♦  ♦  ♦  ○  ○   │\n"
					   "├───────────────────┤\n"
					   "│ [0] Retour        │\n"
					   "└───────────────────┘\n");
				break;
			case 106 :
				printf("┌───────────────────┐\n"
					   "│  \e[0;32mGatling à Proton\e[0m │\n"
					   "├───────────────────┤\n"
					   "│      Attaque      │\n" 
					   "├───────────────────┤\n"
					   "│   ♦  ♦  ♦  ○  ○   │\n"
					   "├───────────────────┤\n"
					   "│     Precision     │\n"
					   "├───────────────────┤\n"
					   "│   ♦  ○  ○  ○  ○   │\n"
					   "├───────────────────┤\n"
					   "│   Nombre de Tirs  │\n"
					   "├───────────────────┤\n"
					   "│   ♦  ♦  ♦  ♦  ♦   │\n"
					   "├───────────────────┤\n"
					   "│ [0] Retour        │\n"
					   "└───────────────────┘\n");
				break;
			case 107 :
				printf("┌───────────────────┐\n"
					   "│       \e[0;32mLaser\e[0m       │\n"
					   "├───────────────────┤\n"
					   "│      Attaque      │\n" 
					   "├───────────────────┤\n"
					   "│   ♦  ♦  ♦  ○  ○   │\n"
					   "├───────────────────┤\n"
					   "│     Precision     │\n"
					   "├───────────────────┤\n"
					   "│   ♦  ♦  ♦  ○  ○   │\n"
					   "├───────────────────┤\n"
					   "│   Nombre de Tirs  │\n"
					   "├───────────────────┤\n"
					   "│   ♦  ○  ○  ○  ○   │\n"
					   "├───────────────────┤\n"
					   "│ [0] Retour        │\n"
					   "└───────────────────┘\n");
				break;
			case 108 :
				printf("┌───────────────────┐\n"
					   "│     \e[0;32mCannon XM1\e[0m    │\n"
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

	void displayAtlanta(){
		printf("                 ┌──────────────────────────────┐\n" 
			   "                 │ █▀█ ▀█▀ █   █▀█ █▄ █ ▀█▀ █▀█ │\n"
			   "                 │ █▀█  █  █▄▄ █▀█ █ ▀█  █  █▀█ │\n"
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
		displayAtlanta();
		printf("├───────────────────────────────────────────────────────────────┤\n"
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

	void drops(ennemyId, etape){
		int test = 1;
		system("clear");
		displayAtlanta();
		display(5,etape);
		printf("├───────────────────────────────────────────────────────────────┤\n"
			   "│                                                               │\n");
		atlanta.fuel += ennemis[ennemyId].fuel;
		atlanta.flouze += ennemis[ennemyId].flouze;
		if (ennemis[ennemyId].fuel < 10 && ennemis[ennemyId].flouze < 10) {
			printf("│ * Vous récoltez %d Fuel et %d Crédits dans la carcasse du       │\n",ennemis[ennemyId].fuel,ennemis[ennemyId].flouze);
		}else if (ennemis[ennemyId].fuel > 10 && ennemis[ennemyId].flouze > 10){
			printf("│ * Vous récoltez %d Fuel et %d Crédits dans la carcasse du     │\n",ennemis[ennemyId].fuel,ennemis[ennemyId].flouze);
		}else{
			printf("│ * Vous récoltez %d Fuel et %d Crédits dans la carcasse du      │\n",ennemis[ennemyId].fuel,ennemis[ennemyId].flouze);
		}
		printf("│   vaisseau ennemis !                                          │\n");
		int xp = random_nbr(2,6);
		printf("│ * James remporte %d xp !                                       │\n", xp);
		james.xp += xp;
		if (james.xp > 35){
			james.xp -= 35;
			james.level += 1;
			james.value += 5;
			printf("│ * James gagne un niveau ! L'efficacité du bouclier augmente ! │\n");
		}
		printf("│                                                               │\n");
		xp = random_nbr(2,6);
		printf("│ * Peter remporte %d xp !                                       │\n", xp);
		peter.xp += xp;
		if (peter.xp > 35){
			peter.xp -= 35;
			peter.level += 1;
			peter.value += 2;
			printf("│ * Peter gagne un niveau ! La génération d'énergie augmente !  │\n");
		}
		printf("│                                                               │\n");
		xp = random_nbr(2,6);
		printf("│ * Victor remporte %d xp !                                      │\n", xp);
		victor.xp += xp;
		if (victor.xp > 35){
			victor.xp -= 35;
			victor.level += 1;
			victor.value += 5;
			printf("│ * Victor gagne un niveau ! La précision des tirs augmente !   │\n");
		}
		printf("│                                                               │\n");
		xp = random_nbr(2,6);
		printf("│ * Qk'tal remporte %d xp !                                      │\n", xp);
		victor.xp += xp;
		if (victor.xp > 35){
			victor.xp -= 35;
			victor.level += 1;
			victor.value += 5;
			printf("│ * Qk'tal gagne un niveau ! Les chances d'esquive augmentent !  │\n");
		}
		printf("│                                                               │\n"
			   "├───────────────────────────────────────────────────────────────┤\n"
			   "│ [0] Suite                                                     │\n"
			   "└───────────────────────────────────────────────────────────────┘\n");
		while (test != 0){
			scanf("%d",&test);
		}
	}
//int id;	char nom[30];	int atk;	int nbr_fire;	int accuracy;	int price;	char priceChar[5];
	void attaque(int wpId, int etape, int cible, int ennemyId, int esquive){
		system("clear");
		displayAtlanta();
		displayEnnemy(&ennemis[ennemyId]);
		display(5,etape);
		printf("├───────────────────────────────────────────────────────────────┤\n"
			   "│                                                               │\n");
		for (int i = 0; i < armes[wpId].nbr_fire; i++){
			int r = random_nbr(0, 100);
			if (cible != -1){
				if (r <= (armes[wpId].accuracy + victor.value)){
					ennemis[ennemyId].shield -= armes[wpId].atk;
					if (ennemis[ennemyId].shield < 0){
						ennemis[ennemyId].hull += ennemis[ennemyId].shield;
						ennemis[ennemyId].shield = 0;
					}
					printf("│ \e[0;92m* L'ennemis prends %d points de dégats !\e[0m                      │\n",armes[wpId].atk);
				}else{
					printf("│ \e[0;91m* Le tir à raté !\e[0m                                             │\n");
				}
			}else{
				if (r <= (armes[wpId].accuracy - qtal.value - esquive)){
					atlanta.shield -= armes[wpId].atk;
					if (atlanta.shield < 0){
						atlanta.hull += atlanta.shield;
						atlanta.shield = 0;
					}
					printf("│ \e[0;91m* Vous prennez %d points de dégats !\e[0m                          │\n",armes[wpId].atk);
				}else{
					printf("│ \e[0;92m* Ce tir à raté !\e[0m                                             │\n");
				}
			}
			printf("│                                                               │\n");
		}
		display(3,4);
		display(3,5);
		int choix = 1;
		while (choix != 0){
			scanf("%d", &choix);
		}
	}

	void combat(int ennemyId, int etape){
		int tour = 1;
		int choix = 0;
		int choix1 = 0;
		int test = 0;
		int dodge = 0;
		ennemis[ennemyId].hull = ennemis[ennemyId].maxHull;
		ennemis[ennemyId].shield = ennemis[ennemyId].maxShield;
		ennemis[ennemyId].energy = ennemis[ennemyId].maxEnergy/2;
		atlanta.energy = atlanta.maxEnergy/2;
		while ((atlanta.hull > 0) && (ennemis[ennemyId].hull > 0)){
			choix = 0;
			tour = 1;
			if (armes[atlanta.weapon1].used == 1){
			 	armes[atlanta.weapon1].used = 0;
			}
			if (armes[atlanta.weapon2].used == 1){
			 	armes[atlanta.weapon2].used = 0;
			}
			if (armes[atlanta.weapon3].used == 1){
				armes[atlanta.weapon3].used = 0;
			}
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
						test = 0;
						system("clear");
						displayAtlanta();
						displayEnnemy(&ennemis[ennemyId]);
						display(5,etape);
						display(3,2);
						while (test == 0){
							scanf("%d",&choix1);
							switch (choix1){
								case 0:
									test = 1;
									choix = 0;
									break;
								case 1:
									if (armes[atlanta.weapon1].used == 0){
										if (atlanta.energy >= armes[atlanta.weapon1].nrj){
											attaque(atlanta.weapon1, etape, ennemyId, ennemyId, 0);
											armes[atlanta.weapon1].used = 1;
											atlanta.energy -= armes[atlanta.weapon1].nrj;
											choix = 0;
											test = 1;
										}else{
											printf("Pas assez d'énergie !\n");
										}
									}else{
										printf("Vous ne pouvez pas utiliser deux fois la même arme dans le même tour !\n");
									}
									break;
								case 2:
									if (armes[atlanta.weapon2].used == 0){
										if (atlanta.energy >= armes[atlanta.weapon2].nrj){
											attaque(atlanta.weapon2, etape, ennemyId, ennemyId, 0);
											armes[atlanta.weapon2].used = 1;
											atlanta.energy -= armes[atlanta.weapon2].nrj;
											choix = 0;
											test = 1;
										}else{
											printf("Pas assez d'énergie !\n");
										}
									}else if (armes[atlanta.weapon2].used == -1){
										printf("Choix Invalide\n");
									}else{
										printf("Vous ne pouvez pas utiliser deux fois la même arme dans le même tour !\n");
									}
									break;
								case 3:
									if (armes[atlanta.weapon3].used == 0){
										if (atlanta.energy >= armes[atlanta.weapon3].nrj){
											attaque(atlanta.weapon3, etape, ennemyId, ennemyId, 0);
											armes[atlanta.weapon3].used = 1;
											atlanta.energy -= armes[atlanta.weapon3].nrj;
											choix = 0;
											test = 1;
										}else{
											printf("Pas assez d'énergie !\n");
										}
									}else if (armes[atlanta.weapon3].used == -1){
										printf("Choix Invalide\n");
									}else{
										printf("Vous ne pouvez pas utiliser deux fois la même arme dans le même tour !\n");
									}
									break;
								default:
									printf("Choix Invalide\n");
									break;
							}
						}
						if (ennemis[ennemyId].hull <= 0){
							drops(ennemyId, etape);
							return;
						}
						break;
					case 2:
						//esquive
						if (dodge == 0){
							dodge = qtal.value * 2;
							atlanta.fuel -= 2;
							system("clear");
							choix = 0;
						}else{
							printf("Choix impossible\n");
							choix = 0;
						}
						break;
					case 3:
						//shield
						system("clear");
						displayAtlanta();
						displayEnnemy(&ennemis[ennemyId]);
						display(5,etape);
						printf("├───────────────────────────────────────────────────────────────┤\n"
			   			       "│                                                               │\n"
			   			       "│  \e[0;92m* Le Bouclier se régénèrera davantage ce tour-ci !\e[0m           │\n"
			   			       "│                                                               │\n"
			   			       "│                                                               │\n"
			   			       "│                                                               │\n"
			   			       "├───────────────────────────────────────────────────────────────┤\n"
			   			       "│ [0] Retour                                                    │\n"
			   			       "└───────────────────────────────────────────────────────────────┘\n");
						atlanta.shieldGain += atlanta.shield/2;
						atlanta.energy -= 5;
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
			//tour ennemis
			int cumul = 0;
			for (int i = 3; i > 0; i--){
				system("clear");
				if (i == 3){
					if (ennemis[ennemyId].energy >= armes[ennemis[ennemyId].weapon3].nrj && (armes[ennemis[ennemyId].weapon3].nrj != 0)) {
						attaque(ennemis[ennemyId].weapon3, etape, -1, ennemyId, dodge);
						ennemis[ennemyId].energy -= armes[ennemis[ennemyId].weapon3].nrj;
					}else{
						cumul += 1;
					}
				}else if (i == 2){
					if (ennemis[ennemyId].energy >= armes[ennemis[ennemyId].weapon2].nrj && (armes[ennemis[ennemyId].weapon2].nrj != 0)) {
						attaque(ennemis[ennemyId].weapon2, etape, -1, ennemyId, dodge);
						ennemis[ennemyId].energy -= armes[ennemis[ennemyId].weapon2].nrj;
					}else{
						cumul += 1;
					}
				}else if (i == 1){
					if (ennemis[ennemyId].energy >= armes[ennemis[ennemyId].weapon1].nrj && (armes[ennemis[ennemyId].weapon1].nrj != 0)) {
						attaque(ennemis[ennemyId].weapon1, etape, -1, ennemyId, dodge);
						ennemis[ennemyId].energy -= armes[ennemis[ennemyId].weapon1].nrj;
					}else{
						cumul += 1;
					}
				}
			}
			dodge = 0;
			if (cumul == 3){
				system("clear");
				displayAtlanta();
				displayEnnemy(&ennemis[ennemyId]);
				display(5,etape);
				printf("├───────────────────────────────────────────────────────────────┤\n"
			   		   "│                                                               │\n"
			   		   "│  \e[0;92mL'ennemis n'a pas pu attaquer ce tour !\e[0m                      │\n"
			   		   "│                                                               │\n"
			   		   "│                                                               │\n"
			   		   "│                                                               │\n"
			   		   "├───────────────────────────────────────────────────────────────┤\n"
			   		   "│ [0] Suite                                                     │\n"
			   		   "└───────────────────────────────────────────────────────────────┘\n");
				choix1 = 1;
				while (choix1 != 0){
					scanf("%d",&choix1);
				}
			}
			system("clear");
			if (atlanta.hull < 0){
				atlanta.hull = 0;
				printf("┌───────────────────────────────────────────────────────────────┐\n"
					   "│                           Game Over                           │\n"
					   "└───────────────────────────────────────────────────────────────┘\n");
				exit(0);
			}
			//Fin tour
			switch (etape) {
				case 1://solei
					atlanta.hull -= 5;
					ennemis[ennemyId].hull -= 5;
					displayAtlanta();
					displayEnnemy(&ennemis[ennemyId]);
					display(5,etape);
					printf("├───────────────────────────────────────────────────────────────┤\n"
			   			   "│                                                               │\n"
			   			   "│ \e[0;91m* Les rayons solaire font perdre 5 points à l'Atlanta\e[0m         │\n"
			   			   "│                                                               │\n"
			   			   "│ \e[0;92m* Les rayons solaire font perdre 5 points à l'ennemis\e[0m         │\n"
			   			   "│                                                               │\n"
			   			   "├───────────────────────────────────────────────────────────────┤\n"
			   			   "│ [0] Suite                                                     │\n"
			   			   "└───────────────────────────────────────────────────────────────┘\n");
					break;
				case 2://planete
					displayAtlanta();
					displayEnnemy(&ennemis[ennemyId]);
					display(5,etape);
					printf("├───────────────────────────────────────────────────────────────┤\n"
			   			   "│                                                               │\n"
			   			   "│                                                               │\n"
			   			   "│                                                               │\n"
			   			   "│                                                               │\n"
			   			   "│                                                               │\n"
			   			   "├───────────────────────────────────────────────────────────────┤\n"
			   			   "│ [0] Suite                                                     │\n"
			   			   "└───────────────────────────────────────────────────────────────┘\n");
					break;
				case 3://asteroid
					displayAtlanta();
					displayEnnemy(&ennemis[ennemyId]);
					display(5,etape);
					printf("├───────────────────────────────────────────────────────────────┤\n"
			   			   "│                                                               │\n");
					int r1 = random_nbr(1,2);
					if (r1 == 1) {
						atlanta.shield -= 20;
						if (atlanta.shield < 0){
							atlanta.hull += atlanta.shield;
							atlanta.shield = 0;
						}
						if (atlanta.hull < 0){
							atlanta.hull = 0;
						}
						printf("│ \e[0;91m* Vous percutez un astéroïde et prennez 20 points de dégats !\e[0m │\n");
					}else{
						printf("│                                                               │\n");
					}
					printf("│                                                               │\n");
					r1 = random_nbr(1,2);
					if (r1 == 1) {
						ennemis[ennemyId].shield -= 20;
						if (ennemis[ennemyId].shield < 0){
							ennemis[ennemyId].hull += ennemis[ennemyId].shield;
							ennemis[ennemyId].shield = 0;
						}
						if (ennemis[ennemyId].hull < 0){
							ennemis[ennemyId].hull = 0;
						}
						printf("│ \e[0;92m* L'ennemis percute un astéroïde, 20 points de dégats !\e[0m       │\n");
					}else{
						printf("│                                                               │\n");
					}
					printf("│                                                               │\n"
						   "├───────────────────────────────────────────────────────────────┤\n"
			   			   "│ [0] Suite                                                     │\n"
			   			   "└───────────────────────────────────────────────────────────────┘\n");
					break;
				case 4://vide
					displayAtlanta();
					displayEnnemy(&ennemis[ennemyId]);
					display(5,etape);
					atlanta.energyGain = peter.value/2;
					printf("├───────────────────────────────────────────────────────────────┤\n"
			   			   "│                                                               │\n"
			   			   "│  \e[0;94mLe Manque de lumière environnante ralentit la récupération\e[0m   │\n"
			   			   "│                                                               │\n"
			   			   "│  \e[0;94md'énergie !\e[0m                                                  │\n"
			   			   "│                                                               │\n"
			   			   "├───────────────────────────────────────────────────────────────┤\n"
			   			   "│ [0] Suite                                                     │\n"
			   			   "└───────────────────────────────────────────────────────────────┘\n");
					break;
			}
			atlanta.energy += (atlanta.energyGain + peter.value);
			if (atlanta.energy > atlanta.maxEnergy){
				atlanta.energy = atlanta.maxEnergy;
			}
			atlanta.shield += (atlanta.shieldGain + james.value);
			if (atlanta.shield > atlanta.maxShield){
				atlanta.shield = atlanta.maxShield;
			}
			ennemis[ennemyId].energy += ennemis[ennemyId].energyGain;
			if (ennemis[ennemyId].energy > ennemis[ennemyId].maxEnergy){
				ennemis[ennemyId].energy = ennemis[ennemyId].maxEnergy;
			}
			ennemis[ennemyId].shield += ennemis[ennemyId].shieldGain;
			if (ennemis[ennemyId].shield > ennemis[ennemyId].maxShield){
				ennemis[ennemyId].shield = ennemis[ennemyId].maxShield;
			}
			choix = 1;
			if (ennemis[ennemyId].hull < 0){
				ennemis[ennemyId].hull = 0;
				drops(ennemyId, etape);
				return;
			}
			if (atlanta.hull < 0){
				atlanta.hull = 0;
				system("clear");
				printf("┌───────────────────────────────────────────────────────────────┐\n"
					   "│                           Game Over                           │\n"
					   "└───────────────────────────────────────────────────────────────┘\n");
				exit(0);
			}
			while (choix != 0){
				scanf("%d",&choix);
			}
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
		displayAtlanta();
		printf("├───────────────────────────────────────────────────────────────┤\n"
			   "│ Selectionnez l'arme à remplacer :                             │\n");
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
						displayAtlanta();
						printf("├───────────────────────────────────────────────────────────────┤\n"
							   "│ Choix validé !                                                │\n");
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
						displayAtlanta();
						printf("├───────────────────────────────────────────────────────────────┤\n"
							   "│ Choix validé !                                                │\n");
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
						displayAtlanta();
						printf("├───────────────────────────────────────────────────────────────┤\n"
							   "│ Choix validé !                                                │\n");
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
		int level[][12] = {{1,2,3,4,5,6,7,8,3,3,4,4},
						   {5,6,7,8,9,10,11,12,0,0,0,0},
						   {9,10,11,12,13,14,15,16,0,0,0,0},
						   {13,14,15,16,17,18,19,20,0,0,0,0},
						   {17,18,19,20,21,22,23,24,0,0,0,0},
						   {20,21,22,23,24,25,26,27,0,0,0,0},
						   {23,24,25,26,27,28,29,30,0,0,0,0},
						   {23,24,25,26,27,28,29,30,0,0,0,0}};
		   //Items pool    // 8 Weapons            ║4 Systems
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
								if (atlanta.flouze >= 300){
									atlanta.sys[level[pool][randSys[0]]] = 1;
									atlanta.flouze -= 300;
									system("clear");
								}else{
									printf("Pas assez de Crédits !\n");
								}
								break;
							case 2:
								if (atlanta.flouze >= 300){
									atlanta.sys[level[pool][randSys[1]]] = 1;
									atlanta.flouze -= 300;
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
		int r = 0;
		switch (niveau){
			case 1 :
				switch (milieu){
					case 1 :
						switch (index){
							case 1:
								r = random_nbr(0,7);
								combat(r,milieu);
								printf("Vous avez gagné !\n");
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
						//couleur de la planète
						r = random_nbr(1,6);
						switch (r){
							case 1:
								strcpy(&clr[0], "1;91");
								break;
							case 2:
								strcpy(&clr[0], "0;31");
								break;
							case 3:
								strcpy(&clr[0], "1;92");
								break;
							case 4:
								strcpy(&clr[0], "0;36");
								break;
							case 5:
								strcpy(&clr[0], "0;96");
								break;
							case 6:
								strcpy(&clr[0], "1;97");
								break;
						}
						
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
			scenario(monde,world.type[currentEtape],alea);
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
		//generateMap(1);
		//cycle
		/*while (end == 0){
			//generate map
		}*/
		for (int i = 1; i<=8; i++){
			generateMap(i);
		}
	return 0;
	}