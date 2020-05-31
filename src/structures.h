//
//  srructures.h
//  
//
//  Created by loona gaillard on 04/05/2020.
//

#ifndef structures_h
#define structures_h

#include <stdio.h>

#include <SDL/SDL.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL/SDL_image.h>

/* Dimensions initiales et titre de la fenetre */
static const unsigned int WINDOW_WIDTH = 600;
static const unsigned int WINDOW_HEIGHT = 680;
static const char WINDOW_TITLE[] = "Projet OpenGL";

static float aspectRatio;

/* Espace fenetre virtuelle */
static const float GL_VIEW_SIZE = 150.;

/* Nombre de bits par pixel de la fenetre */
static const unsigned int BIT_PER_PIXEL = 32;

/* Nombre minimal de millisecondes separant le rendu de deux images */
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

/* --------------------
 Declaration Unite
 -------------------- */

typedef struct Unite {
    float pv;         // points de vie de l'unite
    float force;      // force de l'unite
    int portee;     // portee d'attaque (a combien de case elle peut attaquer)
    int mobilite;   // dexterite de l'unite (jusque combien de case elle peut se deplacer)
    int prix;
    int type;       // prix de l'unite
    GLuint image;    // image pour texture de l'unite
    float x, y;     // position de l'unite
} Unite;

/* --------------------
Declaration Joueur
-------------------- */

typedef struct Joueur{
    int id;
    Unite unite[10];
    int nbUnite;
    int cagnotte;
    GLuint image; // Association avec l'image du joueur
} Joueur;

typedef struct Curseur{
    GLuint image;
    float x, y;
} Curseur;


typedef struct Case{
    int type;
    int occupied;
    // Position de la case
    int x;
    int y;
}  Case;

#endif /* structures_h */
