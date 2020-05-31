//
//  achat.h
//  
//
//  Created by loona gaillard on 15/05/2020.
//

#ifndef achat_h
#define achat_h

#include <stdio.h>

#include "structures.h"
#include "collisions.h"

void Unite1(Joueur *j, int c, int tour);
void Unite2(Joueur *j, int c, int tour);
void Unite3(Joueur *j, int c, int tour);

void achatUnite(Joueur *j, int type, int c, int tour, Case tabCase[20][20]);

#endif /* achat_h */
