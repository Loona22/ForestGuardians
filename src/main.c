//
//  main.c
//
//
//  Created by loona gaillard on 04/05/2020.
//

// inclusion fichiers header
#include "main.h"
#include "structures.h"
#include "map.h"
#include "action.h"
#include "achat.h"
#include "collisions.h"
#include "ia.h"

// inclusion biblitohèques
#include <SDL/SDL.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

//Fonction permettant de générer des nombres aléatoires
int my_rand (void)
{
   static int first = 0;

   if (first == 0)
   {
      srand (time (NULL));
      first = 1;
   }
   return (rand ());
}

void reshape(SDL_Surface** surface, unsigned int width, unsigned int height)
{
    SDL_Surface* surface_temp = SDL_SetVideoMode(
        width, height, BIT_PER_PIXEL,
        SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_RESIZABLE);
    if(NULL == surface_temp)
    {
        fprintf(
            stderr,
            "Erreur lors du redimensionnement de la fenetre.\n");
        exit(EXIT_FAILURE);
    }
    *surface = surface_temp;

    aspectRatio = width / (float) height;

    glViewport(0, 0, (*surface)->w, (*surface)->h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(
        0,1,
        0,1);
}

int main(int argc, char** argv)
{
    /* Initialisation de la SDL */
        if(-1 == SDL_Init(SDL_INIT_VIDEO))
        {
            fprintf(
                stderr,
                "Impossible d'initialiser la SDL. Fin du programme.\n");
            exit(EXIT_FAILURE);
        }
    
        /* Initialisation de la SDl_ttf */
        if(TTF_Init() == -1)
        {
            fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
            exit(EXIT_FAILURE);
        }
    
        /* Ouverture d'une fenetre et creation d'un contexte OpenGL */
        SDL_Surface* surface;
        reshape(&surface, WINDOW_WIDTH, WINDOW_HEIGHT);
    
        Case tabCase[20][20];
        SDL_Surface* carte;
        carte = IMG_Load("./img/collisions2.png");
        createTableau(carte, tabCase);
    
        /* Initialisation du titre de la fenetre */
        SDL_WM_SetCaption(WINDOW_TITLE, NULL);
    
        /* initialisation des paramètres pour la TTF */
        TTF_Font *police15 = TTF_OpenFont("./fonts/Minecraft.ttf", 15);
        TTF_Font *police20 = TTF_OpenFont("./fonts/Minecraft.ttf", 20);
        SDL_Color blanc = {255, 255, 255};
        SDL_Color noir = {0, 0, 0};
        SDL_Surface *texte = NULL;
    
        /* --------------
         INITIALISATION JOUEURS
         -------------- */
    
        //Création d'un joueur temporaire qui permet de savoir qui doit jouer
        Joueur* currentPlayer;
        Joueur* otherPlayer;
    
        Joueur j1;
        j1.id = 1;
        j1.cagnotte = 200;
        j1.nbUnite = 1;
        
        // Déclaration de toutes les caractéristiques de la première unité de J1
        j1.unite[0].pv = 100.0;
        j1.unite[0].mobilite = 5;
        j1.unite[0].portee = 4;
        j1.unite[0].force = 0.3;
        j1.unite[0].type = 1;
        j1.unite[0].x = (int) (my_rand() / (double)RAND_MAX * 19);
        j1.unite[0].y = (int) (my_rand() / (double)RAND_MAX * 19);
        j1.unite[0].image = Texture_Load("./img/loupJ1.png", 29, 33);
    
        tabCase[20 - (int)j1.unite[0].y][(int)j1.unite[0].x - 1].occupied = 1;
    
        Joueur j2;
        j2.id = 2;
        j2.cagnotte = 200;
        j2.nbUnite = 1;

        j2.unite[0].pv = 100.0;
        j2.unite[0].mobilite = 5;
        j2.unite[0].portee = 4;
        j2.unite[0].force = 0.3;
        j2.unite[0].type = 1;
        j2.unite[0].x = (int) (my_rand() / (double)RAND_MAX * 19);
        j2.unite[0].y = (int) (my_rand() / (double)RAND_MAX * 19);
        j2.unite[0].image = Texture_Load("./img/loupJ2.png", 29, 33);
    
        Joueur ordinateur;
    
        //initialisation curseur attaque
    
        Curseur curseurAtt;
        curseurAtt.image = Texture_Load("./img/curseurattaque.png", 32, 32);
    
        //initialisation curseur choix unité
    
        Curseur curseurChoix;
        curseurChoix.image = Texture_Load("./img/curseur.png", 32, 32);
        curseurChoix.x = 10.0;
        curseurChoix.y = 10.0;
    
        //Initialisation case magique
        Unite chance;
        chance.x = 16;
        chance.y = 4;
        chance.image = Texture_Load("./img/fruits.png", 32, 32);
    
        /* --------------
         INITIALISATION des différents "modes"
         -------------- */
        int chgtTexture = 0;
        int playing = 0;
    
        int selection = 0;
        int action = 0;
        int deplacer = 0;
        int attaquer = 0;
    
        int erreur = 0;
        int HibouEau = 0;
    
        int intro1 = 0;
        int intro2 = 0;
        int achat = 0;
        int aide = 0 ;
    
        int currentUnite = 0;
        int attaqueUnite = 0;
    
        int ordi = 0;
    
        int quitter = 0;
    
    
        // compteur qui permet ensuite de savoir si le joueur a perdu toutes ses unités ou non
        int perteUniteJ1 = 0;
        int perteUniteJ2 = 0;
        int perteUniteOrdi = 0;
    
        //initialisation des compteurs de pas
        //utilisés dans "attaquer" (pour vérifier que le curseur d'attaque doit bien dans le périmètre de portée) ET dans "se déplacer" (pour vérifier que le joueur se déplace dans le périmètre de mobilité)
        int pasX = 0;
        int pasY = 0;
    
        //c'est juste des valeurs qui servent pour le clic de la souris (c'est plus pratique que d'utiliser les e.ubtton.x là)
        int positionX = 0;
        int positionY = 0;
    
        //Chargement texture de la map
        GLuint texture_map = Texture_Load("./img/map.png", 960, 1088);
    
        //chargement texture infos pv
        GLuint texture_infos = Texture_Load("./img/infospv.png", 960, 1088);
    
        //Chargement texture à qui le tour
        GLuint texture_tourJ1 = Texture_Load("./img/losangeJ1.png", 60, 67);
        GLuint texture_tourJ2 = Texture_Load("./img/losangeJ2.png", 60, 67);
    
        //Chargement texture erreur
        GLuint texture_erreur = Texture_Load("./img/erreurunite.png", 960, 1088);
        GLuint texture_erreurhibou = Texture_Load("./img/erreurhibou.png", 960, 1088);
    
        //Chargement texture du début du jeu
        GLuint texture_menu = Texture_Load("./img/newmenu.png", 960, 1088);
        GLuint texture_forest = Texture_Load("./img/forest.png", 411, 95);
        GLuint texture_guardians = Texture_Load("./img/guardians.png", 633, 98);
        GLuint texture_play = Texture_Load("./img/play.png", 354, 114);
        GLuint texture_quit = Texture_Load("./img/quit.png", 335, 117);
    
        //Chargement texture choix entre partie 2 joueurs ou VS ordi
        GLuint texture_2menu = Texture_Load("./img/2menu.png", 960, 1088);
        GLuint texture_2players = Texture_Load("./img/players.png", 741, 114);
        GLuint texture_computer = Texture_Load("./img/computer.png", 987, 117);
        GLuint texture_selectionIA = Texture_Load("./img/selectionordi.png", 960, 1088);
        GLuint texture_actionIA = Texture_Load("./img/choixordi.png", 960, 1088);
    
        //Chargement texture du mode de choix
        GLuint texture_choix = Texture_Load("./img/choix.png", 960, 1088);
        GLuint texture_deplacer = Texture_Load("./img/deplacer.png", 960, 1088);
        GLuint texture_attaquer = Texture_Load("./img/attaquer.png", 960, 1088);
        GLuint texture_menu_burger = Texture_Load("./img/menu_burger.png", 960, 1088);
    
        //Chargement texture du mode intro1 *message de bonjour joueur1*
        GLuint texture_intro1 = Texture_Load("./img/fond-intro1.png", 960, 1088);
    
        //Chargement texture du mode intro2 *message de bonjour joueur2*
        GLuint texture_intro2 = Texture_Load("./img/fond-intro2.png", 960, 1088);
    
        //Chargement texture du menu achat
        GLuint texture_menu_achat = Texture_Load("./img/menu-achat.png", 960, 1088);
    
        //Chargement texture de fin de partie
        GLuint texture_victoireJ1 = Texture_Load("./img/victoireJ1.png", 960, 1088);
        GLuint texture_victoireJ2 = Texture_Load("./img/victoireJ2.png", 960, 1088);
    
        GLuint texture_victoireIA = Texture_Load("./img/victoireordi.png", 960, 1088);
        GLuint texture_defaiteIA = Texture_Load("./img/defaiteordi.png", 960, 1088);
    
        //Chargement texture menu aide
        GLuint texture_boutonhelp = Texture_Load("./img/boutonhelp.png", 960, 1088);
        GLuint texture_help = Texture_Load("./img/help.png", 960, 1088);
    
        //Chargement texture quitter le jeu
        GLuint texture_quitter = Texture_Load("./img/quitter.png", 960, 1088);
    
    
        /* Boucle principale */
        int loop = 1;
        while(loop){
            /* Recuperation du temps au debut de la boucle */
            Uint32 startTime = SDL_GetTicks();
            /* Code de dessin */
            glClear(GL_COLOR_BUFFER_BIT);
            
            if (ordi == 1){
                ordinateur.nbUnite = 1;
                ordinateur.unite[0].pv = 300.0;
                ordinateur.unite[0].mobilite = 4;
                ordinateur.unite[0].portee = 8;
                ordinateur.unite[0].force = 0.2;
                ordinateur.unite[0].type = 4;
                ordinateur.unite[0].x = (int) (my_rand() / (double)RAND_MAX * 19);
                ordinateur.unite[0].y = (int) (my_rand() / (double)RAND_MAX * 19);
                ordinateur.unite[0].image = Texture_Load("./img/chasseur.png", 23, 31);
    
                tabCase[20 - (int)ordinateur.unite[0].y][(int)ordinateur.unite[0].x - 1].occupied = 1;
    
                ordi++;
    
            }
    
            /* si on est dans le menu */
            if (chgtTexture == 0){
                afficheMap(texture_menu, 0.5, 0.5, 1, 1);
                afficheMap(texture_forest, 0.5, 0.75, 0.33, 0.1);
                afficheMap(texture_guardians, 0.5, 0.64, 0.5, 0.15);
                afficheMap(texture_play, 0.5, 0.5, 0.2, 0.06);
                afficheMap(texture_quit, 0.5, 0.38, 0.2, 0.06);
            }
    
            if (chgtTexture == 1){
                afficheMap(texture_2menu, 0.5, 0.5, 1, 1);
                afficheMap(texture_2players, 0.5, 0.647, 0.32, 0.055);
                afficheMap(texture_computer, 0.5, 0.47, 0.36, 0.053);
            }

            /* si on est dans le jeu */
                    if (chgtTexture == 2){
                        if (ordi == 0){
                            if (victoire(&j1, perteUniteJ1) == 0 && victoire(&j2, perteUniteJ2) == 0){
                                //Affichage de la map
                                afficheMap(texture_map, 0.5, 0.5, 1, 1);
                                afficheMap(texture_infos, 0.5, 0.5, 1, 1);
                                afficheMap(texture_boutonhelp, 0.5, 0.5, 1, 1);
                                afficheMap(chance.image, (chance.x + 5)/30.0, (chance.y + 9)/34.0, 0.05, 0.045);
            
                                perteUniteJ1 = 0;
                                perteUniteJ2 = 0;
            
                                //affichage cagnotte joueur 1
                                char buf1[256];
                                char* argent1 = gcvt(j1.cagnotte, 3, buf1);
            
                                texte = TTF_RenderText_Blended(police15, argent1, blanc);
                                int w = texte->w;
                                int h = texte->h;
            
                                //on charge le texte dans la ttf et on l'affiche
                                GLuint text_texte = Text_Load(texte);
                                afficheTexte(text_texte, 0.39, 0.18, w, h);
            
                                for (int i = 0; i < j1.nbUnite; i++){
                                    if(mort(&j1, i) == 0){
                                        afficheMap(j1.unite[i].image, (j1.unite[i].x + 5)/30.0, (j1.unite[i].y + 9)/34.0, 0.05, 0.045);
                                    }else{
                                        perteUniteJ1 ++ ;
                                    }
            
                                    if(j1.unite[i].pv <= 0){
                                        j1.unite[i].pv = 0;
                                    }
            
                                    /* affichage des informations relatives aux PVs des unités du j1*/
            
                                    //récupération du nb de pv restant, utilisation de gcvt car on passe d'un float à un char*
                                    char buf[256];
                                    char* pv = gcvt(j1.unite[i].pv, 3, buf);
            
                                    //récupération du numéro de l'unité
                                    char buf2[10];
                                    char* uniteI = gcvt(i+1, 1, buf2);
            
                                    char infos[50];
                                    strcpy(infos, "Unite no ");
                                    strcat(infos, uniteI);
                                    strcat(infos, " : PV = ");
                                    strcat(infos, pv);
                                    strcat(infos, "/100");
            
                                    texte = TTF_RenderText_Blended(police15, infos, blanc);
                                    int w = texte->w;
                                    int h = texte->h;
            
                                    //on charge le texte dans la ttf et on l'affiche
                                    GLuint text_texte = Text_Load(texte);
                                    afficheTexte(text_texte, 0.12, 0.14 - i*0.025, w, h);
                                }
            
            
                                for (int i = 0; i < j2.nbUnite; i++) {
                                    if(mort(&j2, i) == 0) {
                                        afficheMap(j2.unite[i].image, (j2.unite[i].x + 5)/30.0, (j2.unite[i].y + 9)/34.0, 0.05, 0.045);
                                    } else {
                                        perteUniteJ2 ++;
                                    }
                                    
                                    if(j2.unite[i].pv <= 0){
                                        j2.unite[i].pv = 0;
                                    }
            
                                    /* affichage des informations relatives aux PVs des unités du j1*/
            
                                    //récupération du nb de pv restant, utilisation de gcvt car on passe d'un float à un char*
                                    char buf[256];
                                    char* pv = gcvt(j2.unite[i].pv, 3, buf);
            
                                    //récupération du numéro de l'unité
                                    char buf2[10];
                                    char* uniteI = gcvt(i+1, 1, buf2);
            
                                    char infos[50];
                                    strcpy(infos, "Unite no ");
                                    strcat(infos, uniteI);
                                    strcat(infos, " : PV = ");
                                    strcat(infos, pv);
                                    strcat(infos, "/100");
            
                                    texte = TTF_RenderText_Blended(police15, infos, blanc);
                                    int w = texte->w;
                                    int h = texte->h;
            
                                    //on charge le texte dans la ttf et on l'affiche
                                    GLuint text_texte = Text_Load(texte);
                                    afficheTexte(text_texte, 0.59, 0.14 - i*0.025, w, h);
                                }
                                
                                //affichage cagnotte joueur 2
                                char buf2[256];
                                char* argent2 = gcvt(j2.cagnotte, 3, buf2);
            
                                texte = TTF_RenderText_Blended(police15, argent2, blanc);
                                int w1 = texte->w;
                                int h1 = texte->h;
            
                                //on charge le texte dans la ttf et on l'affiche
                                GLuint text_texte2 = Text_Load(texte);
                                afficheTexte(text_texte2, 0.575, 0.18, w1, h1);
                            } else {
                                selection = 0;
                                action = 0;
                                deplacer = 0;
                                attaquer = 0;
                                playing = 0;
            
                                if(victoire(&j1, perteUniteJ1) == 1) {
                                    afficheMap(texture_victoireJ2, 0.5, 0.5, 1, 1);
                                }
                                if(victoire(&j2, perteUniteJ2) == 1){
                                    afficheMap(texture_victoireJ1, 0.5, 0.5, 1, 1);
                                }
                            }
                        } else {
                            if (victoire(&j1, perteUniteJ1) == 0 && victoire(&ordinateur, perteUniteOrdi) == 0){
                                //Affichage de la map
                                afficheMap(texture_map, 0.5, 0.5, 1, 1);
                                afficheMap(texture_infos, 0.5, 0.5, 1, 1);
                                afficheMap(texture_boutonhelp, 0.5, 0.5, 1, 1);
                                afficheMap(chance.image, (chance.x + 5)/30.0, (chance.y + 9)/34.0, 0.05, 0.045);
            
                                perteUniteJ1 = 0;
                                perteUniteOrdi = 0;
            
                                //affichage cagnotte joueur 1
                                char buf1[256];
                                char* argent1 = gcvt(j1.cagnotte, 3, buf1);
            
                                texte = TTF_RenderText_Blended(police15, argent1, blanc);
                                int w = texte->w;
                                int h = texte->h;
            
                                //on charge le texte dans la ttf et on l'affiche
                                GLuint text_texte = Text_Load(texte);
                                afficheTexte(text_texte, 0.39, 0.18, w, h);
            
                                for (int i = 0; i < j1.nbUnite; i++) {
                                    if(mort(&j1, i) == 0){
                                        afficheMap(j1.unite[i].image, (j1.unite[i].x + 5)/30.0, (j1.unite[i].y + 9)/34.0, 0.05, 0.045);
                                    }else{
                                        perteUniteJ1 ++ ;
                                    }
            
                                    if(j1.unite[i].pv <= 0){
                                        j1.unite[i].pv = 0;
                                    }
            
                                    /* affichage des informations relatives aux PVs des unités du j1*/
            
                                    //récupération du nb de pv restant, utilisation de gcvt car on passe d'un float à un char*
                                    char buf[256];
                                    char* pv = gcvt(j1.unite[i].pv, 3, buf);
            
                                    //récupération du numéro de l'unité
                                    char buf2[10];
                                    char* uniteI = gcvt(i+1, 1, buf2);
            
                                    char infos[50];
                                    strcpy(infos, "Unite no ");
                                    strcat(infos, uniteI);
                                    strcat(infos, " : PV = ");
                                    strcat(infos, pv);
                                    strcat(infos, "/100");
            
                                    texte = TTF_RenderText_Blended(police15, infos, blanc);
                                    int w = texte->w;
                                    int h = texte->h;
            
                                    //on charge le texte dans la ttf et on l'affiche
                                    GLuint text_texte = Text_Load(texte);
                                    afficheTexte(text_texte, 0.12, 0.14 - i*0.025, w, h);
                                }

                                afficheMap(ordinateur.unite[0].image, (ordinateur.unite[0].x + 5)/30.0, (ordinateur.unite[0].y + 9)/34.0, 0.05, 0.045);
            
                                if(mort(&ordinateur, 0) == 0){
                                    afficheMap(ordinateur.unite[0].image, (ordinateur.unite[0].x + 5)/30.0, (ordinateur.unite[0].y + 9)/34.0, 0.05, 0.045);
                                } else {
                                    perteUniteOrdi ++ ;
                                }
            
                                if(ordinateur.unite[0].pv <= 0){
                                    ordinateur.unite[0].pv = 0;
                                }
            
                                /* affichage des informations relatives aux PVs des unités de l'ordinateur*/
            
                                //récupération du nb de pv restant, utilisation de gcvt car on passe d'un float à un char*
                                char buf[256];
                                char* pv = gcvt(ordinateur.unite[0].pv, 3, buf);

                                char infos[50];
                                strcpy(infos, "Unite no 1 : PV = ");
                                strcat(infos, pv);
                                strcat(infos, "/300");
            
                                texte = TTF_RenderText_Blended(police15, infos, blanc);
                                int w1 = texte->w;
                                int h1 = texte->h;
            
                                //on charge le texte dans la ttf et on l'affiche
                                GLuint text_texte1 = Text_Load(texte);
                                afficheTexte(text_texte1, 0.59, 0.14, w1, h1);
                            } else{
                                selection = 0;
                                action = 0;
                                deplacer = 0;
                                attaquer = 0;
                                playing = 0;
            
                                if(victoire(&j1, perteUniteJ1) == 1){
                                    afficheMap(texture_defaiteIA, 0.5, 0.5, 1, 1);
                                }
                                if(victoire(&ordinateur, perteUniteOrdi) == 1){
                                    afficheMap(texture_victoireIA, 0.5, 0.5, 1, 1);
                                }
                            }
                        }
                    }
    
            /* on cherche au tour de qui c'est et on actualise le currentPlayer */
            if (playing%2 != 0){
                if (ordi == 0){
                    currentPlayer = &j1;
                    otherPlayer = &j2;
                    afficheMap(texture_tourJ1, 0.13, 0.19, 0.035, 0.03);
                } else {
                    currentPlayer = &j1;
                    otherPlayer = &ordinateur;
                }
            }
            if (playing%2 == 0 && playing!=0){
                if (ordi == 0){
                        currentPlayer = &j2;
                        otherPlayer = &j1;
                        afficheMap(texture_tourJ2, 0.87, 0.19, 0.035, 0.03);
                    } else {
                        currentPlayer = &ordinateur;
                        otherPlayer = &j1;

                        /* ******************* implémenter fonction IA ******************* */
                        IA(currentPlayer, otherPlayer, tabCase);

                        playing++;
                        //selection = 1;
                        curseurChoix.x = currentPlayer->unite[0].x;
                        curseurChoix.y = currentPlayer->unite[0].y;
                        //loop++;
                    }
                }
    
            if (intro1 == 1){
                for (int i = 0; i < j1.nbUnite; i++){
                    afficheMap(texture_map, 0.5, 0.5, 1, 1);
                    afficheMap(texture_intro1, 0.5, 0.5, 1, 1);
                    afficheMap(j1.unite[i].image, (j1.unite[i].x + 5)/30.0, (j1.unite[i].y + 9)/34.0, 0.05, 0.045);
                }
            }
    
            if (intro2 == 1){
                for (int i = 0; i < j2.nbUnite; i++){
                    afficheMap(texture_map, 0.5, 0.5, 1, 1);
                    afficheMap(texture_intro2, 0.5, 0.5, 1, 1);
                    afficheMap(j2.unite[i].image, (j2.unite[i].x + 5)/30.0, (j2.unite[i].y + 9)/34.0, 0.05, 0.045);
                }
            }
    
            if (selection == 1 && currentPlayer != &ordinateur){
                for (int i = 0; i < currentPlayer->nbUnite; i++) {
                    if (currentPlayer->unite[i].pv > 0){
                        curseurChoix.x = currentPlayer->unite[i].x;
                        curseurChoix.y = currentPlayer->unite[i].y;
                        break;
                    }
                }
                selection++;
            }
    
            //au début de chaque tour, le mode sélection s'affiche
            if (selection == 2 && victoire(&j1, perteUniteJ1) == 0 && victoire(&j2, perteUniteJ2) == 0){
                if ( ordi == 0  ){
                    afficheMap(texture_choix, 0.5, 0.5, 1, 1);
                    afficheMap(texture_menu_burger, 0.5, 0.5, 1, 1);
    
                    /* affichage texte sélection */
                    char choix[50];
                    strcpy(choix, "Joueur ");
    
                    char buf[10];
                    char* NUMunite = gcvt(currentPlayer->id, 1, buf);
    
                    strcat(choix, NUMunite);
                    strcat(choix, ", choisissez une unite ! ");
    
                    texte = TTF_RenderText_Blended(police20, choix, noir);
                    int w = texte->w;
                    int h = texte->h;
    
                    //on charge le texte dans la ttf et on l'affiche
                    GLuint text_texte = Text_Load(texte);
                    afficheTexte(text_texte, 0.25, 0.925, w, h);
    
    
                    afficheMap(curseurChoix.image, (curseurChoix.x + 5)/30.0, (curseurChoix.y + 9)/34.0, 0.05, 0.045);
                }
                else{
                    afficheMap(texture_selectionIA, 0.5, 0.5, 1, 1);
                    afficheMap(curseurChoix.image, (curseurChoix.x + 5)/30.0, (curseurChoix.y + 9)/34.0, 0.05, 0.045);
                    afficheMap(texture_menu_burger, 0.5, 0.5, 1, 1);
                }
            }
    
            //après avoir sélectionné une de ses unités, le mode action apparait
            if (action == 1){
                //on vérifie si l'unité sélectionnée peut attaquer ou pas et on affiche en conséquence
                if (verifAttaque(currentPlayer, otherPlayer, currentUnite, tabCase) >= 0){
                    if (ordi == 0){
                        afficheMap(texture_choix, 0.5, 0.5, 1, 1);
                    } else{
                        afficheMap(texture_actionIA, 0.5, 0.5, 1, 1);
                    }
                    afficheMap(texture_deplacer, 0.5, 0.5, 1, 1);
                    afficheMap(texture_attaquer, 0.5, 0.5, 1, 1);
                }else{
                    if (ordi == 0){
                        afficheMap(texture_choix, 0.5, 0.5, 1, 1);
                    } else{
                        afficheMap(texture_actionIA, 0.5, 0.5, 1, 1);
                    }
                    afficheMap(texture_deplacer, 0.5, 0.5, 1, 1);
                }
    
                if (ordi == 0){
                    /* affichage texte sur le choix d'une action */
    
                    char choix[50];
                    strcpy(choix, "Joueur ");
    
                    char buf[10];
                    char* NUMunite = gcvt(currentPlayer->id, 1, buf);
    
                    strcat(choix, NUMunite);
                    strcat(choix, ", voici ce que vous pouvez faire : ");
    
                    texte = TTF_RenderText_Blended(police20, choix, noir);
                    int w = texte->w;
                    int h = texte->h;
    
                    //on charge le texte dans la ttf et on l'affiche
                    GLuint text_texte = Text_Load(texte);
                    afficheTexte(text_texte, 0.17, 0.94, w, h);
                }
            }

            /* si on est dans le mode attaque on initialise d'abord la position du curseur avec la position du joueur */
            if (attaquer == 1){
                curseurAtt.x = currentPlayer->unite[currentUnite].x;
                curseurAtt.y = currentPlayer->unite[currentUnite].y;
                attaquer = 2;
            }
    
            //puis on passe à attaque=2 car sinon le curseur reste toujours à la position du currentPlayer
            //affichage du curseur
            if (attaquer == 2){
                afficheMap(curseurAtt.image, (curseurAtt.x + 5)/30.0, (curseurAtt.y + 9)/34.0, 0.05, 0.045);
            }

            //menu achat
            if (achat == 1){
                afficheMap(texture_map, 0.5, 0.5, 1, 1);
                afficheMap(texture_menu_achat, 0.5, 0.5, 1, 1);
            }
            
            //menu aide
            if (aide == 1){
                afficheMap(texture_map, 0.5, 0.5, 1, 1);
                afficheMap(texture_help, 0.5, 0.5, 1, 1);
            }

            //si la personne sélectionne une mauvaise unité
            if (erreur == 1){
                afficheMap(texture_erreur, 0.5, 0.5, 1, 1);
            }
    
            //si la personne est en train de déplacer son unité hibou et qu'elle veut la positionner sur de l'eau
            if (HibouEau == 1) {
                afficheMap(texture_erreurhibou, 0.5, 0.5, 1, 1);
            }
    
    
            //si la personne veut quitter le jeu
            if (quitter == 1){
                afficheMap(texture_quitter, 0.5, 0.5, 1, 1);
            }
    
    
            /* Echange du front et du back buffer : mise a jour de la fenetre */
            SDL_GL_SwapBuffers();
    
            /* Boucle traitant les evenements */
            SDL_Event e;
            while(SDL_PollEvent(&e))
            {
                /* L'utilisateur ferme la fenetre : */
                if(e.type == SDL_QUIT){
                    quitter = 1;
                    break;
                }
    
                if(e.type == SDL_KEYDOWN && (e.key.keysym.sym == SDLK_q || e.key.keysym.sym == SDLK_ESCAPE) && quitter == 0){
                    quitter = 1;
                    break;
                }
    
                if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE && quitter == 1){
                    quitter = 0;
                    break;
                }
    
                if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN && quitter == 1) {
                    loop = 0;
                    break;
                }
    
                /* Quelques exemples de traitement d'evenements : */
                switch(e.type){
                //Redimensionnement fenetre
                case SDL_VIDEORESIZE:
                    reshape(&surface, e.resize.w, e.resize.h);
                    break;
    
                /* Clic souris */
                case SDL_MOUSEBUTTONUP:
                    switch(e.button.button) {
                        case SDL_BUTTON_LEFT:
                            positionX = e.button.x*30/600;
                            positionY = e.button.y*34/680;
        
                            //pour quitter le jeu si on est dans le menu et qu'on clique sur 'Quit'
                            if( chgtTexture == 0 && quitter == 0 && positionX > 11 && positionX < 18 && positionY > 19 && positionY < 22){
                                loop = 0;
                            }
        
                            //pour lancer le jeu si on est dans le menu et qu'on clique sur 'Play'
                            if( chgtTexture == 0 && quitter == 0 && positionX > 11 && positionX < 18 && positionY > 15 && positionY < 18){
                                chgtTexture = 1;
                                break;
                            }
        
                            if (chgtTexture == 1 && positionX > 8 && positionX < 21 && positionY > 10 && positionY < 13){
                                chgtTexture = 2;
                                playing++;
                                intro1 = 1;
                                tabCase[20 - (int)j2.unite[0].y][(int)j2.unite[0].x - 1].occupied = 1;
                                break;
                            }
        
                            if (chgtTexture == 1 && positionX > 8 && positionX < 21 && positionY > 16 && positionY < 19){
                                ordi = 1;
                                playing++;
                                chgtTexture = 2;
                                selection = 1;
                                break;
                            }
    
                        /* --------------- MODE INTRO 1 --------------------*/
    
                        // Si on clique sur acheter on enclenche le mode achat
                        if(intro1 == 1 && positionX > 6 && positionX < 13 && positionY > 29 && positionY < 33){
                            achat = 1; // affichage du mode achat
                            break;
                        }
    
                        // Si on clique sur jouer on enclenche le mode intro2
                        if(intro1 == 1 && positionX > 17 && positionX < 24 && positionY > 29 && positionY < 33){
                            intro2 = 1; // affichage du mode intro2
                            intro1 = 0;
                            playing++;
                            break;
    
                        }
    
                        /* --------------- MODE INTRO 2 --------------------*/
    
                        if(intro2 == 1 && positionX > 6 && positionX < 13 && positionY > 29 && positionY < 33){
                            achat = 1; // affichage du mode achat
                            break;
                        }
    
                        if(intro2 == 1 && positionX > 17 && positionX < 24 && positionY > 29 && positionY < 33){
                            selection = 1; // affichage du mode action
                            intro2 = 0;
                            playing++;
                            break;
                        }
    
                        /* --------------- MODE ACHAT --------------------*/
    
                        // si on clique sur les unités
                        // Choix type unité 1
                        if(achat == 1 && positionX > 5 && positionX < 11 && positionY > 13 && positionY < 19){
                            achatUnite(currentPlayer, 1, currentPlayer->nbUnite, playing, tabCase);
                            currentPlayer->nbUnite++;
                            achat = 0;
                        }
    
                        // Choix type unité 2
                        if(achat == 1 && positionX > 12 && positionX < 18 && positionY > 13 && positionY < 19){
                            achatUnite(currentPlayer, 2, currentPlayer->nbUnite, playing, tabCase);
                            currentPlayer->nbUnite++;
                            achat = 0;
                        }
    
                        // Choix type unité 3
                        if(achat == 1 && positionX > 19 && positionX < 25 && positionY > 13 && positionY < 19){
                            achatUnite(currentPlayer, 3, currentPlayer->nbUnite, playing, tabCase);
                            currentPlayer->nbUnite++;
                            achat = 0;
                        }
    
                        // Si on appuie sur le bouton menu
                        // Affiche le menu achat
                        if(achat == 0 && positionX > 27 && positionX < 30 && positionY > 4 && positionY < 14 ) {
                            achat = 1;
                        }
    
                        // Si bouton retour pressé
                        // Retour sur la map
                        if(achat == 1 && positionX > 11 && positionX < 19 && positionY > 27 && positionY < 30 ){
                            achat = 0;
                        }
    
                        /* --------------- MODE AIDE --------------------*/
                        if(chgtTexture == 2 && positionX > 0 && positionX < 3 && positionY > 4 && positionY < 8 ){
                            aide = 1;
                        }
                        
                        if(aide == 1 && positionX > 10 && positionX < 20 && positionY > 29 && positionY < 32 ){
                            aide = 0;
                        }
                        
    
                        //si on est dans le mode "action" et qu'on clique sur "se déplacer"
                        if (action == 1 && positionX > 6 && positionX < 13 && positionY > 2 && positionY < 4)
                        {
                            deplacer = 1;
                            action = 0;
                        }
    
                        //si on est dans le mode "action" et qu'on clique sur "attaquer"
                        if (action == 1 && positionX > 16 && positionX < 23 && positionY > 2 && positionY < 4)
                        {
                            attaquer = 1;
                            action = 0;
                        }
    
                        //si on est sur une erreur et qu'on clique sur 'OK'
                        if (erreur == 1 && positionX > 13 && positionX < 16 && positionY > 18 && positionY < 20)
                        {
                            erreur = 0;
                        }
    
                        //si on a voulu placer le hibou dans l'eau
                        if (HibouEau == 1 && positionX > 10 && positionX < 19 && positionY > 17 && positionY < 20)
                        {
                            HibouEau = 0;
                        }
    
                        if (quitter == 1 && positionX > 9 && positionX < 12 && positionY > 19 && positionY < 21) {
                            loop = 0;
                        }
    
                        if (quitter == 1 && positionX > 16 && positionX < 19 && positionY > 19 && positionY < 21) {
                            quitter = 0;
                        }
    
                        break;
                    default:
                        break;
                    }
                    break;
    
                /* Touche clavier */
                case SDL_KEYDOWN:
    
                    //mode sélection
                    if (selection == 2)
                    {
                        switch(e.key.keysym.sym)
                        {
                        case SDLK_UP:
                            deplacercurseur(&curseurChoix, HAUT);
                            break;
                        case SDLK_DOWN:
                            deplacercurseur(&curseurChoix, BAS);
                            break;
                        case SDLK_RIGHT:
                            deplacercurseur(&curseurChoix, DROITE);
                            break;
                        case SDLK_LEFT:
                            deplacercurseur(&curseurChoix, GAUCHE);
                            break;
                        case SDLK_RETURN:
    
                            if (quitter == 0) {
                                if (chercheUnite(currentPlayer, curseurChoix.x, curseurChoix.y) >= 0)
                                {
                                    currentUnite = chercheUnite(currentPlayer, curseurChoix.x, curseurChoix.y);
                                    selection = 0;
                                    action = 1;
                                }
                                else
                                {
                                    if (erreur == 1){
                                        erreur = 0;
                                    } else {
                                        erreur = 1;
                                    }
                                }
                            }
                            break;
    
                        default:
                            break;
                        }
                        break;
                    }
    
                    //si on a cliqué sur "attaquer":
                    if (attaquer == 2)
                    {
                        //condition sur la portée du joueur placée dans les case pour que le joueur puisse déplacer le curseur dans un certain périmètre et non un certain nombre de fois
    
                        switch(e.key.keysym.sym)
                        {
                        case SDLK_UP:
                            if ( (abs(pasX) + abs(pasY+1)) <= currentPlayer->unite[currentUnite].portee && testCaseAtt(tabCase, curseurAtt.x, curseurAtt.y+1, currentPlayer->unite[currentUnite].type) == 1 ) {
                                deplacercurseur(&curseurAtt, HAUT);
                                pasY++;
                            }
                            break;
                        case SDLK_DOWN:
                            if ( (abs(pasX) + abs(pasY-1)) <= currentPlayer->unite[currentUnite].portee && testCaseAtt(tabCase, curseurAtt.x, curseurAtt.y-1, currentPlayer->unite[currentUnite].type) == 1){
                                deplacercurseur(&curseurAtt, BAS);
                                pasY--;
                            }
                            break;
                        case SDLK_RIGHT:
                            if ( (abs(pasX+1) + abs(pasY)) <= currentPlayer->unite[currentUnite].portee && testCaseAtt(tabCase, curseurAtt.x+1, curseurAtt.y, currentPlayer->unite[currentUnite].type) == 1){
                                deplacercurseur(&curseurAtt, DROITE);
                                pasX++;
                            }
                            break;
                        case SDLK_LEFT:
                            if ( (abs(pasX-1) + abs(pasY)) <= currentPlayer->unite[currentUnite].portee && testCaseAtt(tabCase, curseurAtt.x-1, curseurAtt.y, currentPlayer->unite[currentUnite].type) == 1){
                                deplacercurseur(&curseurAtt, GAUCHE);
                                pasX--;
                            }
                            break;
    
                        //le joueur appuie sur entrée pour valider la cible à attaquer
                        //pour l'instant on vérifie juste que la case correspond à celle du joueur adversaire mais on intègrera une fonction qui recherchera pour toutes les unités
                        case SDLK_RETURN:
    
                            if (quitter == 0){
                                if ( chercheUnite(otherPlayer, curseurAtt.x, curseurAtt.y) >= 0){
                                    attaqueUnite = chercheUnite(otherPlayer, curseurAtt.x, curseurAtt.y);
                                    attaque(currentPlayer, otherPlayer, currentUnite, attaqueUnite);   //on attaque le joueur
                                    attaquer = 0;
                                    selection = 1;                            //on revient dans le mode "action"
                                    playing++;
                                    pasX = 0;                              //on réinitialise les "pas"
                                    pasY = 0;
                                }
                                else {
                                    if (erreur == 1){
                                        erreur = 0;
                                    } else {
                                        erreur = 1;
                                    }
                                }
                            }
                            break;
    
                        default :
                            break;
                        }
                        break;
                    }
    
                    //si on a cliqué sur "se déplacer" :
                    if (deplacer == 1){
                        //condition sur la mobilité du joueur pour savoir de combien de cases il peut avancer
                        //condition déplacée à l'intérieur des case pour que le joueur avance dans un certain périmètre
                        switch(e.key.keysym.sym){
                            case SDLK_UP:
                                if ( ( (abs(pasX) + abs(pasY+1)) <= currentPlayer->unite[currentUnite].mobilite ) && (testCase(tabCase, currentPlayer->unite[currentUnite].x, currentPlayer->unite[currentUnite].y+1, currentPlayer->unite[currentUnite].type) == 1) ){
        
                                    deplacerjoueur(currentPlayer, HAUT, currentUnite, tabCase);
                                    pasY ++ ;
                                }
                                break;
                            case SDLK_DOWN:
                                if ( ((abs(pasX) + abs(pasY-1)) <= currentPlayer->unite[currentUnite].mobilite) && (testCase(tabCase, currentPlayer->unite[currentUnite].x, currentPlayer->unite[currentUnite].y-1, currentPlayer->unite[currentUnite].type) == 1) ){
        
                                    deplacerjoueur(currentPlayer, BAS, currentUnite, tabCase);
                                    pasY -- ;
                                }
                                break;
                            case SDLK_RIGHT:
                                if ( ((abs(pasX+1) + abs(pasY)) <= currentPlayer->unite[currentUnite].mobilite) && (testCase(tabCase, currentPlayer->unite[currentUnite].x+1, currentPlayer->unite[currentUnite].y, currentPlayer->unite[currentUnite].type) == 1) ){
        
                                    deplacerjoueur(currentPlayer, DROITE, currentUnite, tabCase);
                                    pasX ++ ;
                                }
                                break;
                            case SDLK_LEFT:
                                if ( ((abs(pasX-1) + abs(pasY)) <= currentPlayer->unite[currentUnite].mobilite) && (testCase(tabCase, currentPlayer->unite[currentUnite].x-1, currentPlayer->unite[currentUnite].y, currentPlayer->unite[currentUnite].type) == 1) ){
        
                                    deplacerjoueur(currentPlayer, GAUCHE, currentUnite, tabCase);
                                    pasX -- ;
                                }
                            break;
    
                            //le joueur valide son déplacement
                            case SDLK_RETURN:
                                if (quitter == 0 && HibouEau == 0 && testSePoser(tabCase, currentPlayer->unite[currentUnite].x, currentPlayer->unite[currentUnite].y) == 1) {
                                    //Appel pour savoir si l'unité est sur la case magique
                                    caseMagique(currentPlayer, &chance, currentUnite);
                                    pasX = 0;        //on réinitialise les "pas"
                                    pasY = 0;
                                    playing++;
                                    deplacer = 0;
                                    selection = 1;         //on revient dans le mode "action"
                                    break;
                                }
                                if (quitter == 0 && HibouEau == 0 && testSePoser(tabCase, currentPlayer->unite[currentUnite].x, currentPlayer->unite[currentUnite].y) == 0) {
                                    HibouEau = 1;
                                    break;
                                }
                                if (HibouEau == 1) {
                                    HibouEau = 0;
                                    break;
                                }
                            default :
                                break;
                            }
                            break;
                        }
    
                        switch (e.key.keysym.sym){
                            case SDLK_RETURN:
                                if (chgtTexture == 0 && quitter == 0) {
                                    chgtTexture = 1;
                                    playing++;
                                    intro1 = 1;
                                    break;
                                }
                                if (intro1 == 1 && quitter == 0){
                                    intro1 = 0;
                                    intro2 = 1;
                                    playing++;
                                    break;
                                }
                                if (intro2 == 1 && quitter == 0) {
                                    intro2 = 0;
                                    playing++;
                                    selection = 1;
                                    break;
                                }
                            default:
                                break;
                            }
                            break;
        
                        default:
                            break;
                        }
                    }
    
            /* Calcul du temps ecoule */
            Uint32 elapsedTime = SDL_GetTicks() - startTime;
            /* Si trop peu de temps s'est ecoule, on met en pause le programme */
            if(elapsedTime < FRAMERATE_MILLISECONDS){
                SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
            }
        }
    
        /* Libération des textures */
        glDeleteTextures(1, &texture_map);
        glDeleteTextures(1, &texture_menu);
        glDeleteTextures(1, &texture_infos);
        glDeleteTextures(1, &texture_forest);
        glDeleteTextures(1, &texture_guardians);
        glDeleteTextures(1, &texture_play);
        glDeleteTextures(1, &texture_quit);
        glDeleteTextures(1, &texture_choix);
        glDeleteTextures(1, &texture_deplacer);
        glDeleteTextures(1, &texture_attaquer);
    
        for (int i = 0; i < j1.nbUnite; i++)
        {
            glDeleteTextures(1, &j1.unite[i].image);
        }
        for (int i = 0; i < j2.nbUnite; i++)
        {
            glDeleteTextures(1, &j2.unite[i].image);
        }
    
        glDeleteTextures(1, &curseurAtt.image);
        glDeleteTextures(1, &curseurChoix.image);
        glDeleteTextures(1, &texture_erreur);
        glDeleteTextures(1, &texture_intro1);
        glDeleteTextures(1, &texture_intro2);
        glDeleteTextures(1, &texture_menu_achat);
        glDeleteTextures(1, &texture_victoireJ1);
        glDeleteTextures(1, &texture_victoireJ2);
        glDeleteTextures(1, &texture_quitter);
        glDeleteTextures(1, &texture_erreurhibou);
        glDeleteTextures(1, &texture_2menu);
        glDeleteTextures(1, &texture_2players);
        glDeleteTextures(1, &texture_computer);
        glDeleteTextures(1, &texture_selectionIA);
        glDeleteTextures(1, &texture_actionIA);
        glDeleteTextures(1, &ordinateur.unite[0].image);
        glDeleteTextures(1, &texture_boutonhelp);
        glDeleteTextures(1, &texture_help);
        glDeleteTextures(1, &texture_victoireIA);
        glDeleteTextures(1, &texture_defaiteIA);
    
    
        /* Liberation de la m�moire occupee par img */
        SDL_FreeSurface(surface);
        SDL_FreeSurface(texte);
    
        /*Libération de la mémoire de la SDL_ttf */
    
        TTF_CloseFont(police15); /* Doit être avant TTF_Quit() */
        TTF_CloseFont(police20);
        TTF_Quit();
    
        /* Liberation des ressources associees a la SDL */
        SDL_Quit();
    
        return EXIT_SUCCESS;
    }
