#include "action.h"
#include "structures.h"
#include "collisions.h"
#include "main.h"

// Fonction qui permet de déplacer le joueur
// Rentre en action lors de la boucle évènement du while
// direction représente la touche actionnée dans main
// En fonction de la direction actionnée HAUT, BAS, DROITE, GAUCHE la position bouge différemment
// Si la position demandé fait sortir l'unité du cadre alors l'action est annulée (break;)

void deplacerjoueur(Joueur *j, int direction, int c, Case tabCase[20][20])
{
    switch(direction){
        case HAUT:
            if(j->unite[c].y + 1 > 20) // test pour savoir si il n'est plus dans la fenetre{
                break;
            else
            {
                tabCase[20 - (int)j->unite[c].y][(int)j->unite[c].x - 1].occupied = 0;
                j->unite[c].y = j->unite[c].y + 1.0;
                tabCase[20 - (int)j->unite[c].y][(int)j->unite[c].x - 1].occupied = 1;
            }
            break;
        case BAS:
            if(j->unite[c].y - 1 < 0){
                break;
            }
            else{
                tabCase[20 - (int)j->unite[c].y][(int)j->unite[c].x - 1].occupied  = 0;
                j->unite[c].y = j->unite[c].y - 1.0;
                tabCase[20 - (int)j->unite[c].y][(int)j->unite[c].x - 1].occupied  = 1;
            }
            break;
        case DROITE:
            if(j->unite[c].x + 1 > 20){
                break;
            }
            else{
                tabCase[20 - (int)j->unite[c].y][(int)j->unite[c].x - 1].occupied  = 0;
                j->unite[c].x = j->unite[c].x + 1.0;
                tabCase[20 - (int)j->unite[c].y][(int)j->unite[c].x - 1].occupied  = 1;
            }
            break;
        case GAUCHE:
            if(j->unite[c].x - 1 < 0){
                break;
            }
            else{
                tabCase[20 - (int)j->unite[c].y][(int)j->unite[c].x - 1].occupied  = 0;
                j->unite[c].x = j->unite[c].x - 1.0;
                tabCase[20 - (int)j->unite[c].y][(int)j->unite[c].x - 1].occupied  = 1;
            }
            break;
        }
}

//Fonction qui permet de déplacer le curseur
// En entre en jeu lors d'action d'évènement déclenché dans le main
// Même principe que pour deplacerjoueur()
void deplacercurseur(Curseur *c, int direction){
    switch(direction)
    {
    case HAUT:
        if(c->y + 1 > 20) // test pour savoir si il n'est plus dans la fenetre{
            break;
        else{
            c->y = c->y + 1.0;
        }
        break;
    case BAS:
        if(c->y - 1 < 0){
            break;
        }else{
            c->y = c->y - 1.0;
        }
        break;
    case DROITE:
        if(c->x + 1 > 20){
            break;
        }else{
            c->x = c->x + 1.0;
        }
        break;
    case GAUCHE:
        if(c->x - 1 < 0){
            break;
        }else{
            c->x = c->x - 1.0;
        }
        break;
    }
}


// Fonction qui permet d'attaquer une unité adverse, lui retire des pv en fonction de notre force et de nos pv
void attaque(Joueur *attaquant, Joueur *cible, int c1, int c2)
{
    cible->unite[c2].pv = cible->unite[c2].pv - attaquant->unite[c1].force * attaquant->unite[c1].pv;
    attaquant->unite[c1].pv = attaquant->unite[c1].pv - cible->unite[c2].force * cible->unite[c2].pv;
}


// Fonction qui vérifie si le joueur peut attaquer une unité autour de lui
// Si il peut attaquer une unité alors qui renvoie son numéro
int verifAttaque(Joueur *attaquant, Joueur *cible, int c, Case tabCase[20][20])
{
    for (int i = 0; i < cible->nbUnite; i++) {
        if ( (fabs(cible->unite[i].x - attaquant->unite[c].x) + fabs(cible->unite[i].y - attaquant->unite[c].y)) <= attaquant->unite[c].portee && testCaseAtt(tabCase, cible->unite[i].x, cible->unite[i].y, attaquant->unite[c].type) == 1){
            return i;
        }
    }
    return -1;
}

// Fonction qui permet de savoir si la case sélectionnée possède une unité du joueur j
// Si la réponse est oui alors la fonction retourne le numéro de l'unité sélectionée
int chercheUnite(Joueur *j, float x, float y)
{
    for (int i = 0; i < j->nbUnite; i++)
    {
        if ( x == j->unite[i].x && y == j->unite[i].y)
        {
            return i;
        }
    }
    return -1;
}

// Le int c permet de récupérer le numéro de l'unité
int mort(Joueur *j, int c){
    if(j->unite[c].pv <= 0){
        return 1;
    }else{
        return 0;
    }
}

int victoire(Joueur *j, int perteUnite){
    if(perteUnite == j->nbUnite){
        return 1;
    }else {
        return 0;
    }
}


// Joueur pour connaitre la position de l'unité choisie
// int c pour connnaitre le numéro de l'unité choisie et donc avoir accès à ses coordonnées
// Unite chance : déclaration d'une unité qui se déplace aléatoirement sur la map
void caseMagique(Joueur *j, Unite *chance, int c){
    
    if(j->unite[c].x == chance->x && j->unite[c].y == chance->y ){
        if(j->unite[c].pv < 20){
            j->unite[c].pv += 20;
        }
        if(j->unite[c].pv > 20 && j->unite[c].pv <= 35){
            j->unite[c].pv += 15;
        }
        if(j->unite[c].pv > 45 && j->unite[c].pv <= 60){
            j->unite[c].pv += 10;
        }
        if(j->unite[c].pv > 60 && j->unite[c].pv <= 95){
            j->unite[c].pv += 5;
        }
        if(j->unite[c].pv > 95){
            j->unite[c].pv += 0;
        }
        chance->x = (int) (my_rand() / (double)RAND_MAX * 19) ;
        chance->y = (int) (my_rand() / (double)RAND_MAX * 19) ;
    }
}
