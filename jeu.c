#include <stdio.h>
#include <string.h>

struct Scenario{
	int id;
	char nom[30];
}

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
}

struct Weapon{
	int id;
	char nom[30];
	int atk;
	int nbr_fire;
	int accuracy;
}

struct Etape{
	int nbr_choix;
	int milieu1;
	int milieu2;
	int milieu3;
}
//random


//display en couleurs s'il vous plait
display(int index, int sub)	{
	switch(index) {
		//scenario
		case 1 :
			switch (sub) {
				//intro
				case 1:
					printf();
					break;
			}
			break;
	}
}


//Generate map
	//for 8 etape
		//struct etape[i].nbr_choix = rand(2,3);
		//struct etape[i].milieu1 = rand(1,4) *3;
		
	//Niveau --> Debut d'un cycle

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
void navigation(int niveau, int etape){
	//display carte prégénérée au début du niveau, int etape pour definir le choix possible
	//choix du milieu (entre 2 et 3 choix possibles) peut être : près d'une etoile, planete, asteroide, vide spacial, chacun de ces millieu ayant ses opportunités et difficultés
	//pool de scenario possible en fonction de l'avancée et du choix -> scenario(niveau, choix, random);	
}
void niveau(){
	//for 8 etapes
		//navigation
		//check end
}

int main(){
	int end = 0;
	//cycle
	while (end == 0){
		//generate map
	}
return 0;
}