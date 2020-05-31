//
//  map.h
//  
//
//  Created by loona gaillard on 04/05/2020.
//

#ifndef map_h
#define map_h

#include <stdio.h>

#include <SDL/SDL.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL/SDL_image.h>

// GLuint Texture_Load permet d'afficher des textures
// image_path = image de la texture
// texWidth =  largeur de la texture
// texHeight = hauteur de la texture
GLuint Texture_Load(char* image_path,float texWidth, float texHeight);

void drawSquare(int filled);

// afficheMap permet de créer un objet à partir d'un texture
// param texture_id : la texture
// x = position en x de notre texture
// y = position en y de notre texture
// width = largeur de notre texture
// height = hauteur de notre texture
void afficheMap(GLuint texture_id, float x , float y , float width, float height);

GLuint Text_Load(SDL_Surface *texte);

void afficheTexte(GLuint texture, float x, float y, float w, float h);


#endif /* map_h */
