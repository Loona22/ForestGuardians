//
//  ia.c
//  
//
//  Created by loona gaillard on 28/05/2020.
//

#include "ia.h"
#include "structures.h"
#include "collisions.h"
#include "action.h"
#include "main.h"

//Fonction qui permet de donner des instructions au joueur ordinateur
// Si une unité adverse est dans son champs de tire alors l'IA attaque sinon elle se déplace
// Pour effectuer un déplacement, direction est généré aléatoirement entre 0 et 3 car il y a 4 directions posssibles
// En fonction de direction on modifie alors la position de l'IA
//      direction == 0 : déplacement vers le haut
//      direction == 1 : déplacement vers le bas
//      direction == 2 : déplacement vers la droite
//      direction == 2 : déplacement vers la gauche
// Pour effectuer un déplacement correspondant à sa mobilité la variable tour est implémenté à chaque déplacement
// Si jamais le déplacement n'a pas pu s'effectuer, alors tour est déplémenté pour avoir le même nombre de déplacement au final

void IA(Joueur *attaquant, Joueur *cible, Case tabCase[20][20]){
    if (verifAttaque(attaquant, cible, 0, tabCase) >= 0){
        int UniteCible = verifAttaque(attaquant, cible, 0, tabCase);
        attaque(attaquant, cible, 0, UniteCible);
    } else{
        int tour = 0;
        while(tour < attaquant->unite[0].mobilite){
            
            int direction = (int) (my_rand() / (double)RAND_MAX * 4) ;
            //direction == 0 --> direction vers le haut
            if( direction == 0){
                if(attaquant->unite[0].y + 1 > 20 && (testCase(tabCase, attaquant->unite[0].x, attaquant->unite[0].y+1, attaquant->unite[0].type)==4)==1){
                    tour --;
                }else{
                    tabCase[20 - (int)attaquant->unite[0].y][(int)attaquant->unite[0].x - 1].occupied = 0;
                    attaquant->unite[0].y = attaquant->unite[0].y + 1.0;
                    tabCase[20 - (int)attaquant->unite[0].y][(int)attaquant->unite[0].x - 1].occupied = 1;
                    tour ++;
                }
            }
            //direction == 1 --> direction vers le bas
            if( direction == 1){
                if(attaquant->unite[0].y  - 1 < 1 && (testCase(tabCase, attaquant->unite[0].x, attaquant->unite[0].y+1, attaquant->unite[0].type)==4)==1){
                    tour --;
                }else{
                    tabCase[20 - (int)attaquant->unite[0].y][(int)attaquant->unite[0].x - 1].occupied = 0;
                    attaquant->unite[0].y = attaquant->unite[0].y - 1.0;
                    tabCase[20 - (int)attaquant->unite[0].y][(int)attaquant->unite[0].x - 1].occupied = 1;
                    tour ++;
                }
            }
            //direction == 2 --> direction vers la droite
            if( direction == 2){
                if(attaquant->unite[0].x  + 1 > 20 && (testCase(tabCase, attaquant->unite[0].x, attaquant->unite[0].y+1, attaquant->unite[0].type)==4)==1){
                    tour --;
                }else{
                    tabCase[20 - (int)attaquant->unite[0].y][(int)attaquant->unite[0].x - 1].occupied = 0;
                    attaquant->unite[0].x = attaquant->unite[0].x + 1.0;
                    tabCase[20 - (int)attaquant->unite[0].y][(int)attaquant->unite[0].x - 1].occupied = 1;
                    tour ++;
                }
            }
            //direction == 3 --> direction vers la gauche
            if( direction == 3){
                if(attaquant->unite[0].x  - 1 < 1 && (testCase(tabCase, attaquant->unite[0].x, attaquant->unite[0].y+1, attaquant->unite[0].type)==4)==1){
                   tour --;
                }else{
                    tabCase[20 - (int)attaquant->unite[0].y][(int)attaquant->unite[0].x - 1].occupied = 0;
                    attaquant->unite[0].x = attaquant->unite[0].x - 1.0;
                    tabCase[20 - (int)attaquant->unite[0].y][(int)attaquant->unite[0].x - 1].occupied = 1;
                    tour ++;
                }
            }
        }
    }
}
