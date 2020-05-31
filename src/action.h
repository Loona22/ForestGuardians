//
//  action.h
//  
//
//  Created by loona gaillard on 06/05/2020.
//

#ifndef action_h
#define action_h

#include "structures.h"

// Permet de connaitre lors de l'appui des touches clavier flèches
enum {HAUT, BAS, GAUCHE, DROITE};

void deplacerjoueur(Joueur *j, int direction, int c, Case tabCase[20][20]);

void attaque(Joueur *attaquant, Joueur *cible, int c1, int c2);

int verifAttaque(Joueur *attaquant, Joueur *cible, int c, Case tabCase[20][20]);

int chercheUnite(Joueur *j, float x, float y);

int mort(Joueur *j, int c);

int victoire(Joueur *j, int perteUnite);

void caseMagique(Joueur *j, Unite *chance, int c);

#endif /* action_h */
