//
//  collisions.c
//  
//
//  Created by loona gaillard on 25/05/2020.
//

#include "collisions.h"
#include "structures.h"

void createTableau(SDL_Surface* img, Case tabCase[20][20])
{
    int positionY = 0;
        for(int i=16; i<img->h; i+=32)
        {
            int positionX = 0;
            for(int j=16; j<img->w; j+=32)
            {
                Uint8 r = ((Uint8*)(img->pixels))[((i*img->w + j)*4)];
                Uint8 g = ((Uint8*)(img->pixels))[((i*img->w + j)*4) + 1];
                Uint8 b = ((Uint8*)(img->pixels))[((i*img->w + j)*4) + 2];
    
                if ( r == 255 && g == 255 && b == 255 )
                {
                    tabCase[positionY][positionX].type = 0;
                }
                if ( r == 0 && g == 255 && b == 0 )
                {
                    tabCase[positionY][positionX].type = 1;
                }
                if ( r == 255 && g == 0 && b == 0 )
                {
                    tabCase[positionY][positionX].type = 2;
                }
                if ( r == 0 && g == 0 && b == 255 )
                {
                    tabCase[positionY][positionX].type = 3;
                }
    
                tabCase[positionY][positionX].occupied = 0;
                positionX++;
            }
            positionY++;
        }

}

// Fonction qui permet de savoir si l'unité peut accéder à la case en fonction de son type
// On vérifie son type ave le boucle if(type == )
// Puis on vérifie qu'il peut se poser avec le if(tabcase[…][…] || tabcase[…][…] == 0)
// Les condition dans ce type dépendent du type de l'unité :
//          le loup peut accéder au cases qui renvoient 1 (partie forêt) ou 0 (partie normale : herbe)
//          l'ours peut accéder aux cases qui renvoient 2 (partie grotte) ou 0 (partie normale : herbe)
//          le hibou peut accéder aux cases qui renvoient 3 (partie eau) ou 0 (partie normale : herbe)
//          le chasseur (bot) peut accéder aux cases qui ne renvoient 3 (partie eau)
int testCase (Case tabCase[20][20], float caseX, float caseY, int type){
   if (tabCase[20 - (int)caseY][(int)caseX - 1].occupied == 0) {
            if (type == 1){
                if (tabCase[20 - (int)caseY][(int)caseX - 1].type == 1 || tabCase[20 - (int)caseY][(int)caseX - 1].type == 0){
                    return 1;
                }
            }
    
            if (type == 2){
                if (tabCase[20 - (int)caseY][(int)caseX - 1].type == 2 || tabCase[20 - (int)caseY][(int)caseX - 1].type == 0){
                    return 1;
                }
            }
    
            if (type == 3){
                if (tabCase[20 - (int)caseY][(int)caseX - 1].type == 0 || tabCase[20 - (int)caseY][(int)caseX - 1].type == 3) {
                    return 1;
                }
    
            }
    
            if (type == 4){
                if (tabCase[20 - (int)caseY][(int)caseX - 1].type != 3){
                    return 1;
                }
            }
        }
    
        return 0;
}

//Même principe que pour la focntion testCase mais cette fois-ci les conditions sont appliquées à l'attaque des unités
//          le loup (type == 1) peut attaquer au cases qui renvoient 1 (partie forêt) ou 0 (partie normale : herbe)
//          l'ours (type == 2) peut accéder aux cases qui renvoient 2 (partie grotte) ou 0 (partie normale : herbe)
//          le hibou (type==3) peut accéder aux cases qui renvoient 3 (partie eau) ou 0 (partie normale : herbe)
//          le chasseur (type==4) (bot) peut accéder aux cases qui ne renvoient 3 (partie eau)
int testCaseAtt (Case tabCase[20][20], float caseX, float caseY, int type){
   if (type == 1){
            if (tabCase[20 - (int)caseY][(int)caseX - 1].type == 1 || tabCase[20 - (int)caseY][(int)caseX - 1].type == 0){
                return 1;
            }
        }
    
        if (type == 2) {
            if (tabCase[20 - (int)caseY][(int)caseX - 1].type == 2 || tabCase[20 - (int)caseY][(int)caseX - 1].type == 0){
                return 1;
            }
        }
    
        if (type == 3){
            if (tabCase[20 - (int)caseY][(int)caseX - 1].type == 0 || tabCase[20 - (int)caseY][(int)caseX - 1].type == 3){
                return 1;
            }
    
        }
        if (type == 4){
            if (tabCase[20 - (int)caseY][(int)caseX - 1].type != 3){
                return 1;
            }
        }
    
        return 0;

}

//Fonction pour savoir si la chouette peut se poser ou non
int testSePoser(Case tabCase[20][20], float caseX, float caseY){
    if (tabCase[20 - (int)caseY][(int)caseX - 1].type == 3)
    {
        return 0;
    } else
    {
        return 1;
    }
}
