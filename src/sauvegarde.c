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

// inclusion biblitohèques
#include <SDL/SDL.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL/SDL_image.h>

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

    /* Ouverture d'une fenetre et creation d'un contexte OpenGL */
    SDL_Surface* surface;
    reshape(&surface, WINDOW_WIDTH, WINDOW_HEIGHT);

    /* Initialisation du titre de la fenetre */
    SDL_WM_SetCaption(WINDOW_TITLE, NULL);

    /* --------------
     INITIALISATION JOUEUR
     -------------- */


    //création d'un joueur temporaire qui permet de savoir qui doit jouer
    Joueur* currentPlayer;

    Joueur* otherPlayer;

    Joueur j1;
    // Attention � l'allocation de la m�moire !
    j1.id = 1;
    j1.name = "Loona";    //chez moi faut mettre des '  '  et pas des "  " pour que ça marche
    j1.cagnotte = 200;
    j1.nbUnite = 1;
    
    /* --------------
     INITIALISATION UNITES
     -------------- */

//    j1.unite;
    j1.unite[0].pv = 100.0;
    j1.unite[0].mobilite = 5;
    j1.unite[0].portee = 4;
    j1.unite[0].force = 0.3;
    j1.unite[0].x = 10.0;
    j1.unite[0].y = 10.0;
    j1.unite[0].image = Texture_Load("./img/crabe_rouge.png", 31, 31); // 31x31 taille de l'image
    
    /* --------------
     INITIALISATION JOUEUR
     -------------- */

    Joueur j2;
    // Attention � l'allocation de la m�moire !
    j2.id = 2;
    j2.name = "Nelly";
    j2.cagnotte = 200;
    j2.nbUnite = 1;

    /* --------------
     INITIALISATION UNITES
     -------------- */
    
//    j2.unite;
    j2.unite[0].pv = 100.0;
    j2.unite[0].mobilite = 5;
    j2.unite[0].portee = 4;
    j2.unite[0].force = 0.3;
    j2.unite[0].x = 7.0;
    j2.unite[0].y = 7.0;
    j2.unite[0].image = Texture_Load("./img/unite1.png", 31, 31); // 31x31 taille de l'image


    //initialisation curseur attaque

    Curseur curseurAtt;
    curseurAtt.image = Texture_Load("./img/curseurattaque.png", 32, 32);

    //initialisation curseur choix unité

    Curseur curseurChoix;
    curseurChoix.image = Texture_Load("./img/curseur.png", 32, 32);
    curseurChoix.x = 10.0;
    curseurChoix.y = 10.0;

    /* --------------
     INITIALISATION TEXTURES
     -------------- */

    //initialisation des différents "modes"
    int chgtTexture = 0;
    int playing = 0;

    int selection = 0;
    int action = 0;
    int deplacer = 0;
    int attaquer = 0;

    int erreur = 0;

    int intro1 = 0;
    int intro2 = 0;
    int achat = 0;

    int currentUnite = 0;
    int attaqueUnite = 0;


    //initialisation des compteurs de pas
    //utilisés dans "attaquer" (pour vérifier que le curseur d'attaque doit bien dans le périmètre de portée) ET dans "se déplacer" (pour vérifier que le joueur se déplace dans le périmètre de mobilité)
    int pasX = 0;
    int pasY = 0;

    //c'est juste des valeurs qui servent pour le clic de la souris (c'est plus pratique que d'utiliser les e.ubtton.x là)
    int positionX = 0;
    int positionY = 0;

    //Chargement texture de la map
    GLuint texture_map = Texture_Load("./img/newmap.png", 960, 1088); // 630x630 taille de l'image + taille de la fen�tre

    //chargement texture infos pv
    GLuint texture_infos = Texture_Load("./img/infostest.png", 960, 1088);

    //chargement texture mode sélection d'unité
    GLuint texture_selection = Texture_Load("./img/choixunite.png", 960, 1088);

    //Chargement texture erreur
    GLuint texture_erreur = Texture_Load("./img/erreurunite.png", 960, 1088);

    //Chargement texture du menu
    GLuint texture_menu = Texture_Load("./img/newmenu.png", 960, 1088);
    GLuint texture_forest = Texture_Load("./img/forest.png", 411, 95);
    GLuint texture_guardians = Texture_Load("./img/guardians.png", 633, 98);
    GLuint texture_play = Texture_Load("./img/play.png", 354, 114);
    GLuint texture_quit = Texture_Load("./img/quit.png", 335, 117);

    //Chargement texture du mode de choix
    GLuint texture_choix = Texture_Load("./img/testchoix.png", 960, 1088);
    GLuint texture_deplacer = Texture_Load("./img/deplacer.png", 960, 1088);
    GLuint texture_attaquer = Texture_Load("./img/attaquer.png", 960, 1088);
    GLuint texture_menu_burger = Texture_Load("./img/menu_burger.png", 960, 1088);

    //Chargement texture du mode intro1 *message de bonjour joueur1*
    GLuint texture_intro1 = Texture_Load("./img/fond-intro1.png", 960, 1088);

    //Chargement texture du mode intro2 *message de bonjour joueur2*
    GLuint texture_intro2 = Texture_Load("./img/fond-intro2.png", 960, 1088);
    
    //Chargement texture mode achat
    GLuint texture_menu_achat = Texture_Load("./img/menu-achat.png", 960, 1088);

    /* Boucle principale */
    int loop = 1;
    while(loop)
    {
        /* Recuperation du temps au debut de la boucle */
        Uint32 startTime = SDL_GetTicks();
        /* Code de dessin */
        glClear(GL_COLOR_BUFFER_BIT);


        /* si on est dans le menu */
        if (chgtTexture == 0)
        {
            afficheMap(texture_menu, 0.5, 0.5, 1, 1);
            afficheMap(texture_forest, 0.5, 0.75, 0.33, 0.1);
            afficheMap(texture_guardians, 0.5, 0.64, 0.5, 0.15);
            afficheMap(texture_play, 0.5, 0.5, 0.2, 0.06);
            afficheMap(texture_quit, 0.5, 0.38, 0.2, 0.06);
        }

        /* si on est dans le jeu */
        if (chgtTexture == 1) {
            if(victoire(&j1, &j2) == 0){
                //Affichage de la map
                afficheMap(texture_map, 0.5, 0.5, 1, 1);
                afficheMap(texture_infos, 0.5, 0.5, 1, 1);

                for (int i = 0; i < j1.nbUnite; i++)
                {
                    if(mort(&j1, i) == 0){
                        afficheMap(j1.unite[i].image, (j1.unite[i].x + 5)/30.0, (j1.unite[i].y + 9)/34.0, 0.05, 0.045);
                    }
                }
                for (int i = 0; i < j2.nbUnite; i++)
                {
                    if(mort(&j2, i) == 0){
                        afficheMap(j2.unite[i].image, (j2.unite[i].x + 5)/30.0, (j2.unite[i].y + 9)/34.0, 0.05, 0.045);
                    }
                }
            }else {
                int selection = 0;
                int action = 0;
                int deplacer = 0;
                int attaquer = 0;
                
                if(victoire(&j1, &j2) == 1){
                    afficheMap(texture_map, 0.5, 0.5, 1, 1);
                    afficheMap(texture_intro2, 0.5, 0.5, 1, 1);
                }
               if(victoire(&j1, &j2) == 2){
                    afficheMap(texture_map, 0.5, 0.5, 1, 1);
                    afficheMap(texture_intro1, 0.5, 0.5, 1, 1);
               }
            }
        }

        /* on cherche au tour de qui c'est et on actualise le currentPlayer */
        if (playing%2 != 0)
        {
            currentPlayer = &j1;
            otherPlayer = &j2;
        }
        if (playing%2 == 0)
        {
            currentPlayer = &j2;
            otherPlayer = &j1;
        }

        if (intro1 == 1) {
            for (int i = 0; i < j1.nbUnite; i++){
                afficheMap(texture_map, 0.5, 0.5, 1, 1);
                afficheMap(texture_intro1, 0.5, 0.5, 1, 1);
                afficheMap(j1.unite[i].image, (j1.unite[i].x + 5)/30.0, (j1.unite[i].y + 9)/34.0, 0.05, 0.045);
            }
        }

        if (intro2 == 1) {
            for (int i = 0; i < j2.nbUnite; i++){
                afficheMap(texture_map, 0.5, 0.5, 1, 1);
                afficheMap(texture_intro2, 0.5, 0.5, 1, 1);
                afficheMap(j2.unite[i].image, (j2.unite[i].x + 5)/30.0, (j2.unite[i].y + 9)/34.0, 0.05, 0.045);
            }
        }

        if (achat == 1){
            afficheMap(texture_map, 0.5, 0.5, 1, 1);
            afficheMap(texture_menu_achat, 0.5, 0.5, 1, 1);

        }

        //au début de chaque tour, le mode sélection s'affiche
        if (selection == 1) {
            afficheMap(texture_selection, 0.5, 0.5, 1, 1);
            afficheMap(curseurChoix.image, (curseurChoix.x + 5)/30.0, (curseurChoix.y + 9)/34.0, 0.05, 0.045);
        }

        //après avoir sélectionné une de ses unités, le mode action apparait
        if (action == 1) {

            //on vérifie si l'unité sélectionnée peut attaquer ou pas et on affiche en conséquence
            if (verifAttaque(currentPlayer, otherPlayer, currentUnite) >= 0)
            {
                afficheMap(texture_choix, 0.5, 0.5, 1, 1);
                afficheMap(texture_deplacer, 0.5, 0.5, 1, 1);
                afficheMap(texture_attaquer, 0.5, 0.5, 1, 1);
                afficheMap(texture_menu_burger, 0.5, 0.5, 1, 1);
            } else {

                afficheMap(texture_choix, 0.5, 0.5, 1, 1);
                afficheMap(texture_deplacer, 0.5, 0.5, 1, 1);
                afficheMap(texture_menu_burger, 0.5, 0.5, 1, 1);
            }
        }


        /* si on est dans le mode attaque on initialise d'abord la position du curseur avec la position du joueur */
        if (attaquer == 1) {
            curseurAtt.x = currentPlayer->unite[currentUnite].x;
            curseurAtt.y = currentPlayer->unite[currentUnite].y;
            attaquer = 2;
        }

        //puis on passe à attaque=2 car sinon le curseur reste toujours à la position du currentPlayer
        //affichage du curseur
        if (attaquer == 2) {
            afficheMap(curseurAtt.image, (curseurAtt.x + 5)/30.0, (curseurAtt.y + 9)/34.0, 0.05, 0.045);
        }

        //si la personne sélectionne une mauvaise unité
        if (erreur == 1) {
            afficheMap(texture_erreur, 0.5, 0.5, 1, 1);
        }


        /* Echange du front et du back buffer : mise a jour de la fenetre */
        SDL_GL_SwapBuffers();

        /* Boucle traitant les evenements */
        SDL_Event e;
        while(SDL_PollEvent(&e))
        {
            /* L'utilisateur ferme la fenetre : */
            if(e.type == SDL_QUIT)
            {
                loop = 0;
                break;
            }

            if(e.type == SDL_KEYDOWN
                && (e.key.keysym.sym == SDLK_q || e.key.keysym.sym == SDLK_ESCAPE))
            {
                loop = 0;
                break;
            }

            /* Quelques exemples de traitement d'evenements : */
            switch(e.type)
            {
                //Redimensionnement fenetre    (en commentaire chez moi psq sinon �a marche pas)
                case SDL_VIDEORESIZE:
                    reshape(&surface, e.resize.w, e.resize.h);
                    break;

                /* Clic souris */
                case SDL_MOUSEBUTTONUP:
                    printf("clic en (%d, %d)\n", e.button.x, e.button.y);
                    switch(e.button.button)
                    {
                        case SDL_BUTTON_LEFT:
                            positionX = e.button.x*30/600;
                            positionY = e.button.y*34/680;

                            //pour quitter le jeu si on est dans le menu et qu'on clique sur 'Quit'
                            if( chgtTexture == 0 && positionX > 11 && positionX < 18 && positionY > 19 && positionY < 22){
                                loop = 0;
                            }

                            //pour lancer le jeu si on est dans le menu et qu'on clique sur 'Play'
                            if( chgtTexture == 0 && positionX > 11 && positionX < 18 && positionY > 15 && positionY < 18){
                                playing++;
                                chgtTexture = 1;
                                printf("Intro");
                                intro1 = 1;      //ça active le mode "action" qui est le mode où l'on choisit si on veut attaquer où se déplacer
                            }

                            /* --------------- MODE INTRO 1 --------------------*/

                            // Si on clique sur acheter on enclenche le mode achat
                            if(intro1 == 1 && positionX > 6 && positionX < 13 && positionY > 29 && positionY < 33){
                                printf("intro1 achat \n");
                                printf("intro2 = \n %d", intro2);
                                achat = 1; // affichage du mode achat
                                break;
                            }

                            // Si on clique sur jouer on enclenche le mode intro2
                            if(intro1 == 1 && positionX > 17 && positionX < 24 && positionY > 29 && positionY < 33){
                                printf("intro1 jouer \n");
                                printf("intro2 = \n %d", intro2);
                                intro2 = 1; // affichage du mode intro2
                                intro1 = 0; // ferme le mode intro 1
                                playing++; // permet de changer de joueur
                                break;

                            }

                            /* --------------- MODE INTRO 2 --------------------*/

                            if(intro2 == 1 && positionX > 6 && positionX < 13 && positionY > 29 && positionY < 33){
                                printf("intro2 achat \n");
                                achat = 1; // affichage du mode achat
                                break;
                            }

                            if(intro2 == 1 && positionX > 17 && positionX < 24 && positionY > 29 && positionY < 33){
                                printf("intro2 jouer \n");
                                selection = 1; // affichage du mode action
                                intro2 = 0;
                                playing++; // permet de changer de joueur
                                break;
                            }

                            /* --------------- MODE ACHAT --------------------*/

                            // si on clique sur les unités
                            // Choix type unité 1
                            if(achat == 1 && positionX > 5 && positionX < 12 && positionY > 13 && positionY < 20){
                                printf("unite ok");
                                achatUnite(currentPlayer, 1, currentPlayer->nbUnite);
                                currentPlayer->nbUnite++;
                                printf("\n Nombre d'unite :%d", currentPlayer->nbUnite);
                                achat = 0;
                            }
                            // Choix type unité 2
                            if(achat == 1 && positionX > 18 && positionX < 25 && positionY > 13 && positionY < 20){
                                printf("unite ok");
                                achatUnite(currentPlayer, 2, currentPlayer->nbUnite);
                                currentPlayer->nbUnite++;
                                printf("\n Nombre d'unite :%d", currentPlayer->nbUnite);
                                achat = 0;
                            }
                            
                            if(achat == 0 && positionX > 27 && positionX < 30 && positionY > 4 && positionY < 14 ){
                                achat=1;
                                printf("MENU demandé \n");
                            }


                            //si on est dans le mode "action" et qu'on clique sur "se déplacer"
                            if (action == 1 && positionX > 6 && positionX < 13 && positionY > 2 && positionY < 4) {
                                deplacer = 1;
                                action = 0;
                            }

                            //si on est dans le mode "action" et qu'on clique sur "attaquer"
                            if (action == 1 && positionX > 16 && positionX < 23 && positionY > 2 && positionY < 4) {
                                attaquer = 1;
                                action = 0;
                            }

                            //si on est sur une erreur et qu'on clique sur 'OK'
                            if (erreur == 1 && positionX > 13 && positionX < 16 && positionY > 18 && positionY < 20) {
                                erreur = 0;
                            }

                            break;
                        default:
                            break;
                    }
                    break;

                /* Touche clavier */
                case SDL_KEYDOWN:
                    printf("touche pressee (code = %d)\n", e.key.keysym.sym);

                    //mode sélection
                    if (selection == 1) {

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
                                if (chercheUnite(currentPlayer, curseurChoix.x, curseurChoix.y) >= 0)
                                {
                                    currentUnite = chercheUnite(currentPlayer, curseurChoix.x, curseurChoix.y);
                                    selection = 0;
                                    action = 1;
                                } else {
                                    erreur = 1;
                                }
                                break;

                            default:
                                break;
                        } break;

                    }

                    //si on a cliqué sur "attaquer":
                    if (attaquer == 2) {

                        //condition sur la portée du joueur placée dans les case pour que le joueur puisse déplacer le curseur dans un certain périmètre et non un certain nombre de fois

                        switch(e.key.keysym.sym)
                        {
                            case SDLK_UP:
                                if ( (abs(pasX) + abs(pasY+1)) <= currentPlayer->unite[currentUnite].portee )
                                {
                                    deplacercurseur(&curseurAtt, HAUT);
                                    pasY++;
                                }
                                break;
                            case SDLK_DOWN:
                                if ( (abs(pasX) + abs(pasY-1)) <= currentPlayer->unite[currentUnite].portee )
                                {
                                    deplacercurseur(&curseurAtt, BAS);
                                    pasY--;
                                }
                                break;
                            case SDLK_RIGHT:
                                if ( (abs(pasX+1) + abs(pasY)) <= currentPlayer->unite[currentUnite].portee )
                                {
                                    deplacercurseur(&curseurAtt, DROITE);
                                    pasX++;
                                }
                                break;
                            case SDLK_LEFT:
                                if ( (abs(pasX-1) + abs(pasY)) <= currentPlayer->unite[currentUnite].portee )
                                {
                                    deplacercurseur(&curseurAtt, GAUCHE);
                                    pasX--;
                                }
                                break;

                                //le joueur appuie sur entrée pour valider la cible à attaquer
                                //pour l'instant on vérifie juste que la case correspond à celle du joueur adversaire mais on intègrera une fonction qui recherchera pour toutes les unités
                            case SDLK_RETURN:
                                if ( chercheUnite(otherPlayer, curseurAtt.x, curseurAtt.y) >= 0 )
                                {
                                    attaqueUnite = chercheUnite(otherPlayer, curseurAtt.x, curseurAtt.y);
                                    attaque(currentPlayer, otherPlayer, currentUnite, attaqueUnite);   //on attaque le joueur
                                    attaquer = 0;
                                    selection = 1;                            //on revient dans le mode "action"
                                    playing++;
                                    pasX = 0;                              //on réinitialise les "pas"
                                    pasY = 0;
                                } else {
                                    erreur = 1;
                                }
                                break;

                            default :
                                break;
                        } break;
                    }


                    //si on a cliqué sur "se déplacer" :
                    if (deplacer == 1){

                        //condition sur la dextérité du joueur pour savoir de combien de cases il peut avancer
                        //condition déplacée à l'intérieur des case pour que le joueur avance dans un certain périmètre
                            switch(e.key.keysym.sym)
                            {
                                case SDLK_UP:
                                    if ( (abs(pasX) + abs(pasY+1)) <= currentPlayer->unite[currentUnite].mobilite ) {
                                        deplacerjoueur(currentPlayer, HAUT, currentUnite);
                                        printf("\n positionJ1 y haut= %f",currentPlayer->unite[currentUnite].y);
                                        pasY ++ ;
                                    }
                                    break;
                                case SDLK_DOWN:
                                    if ( (abs(pasX) + abs(pasY-1)) <= currentPlayer->unite[currentUnite].mobilite ) {
                                        deplacerjoueur(currentPlayer, BAS, currentUnite);
                                        printf("\n positionJ1 y bas= %f",currentPlayer->unite[currentUnite].y);
                                        pasY -- ;
                                    }
                                    break;
                                case SDLK_RIGHT:
                                    if ( (abs(pasX+1) + abs(pasY)) <= currentPlayer->unite[currentUnite].mobilite ) {
                                        deplacerjoueur(currentPlayer, DROITE, currentUnite);
                                        printf("\n positionJ1 y droite= %f",currentPlayer->unite[currentUnite].x);
                                        pasX ++ ;
                                    }
                                    break;
                                case SDLK_LEFT:
                                    if ( (abs(pasX-1) + abs(pasY)) <= currentPlayer->unite[currentUnite].mobilite ) {
                                        deplacerjoueur(currentPlayer, GAUCHE, currentUnite);
                                        printf("\n positionJ1 y gauche= %f",currentPlayer->unite[currentUnite].x);
                                        pasX -- ;
                                    }
                                    break;

                                    //le joueur valide son déplacement
                                case SDLK_RETURN:
                                    pasX = 0;        //on réinitialise les "pas"
                                    pasY = 0;
                                    playing++;
                                    deplacer = 0;
                                    selection = 1;         //on revient dans le mode "action"
                                    break;

                                default :
                                    break;
                            }
                            break;
                    }


                    switch (e.key.keysym.sym){

                        /* cette partie-là n'est pas encore optimale */
                        case SDLK_m:     //mais en gros ici ça voudrait dire que si tu appuies sur la touche m ça t'envoie sur le menu du jeu (quelque soit la fenêtre où se situe l'utilisateur)
                            chgtTexture = 0;    //en fait je l'ai fait mis au début mais je l'ai pas encore testé mdr
                            break;

                        case SDLK_RETURN:   //et là si t'appuie sur entrée quand t'es dans le menu ça lance le jeu (pas besoin de cliquer avec la souris sur 'play')
                            chgtTexture = 1;  //mais je viens de me dire que sachant que ce switch n'est pas dans une boucle 'if' il devrait être valable tout le temps
                            playing = 1;   //or il ne l'est pas, ça marche que dans le menu (mais c'est un détail pour l'instant)
                            break;

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
        if(elapsedTime < FRAMERATE_MILLISECONDS)
        {
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
//    glDeleteTextures(1, &texture_achatU1);
//    glDeleteTextures(1, &texture_achatU2);
    glDeleteTextures(1, &    texture_menu_achat);


    /* Liberation de la m�moire occupee par img */
    SDL_FreeSurface(surface);

    /* Liberation des ressources associees a la SDL */
    SDL_Quit();

    return EXIT_SUCCESS;
}
