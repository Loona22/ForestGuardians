#include "map.h"
#include "structures.h"

// Initialisation de la texture (� partir de l'image)
GLuint Texture_Load(char* image_path,float texWidth, float texHeight)
{
    GLuint texture_id;

    glGenTextures(1, &texture_id);

    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    SDL_Surface* image = IMG_Load(image_path);

    if(!image)
    {
        fprintf(stderr, "IMG_Load: %s\n", IMG_GetError());
        return 0;
    }

    GLenum format;

    switch(image->format->BytesPerPixel)
    {
    case 1:
        format = GL_RED;
        break;
    case 3:
        format = GL_RGB;
        break;
    case 4:
        format = GL_RGBA;
        break;
    default:
        fprintf(stderr, "Format des pixels de l'image %s non supporte.\n", image_path);
        return EXIT_FAILURE;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, format, GL_UNSIGNED_BYTE, image->pixels);

    glBindTexture(GL_TEXTURE_2D, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    /* Liberation de la m�moire occupee par img */
    SDL_FreeSurface(image);

    return texture_id ;
}

GLuint Text_Load(SDL_Surface *texte)
{
    GLuint texture;

    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glDisable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLenum format;

    switch(texte->format->BytesPerPixel)
    {
    case 1:
        format = GL_RED;
        break;
    case 3:
        format = GL_RGB;
        break;
    case 4:
        format = GL_RGBA;
        break;
    default:
        //fprintf(stderr, "Format des pixels de l'image %s non supporte.\n", image_path);
        return EXIT_FAILURE;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texte->w, texte->h, 0, format, GL_UNSIGNED_BYTE, texte->pixels);

    glBindTexture(GL_TEXTURE_2D, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    /* Liberation de la m�moire occupee par img */
    SDL_FreeSurface(texte);

    return texture;

}

// on dessine les �l�ments sur la map � partir d'une texture
void afficheMap(GLuint texture_id, float x, float y, float width, float height)
{
    if (texture_id)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glPushMatrix();
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
        glVertex2f(x-width/2,y+height/2);
        glTexCoord2f(0, 1);
        glVertex2f(x-width/2,y-height/2);
        glTexCoord2f(1, 1);
        glVertex2f(x+width/2,y-height/2);
        glTexCoord2f(1, 0);
        glVertex2f(x+width/2,y+height/2);

        glEnd();
        glPopMatrix();
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);

    }
    else
    {
        fprintf(stderr, "Erreur de texture\n");
    }
}

void afficheTexte(GLuint texture, float x, float y, float w, float h)
{
    float hauteur = h/WINDOW_HEIGHT;
    float largeur = w/WINDOW_WIDTH;
    if (texture)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glPushMatrix();
        glBegin(GL_QUADS);
        {

            glTexCoord2f(0,1);
            glVertex2f(x, y);
            glTexCoord2f(1,1);
            glVertex2f(x + largeur, y);
            glTexCoord2f(1,0);
            glVertex2f(x + largeur, y + hauteur);
            glTexCoord2f(0,0);
            glVertex2f(x, y + hauteur);


        }
        glEnd();
        glPopMatrix();
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);

    }
    else
    {
        fprintf(stderr, "Erreur de texture\n");
    }
}
