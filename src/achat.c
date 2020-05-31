//
//  achat.c
//  
//
//  Created by loona gaillard on 15/05/2020.
//

#include "achat.h"
#include "structures.h"
#include "map.h"
#include "collisions.h"
#include "main.h"

// Unite1 correspond au loup
// Unite de base
void Unite1(Joueur *j, int c, int tour){

    j->unite[c].x = (int) (my_rand() / (double)RAND_MAX * 19) ;
    j->unite[c].y = (int) (my_rand() / (double)RAND_MAX * 19) ;

    j->unite[c].pv = 100;
    j->unite[c].prix = 20;
    j->unite[c].force = 0.5;
    j->unite[c].mobilite = 7;
    j->unite[c].portee = 7;
    j->unite[c].type = 1;
    if (tour%2 != 0){
         j->unite[c].image = Texture_Load("./img/loupJ1.png", 29, 33);
    } else {
        j->unite[c].image = Texture_Load("./img/loupJ2.png", 29, 33);
    }

}

// Unite2 correspond à l'ours
// Unité forte mais faible en déplacement et portée
void Unite2(Joueur *j, int c, int tour){

    j->unite[c].x = (int) (my_rand() / (double)RAND_MAX * 19) ;
    j->unite[c].y = (int) (my_rand() / (double)RAND_MAX * 19) ;

    j->unite[c].pv = 100;
    j->unite[c].prix = 40;
    j->unite[c].force = 0.7;
    j->unite[c].mobilite = 4;
    j->unite[c].portee = 4;
    j->unite[c].type = 2;
    if (tour%2 != 0){
         j->unite[c].image = Texture_Load("./img/oursJ1.png", 29, 33);
    } else {
         j->unite[c].image = Texture_Load("./img/oursJ2.png", 29, 33);
    }
}

// Unite3 correspond au hibou
// Unité de faible attaque mais se déplace et attaque de loin
void Unite3(Joueur *j, int c, int tour){

    j->unite[c].x = (int) (my_rand() / (double)RAND_MAX * 19) ;
    j->unite[c].y = (int) (my_rand() / (double)RAND_MAX * 19) ;

    j->unite[c].pv = 100;
    j->unite[c].prix = 30;
    j->unite[c].force = 0.3;
    j->unite[c].mobilite = 10;
    j->unite[c].portee = 10;
    j->unite[c].type = 3;
    if (tour%2 != 0){
        j->unite[c].image = Texture_Load("./img/hibouJ1.png", 29, 33);
    } else {
        j->unite[c].image = Texture_Load("./img/hibouJ2.png", 29, 33);
    }
}

void achatUnite(Joueur *j, int type, int c, int tour, Case tabCase[20][20]){

    if(type == 1){
        do {
            Unite1(j, c, tour);
        } while (testCase(tabCase, j->unite[c].x, j->unite[c].y, j->unite[c].type) != 1);

        j->cagnotte = j->cagnotte - j->unite[c].prix;
    }
    if (type == 2){
        do {
            Unite2(j, c, tour);
        } while (testCase(tabCase, j->unite[c].x, j->unite[c].y, j->unite[c].type) != 1);

        j->cagnotte = j->cagnotte - j->unite[c].prix;
    }
    if (type == 3){
        do {
            Unite3(j, c, tour);
        } while (testCase(tabCase, j->unite[c].x, j->unite[c].y, j->unite[c].type) != 1 && testSePoser(tabCase, j->unite[c].x, j->unite[c].y) == 0);

        j->cagnotte = j->cagnotte - j->unite[c].prix;
    }

    tabCase[20 - (int)j->unite[c].y][(int)j->unite[c].x - 1].occupied = 1;

}
