//
//  collisions.h
//  
//
//  Created by loona gaillard on 25/05/2020.
//

#ifndef collisions_h
#define collisions_h

#include <stdio.h>

#include "structures.h"

void createTableau(SDL_Surface* img, Case tabCase[20][20]);

int testCase (Case tabCase[20][20], float caseX, float caseY, int type);

int testCaseAtt (Case tabCase[20][20], float caseX, float caseY, int type);

int testSePoser(Case tabCase[20][20], float caseX, float caseY);

#endif /* collisions_h */
