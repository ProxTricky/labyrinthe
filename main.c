#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#define MUR 0
#define CHEMIN 1
#define TAILLE 50


int tableau[TAILLE][TAILLE];

void razTAB()
{
    int i , j;
    for(i=0;i<TAILLE;i++)
    {
        for(j=0;j<TAILLE;j++)
        {
            tableau[i][j]=MUR;
        }
    }
}

void printTAB()
{
    int i , j;
    for(i=0;i<TAILLE;i++)
    {
        for(j=0;j<TAILLE;j++)
        {
			if(tableau[i][j]==CHEMIN)
				printf("%2c",32);
			else
				printf("%2c",219);
        }
        printf("\n");
    }
}

/*
* Fonction qui renvoie une valeur de direction en fonction de probabilités fournies en entrée
* E : les seuils de probabilités entre 1 et 99 pour chacune des directions
* S : une valeur de direction (1 -> haut | 2 -> bas | 3 -> droite | 4 -> gauche)
*/
int direction(int h,int b,int d,int g)
{
	int prop = rand() %100;

	if(prop < h) return 1;
	if(prop < b) return 2;
	if(prop < d) return 3;
	return 4;
}

int longueur()
{
	return rand() %(TAILLE / 10) + 3;
}


void chemin()
{
    //on parcours le labyrinthe du bas vers le haut, en partant de la colonne du milieu
	int ligne=TAILLE-1, colonne=TAILLE/2;
	int i,pas,dir=1;
    int hautAtteint = 0;

	while(hautAtteint == 0)
	{
		pas = longueur(); //longueur d'un tracé


		switch(dir)
		{
            case 1 : //haut

                while(pas > 0 && ligne>=0)
                {
                    tableau[ligne][colonne]=CHEMIN;
                    ligne--;
                    pas --;
                }
                if(ligne<0)
                {
                    hautAtteint = 1;
                }

            break;

            case 2 : //bas
                if(ligne<TAILLE-pas) //si on a la place de tracer un chemin
                {
                    if(tableau[ligne+pas+1][colonne] != CHEMIN)
                    {
                        for(i=0; i<pas; i++)
                        {
                            tableau[ligne][colonne]=CHEMIN;
                            ligne++;
                        }
                    }
                }

            break;
            case 3 : //droite
                if(colonne<TAILLE-pas-1) //si on a la place de tracer un chemin
                {
                    if(tableau[ligne][colonne+pas+1] != CHEMIN)
                    {
                        for(i=0; i<pas; i++)
                        {
                            tableau[ligne][colonne]=CHEMIN;
                            colonne++;
                        }
                    }
                }
            break;
            case 4 : //gauche
                if(colonne>pas) //si on a la place de tracer un chemin
                {
                    if(tableau[ligne][colonne-pas-1] != CHEMIN)
                    {
                        for(i=0; i<pas; i++)
                        {
                            tableau[ligne][colonne]=CHEMIN;
                            colonne--;
                        }
                    }
                }
            break;
            default : printf("direction inconnue\n");
		}


		//tirage aléatoire d'une nouvelle direction pour la prochaine itération
		dir=direction(30,50,75,100);
	}
}

void impasse(int ligne, int colonne)
{
	int l,c,pas,dir;
	int h=1,b=1,d=1,g=1;

	while(h+b+d+g!=0)
	{
		pas = longueur();
		dir=direction(25,50,75,100);

		if(dir<3)
		{
			for(l=0; l<pas; l++)
			{
				if(dir==1)
				{
					if((ligne>2) && (tableau[ligne-2][colonne] != CHEMIN) && (tableau[ligne-3][colonne] != CHEMIN))
					{
						ligne--;
						h=1;
					}
					else h=0;
				}
				else
				{
					if((ligne<TAILLE-2) && (tableau[ligne+2][colonne] != CHEMIN) && (tableau[ligne+3][colonne] != CHEMIN))
					{
						ligne++;
						b=1;
					}
					else b=0;;
				}
				tableau[ligne][colonne]=CHEMIN;
			}
		}
		else
		{
			for(c=0; c<pas; c++)
			{
				if(dir==3)
				{
					if((colonne<TAILLE-2) && (tableau[ligne][colonne+2] != CHEMIN) && (tableau[ligne][colonne+3] != CHEMIN))
					{
						colonne++;
						d=1;
					}
					else d=0;
				}
				else
				{
					if((colonne>2) && (tableau[ligne][colonne-2] != CHEMIN) && (tableau[ligne][colonne-3] != CHEMIN))
					{
						colonne--;
						g=1;
					}
					else g=0;
				}
				tableau[ligne][colonne]=CHEMIN;
			}
		}
	}
}

void genererImpasses(int nb)
{
	int i,x=0,y=0;

	for(i=1; i<nb ; i++)
	{
		while(tableau[x][y]!=CHEMIN)
		{
			x=rand() %TAILLE;
			y=rand() %TAILLE;
		}
		impasse(x,y);
		x=0;
		y=0;
	}
}

int main(int argc, char* argv[])
{
    //int tableau[20][20];
    int xlab = 0;
    int ylab = 0;
    int start = 0;
    int path = 0;
    int end = 0;
    int i = 0;
    int j = 0;
    SDL_Window* window;
    SDL_Renderer* renderer;
    printf("Début du programme\n");

    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)                                                       //depart initialisation.
    {
        printf("Problème d'initialisation :(\n");
        return EXIT_FAILURE;
    }
    printf("SDL Initialisée :)\n");


    window = SDL_CreateWindow("Labyrinthe", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, SDL_WINDOW_SHOWN);
    if(window == 0)
    {
        printf("Problème création de la fenêtre :(\n");
        return EXIT_FAILURE;
    }
    printf("Fenêtre créée :)\n");
    renderer = SDL_CreateRenderer(window, -1, 0);
    if(renderer == 0)
    {
        printf("Problème d'initialisation du renderer:(\n");
        return EXIT_FAILURE;
    }
    printf("Initialisation terminée\n");


    SDL_Texture  *menu;
    SDL_Rect rect1;
    SDL_Rect rect2;
    SDL_Rect rect3;



    SDL_Event event;
    int x , y;
    Uint32 boutons;
    int restart = 1;
    SDL_bool quit = SDL_FALSE;
    SDL_bool quit2 = SDL_FALSE;
    SDL_bool quit3 = SDL_FALSE;
    SDL_bool principal = SDL_FALSE;
    SDL_bool win = SDL_FALSE;


    while (!principal)
    {
        if(restart==1)                                          //Affichage du menu dans un if pour permettre de faire un return.
        {
        SDL_SetRenderDrawColor(renderer, 0, 200, 200, 255);
        SDL_RenderClear(renderer);

        SDL_Surface *MENU = NULL;
        MENU=IMG_Load("menu.png");

        if(!MENU)
        {
            printf("IMG_Load: %s\n", IMG_GetError());
        }

        menu = SDL_CreateTextureFromSurface(renderer, MENU);
        SDL_FreeSurface(MENU);

        rect1.x = 0;
        rect1.y = 0;
        rect1.w = rect1.h = 800;
        SDL_RenderCopy(renderer, menu, NULL, &rect1);




        SDL_RenderPresent(renderer);
        SDL_Delay(50);
        restart = 0;
        }
        else
        {
        principal = SDL_FALSE;
        quit = SDL_FALSE;
        quit2 = SDL_FALSE;
        quit3 = SDL_FALSE;
        win = SDL_FALSE;
        while(!quit)
        {
            SDL_WaitEvent(&event);
            if(event.type == SDL_QUIT)                              // quitter le prgm via la croix
                principal = SDL_TRUE;
                quit = SDL_TRUE;
            boutons = SDL_GetMouseState( &x, &y);
            if(x>100 &&x<700 && y>100 && y<200 && event.button.button == SDL_BUTTON(SDL_BUTTON_LEFT))
            {
                SDL_SetRenderDrawColor(renderer, 0, 200, 200, 255);     /*Affichage de l'image de fond*/
                SDL_RenderClear(renderer);

                SDL_Surface *MENU = NULL;
                MENU=IMG_Load("brique.jpg");

                if(!MENU)
                {
                    printf("IMG_Load: %s\n", IMG_GetError());
                }

                menu = SDL_CreateTextureFromSurface(renderer, MENU);
                SDL_FreeSurface(MENU);

                rect1.x = 0;
                rect1.y = 0;
                rect1.w = rect1.h = 800;
                SDL_RenderCopy(renderer, menu, NULL, &rect1);

                SDL_Rect rect = {150, 150, 500, 500};           /*Affichage du carre de fond du labyrinthe*/
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

                SDL_SetRenderTarget(renderer, menu);
                SDL_RenderFillRect(renderer, &rect);
                SDL_SetRenderTarget(renderer, NULL);
                SDL_RenderPresent(renderer);
                SDL_Delay(50);
                xlab = 150;
                ylab = 150;
                razTAB();
                chemin();
                genererImpasses(TAILLE);
                j=0;

                for(i=0;i<TAILLE;i++)
                {

                    for(j=0;j<TAILLE;j++)
                    {

                        if(tableau[i][j]==0)
                        {
                            SDL_Rect rect2 = {xlab, ylab, 10, 10};
                            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);           //Affichage des chemins du labyrinthe
                            SDL_SetRenderTarget(renderer, menu);
                            SDL_RenderFillRect(renderer, &rect2);
                            SDL_SetRenderTarget(renderer, NULL);
                            xlab = xlab+10;


                        }
                        else if(tableau[i][j]==1)
                        {
                            SDL_Rect rect2 = {xlab, ylab, 10, 10};
                            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);                   //Affichage des murs du labyrinthes
                            SDL_SetRenderTarget(renderer, menu);
                            SDL_RenderFillRect(renderer, &rect2);
                            SDL_SetRenderTarget(renderer, NULL);
                            xlab = xlab+10;

                        }

                    }

                ylab = ylab+10;
                xlab = 150;
                j=0;


                }


                for(i=0;i<TAILLE;i++)
                {
                    start++;
                    j++;
                    if(tableau[0][i]==CHEMIN)
                    {

                        SDL_Rect rect2 = {150+i*10, 150, 10, 10};
                        SDL_SetRenderDrawColor(renderer, 119, 136, 153, 0);           //Affichage des chemins du labyrinthe
                        SDL_SetRenderTarget(renderer, menu);
                        SDL_RenderFillRect(renderer, &rect2);
                        SDL_SetRenderTarget(renderer, NULL);
                        i=TAILLE;

                    }
                }
                    for(i=0;i<TAILLE;i++)
                {
                    end++;
                    if(tableau[TAILLE-1][i]==CHEMIN)
                    {

                        SDL_Rect rect2 = {150+i*10, 630, 10, 10};
                        SDL_SetRenderDrawColor(renderer, 255, 215, 0, 0);           //Affichage des chemins du labyrinthe
                        SDL_SetRenderTarget(renderer, menu);
                        SDL_RenderFillRect(renderer, &rect2);
                        SDL_SetRenderTarget(renderer, NULL);
                        i=TAILLE;

                    }

                }


                SDL_RenderPresent(renderer);                                //render du labyrinthes fini pour eviter les initialisations repete.
                SDL_Delay(50);
                i=0;
                //j=start;
                printf("%d\n", start);
                printf("j= %d\n", j);
                printf("i = %d\n", i);

                /*while(!win)
                {

                    if(tableau[i][j+1]==CHEMIN)                         // check chemin solution a droite
                    {
                        path++;
                        tableau[i][j+1]=path;
                        SDL_Rect rect3 = {150+j*20, 150+i*20, 20, 20};
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);                   //Affichage des murs du labyrinthes
                        SDL_SetRenderTarget(renderer, menu);
                        SDL_RenderFillRect(renderer, &rect3);
                        SDL_SetRenderTarget(renderer, NULL);
                        j++;
                        SDL_Rect rect4 = {150+j*20, 150+i*20, 20, 20};
                        SDL_SetRenderDrawColor(renderer, 119, 136, 153, 0);                   //Affichage des murs du labyrinthes
                        SDL_SetRenderTarget(renderer, menu);
                        SDL_RenderFillRect(renderer, &rect4);
                        SDL_SetRenderTarget(renderer, NULL);
                        SDL_RenderPresent(renderer);
                        printf("droite");                   //render du labyrinthes fini pour eviter les initialisations repete.
                        SDL_Delay(50);


                    }
                    else if(tableau[i+1][j]==CHEMIN)                    // check chemin solution en haut
                    {
                        path++;
                        tableau[i+1][j]=path;
                        SDL_Rect rect3 = {150+j*20, 150+i*20, 20, 20};
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);                   //Affichage des murs du labyrinthes
                        SDL_SetRenderTarget(renderer, menu);
                        SDL_RenderFillRect(renderer, &rect3);
                        SDL_SetRenderTarget(renderer, NULL);
                        i=i+1;
                        SDL_Rect rect4 = {150+j*20, 150+i*20, 20, 20};
                        SDL_SetRenderDrawColor(renderer, 119, 136, 153, 0);                   //Affichage des murs du labyrinthes
                        SDL_SetRenderTarget(renderer, menu);
                        SDL_RenderFillRect(renderer, &rect4);
                        SDL_SetRenderTarget(renderer, NULL);
                        SDL_RenderPresent(renderer);
                        printf("haute");                                  //render du labyrinthes fini pour eviter les initialisations repete.
                        SDL_Delay(50);
                    }
                    else if(tableau[i][j-1]==CHEMIN)                    //check chemin solution a gauche
                    {
                        path++;
                        tableau[i][j-1]=path;
                        SDL_Rect rect3 = {150+j*20, 150+i*20, 20, 20};
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);                   //Affichage des murs du labyrinthes
                        SDL_SetRenderTarget(renderer, menu);
                        SDL_RenderFillRect(renderer, &rect3);
                        SDL_SetRenderTarget(renderer, NULL);
                        j=j-1;
                        SDL_Rect rect4 = {150+j*20, 150+i*20, 20, 20};
                        SDL_SetRenderDrawColor(renderer, 119, 136, 153, 0);                   //Affichage des murs du labyrinthes
                        SDL_SetRenderTarget(renderer, menu);
                        SDL_RenderFillRect(renderer, &rect4);
                        SDL_SetRenderTarget(renderer, NULL);
                        SDL_RenderPresent(renderer);
                        printf("gauche");                                //render du labyrinthes fini pour eviter les initialisations repete.
                        SDL_Delay(50);

                    }
                    else if(tableau[i-1][j]==CHEMIN)                    //check chemin solution en bas
                    {
                        path++;
                        tableau[i-1][j]=path;
                        SDL_Rect rect3 = {150+j*20, 150+i*20, 20, 20};
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);                   //Affichage des murs du labyrinthes
                        SDL_SetRenderTarget(renderer, menu);
                        SDL_RenderFillRect(renderer, &rect3);
                        SDL_SetRenderTarget(renderer, NULL);
                        i=i-1;
                        SDL_Rect rect4 = {150+j*20, 150+i*20, 20, 20};
                        SDL_SetRenderDrawColor(renderer, 119, 136, 153, 0);                   //Affichage des murs du labyrinthes
                        SDL_SetRenderTarget(renderer, menu);
                        SDL_RenderFillRect(renderer, &rect4);
                        SDL_SetRenderTarget(renderer, NULL);
                        SDL_RenderPresent(renderer);
                        printf("bas");                                 //render du labyrinthes fini pour eviter les initialisations repete.
                        SDL_Delay(50);
                    }
                    else if(tableau[i][j+1]==path-1)                    // check retour en arriere a droite
                    {
                        path--;
                        tableau[i][j+1]=MUR;
                        SDL_Rect rect3 = {150+j*20, 150+i*20, 20, 20};
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);                   //Affichage des murs du labyrinthes
                        SDL_SetRenderTarget(renderer, menu);
                        SDL_RenderFillRect(renderer, &rect3);
                        SDL_SetRenderTarget(renderer, NULL);
                        j=j-1;
                        SDL_Rect rect4 = {150+j*20, 150+i*20, 20, 20};
                        SDL_SetRenderDrawColor(renderer, 119, 136, 153, 0);                   //Affichage des murs du labyrinthes
                        SDL_SetRenderTarget(renderer, menu);
                        SDL_RenderFillRect(renderer, &rect4);
                        SDL_SetRenderTarget(renderer, NULL);
                        SDL_RenderPresent(renderer);                                //render du labyrinthes fini pour eviter les initialisations repete.
                        SDL_Delay(50);

                    }
                    else if(tableau[i+1][j]==path-1)                    //check retour en arriere haut
                    {
                        path--;
                        tableau[i+1][j]=MUR;
                        SDL_Rect rect3 = {150+j*20, 150+i*20, 20, 20};
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);                   //Affichage des murs du labyrinthes
                        SDL_SetRenderTarget(renderer, menu);
                        SDL_RenderFillRect(renderer, &rect3);
                        SDL_SetRenderTarget(renderer, NULL);
                        i=i-1;
                        SDL_Rect rect4 = {150+j*20, 150+i*20, 20, 20};
                        SDL_SetRenderDrawColor(renderer, 119, 136, 153, 0);                   //Affichage des murs du labyrinthes
                        SDL_SetRenderTarget(renderer, menu);
                        SDL_RenderFillRect(renderer, &rect4);
                        SDL_SetRenderTarget(renderer, NULL);
                        SDL_RenderPresent(renderer);                                //render du labyrinthes fini pour eviter les initialisations repete.
                        SDL_Delay(50);
                    }
                    else if(tableau[i][j-1]==path-1)                //check retour en arriere gauche
                    {
                        path--;
                        tableau[i][j-1]=MUR;
                        SDL_Rect rect3 = {150+j*20, 150+i*20, 20, 20};
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);                   //Affichage des murs du labyrinthes
                        SDL_SetRenderTarget(renderer, menu);
                        SDL_RenderFillRect(renderer, &rect3);
                        SDL_SetRenderTarget(renderer, NULL);
                        j=j+1;
                        SDL_Rect rect4 = {150+j*20, 150+i*20, 20, 20};
                        SDL_SetRenderDrawColor(renderer, 119, 136, 153, 0);                   //Affichage des murs du labyrinthes
                        SDL_SetRenderTarget(renderer, menu);
                        SDL_RenderFillRect(renderer, &rect4);
                        SDL_SetRenderTarget(renderer, NULL);
                        SDL_RenderPresent(renderer);                                //render du labyrinthes fini pour eviter les initialisations repete.
                        SDL_Delay(50);

                    }
                    else if(tableau[i-1][j]==path-1)                    //check retour en arriere en bas
                    {
                        path--;
                        tableau[i-1][j]=MUR;
                        SDL_Rect rect3 = {150+j*20, 150+i*20, 20, 20};
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);                   //Affichage des murs du labyrinthes
                        SDL_SetRenderTarget(renderer, menu);
                        SDL_RenderFillRect(renderer, &rect3);
                        SDL_SetRenderTarget(renderer, NULL);
                        i=i+1;
                        SDL_Rect rect4 = {150+i*20, 150+j*20, 20, 20};
                        SDL_SetRenderDrawColor(renderer, 119, 136, 153, 0);                   //Affichage des murs du labyrinthes
                        SDL_SetRenderTarget(renderer, menu);
                        SDL_RenderFillRect(renderer, &rect4);
                        SDL_SetRenderTarget(renderer, NULL);
                        SDL_RenderPresent(renderer);                                //render du labyrinthes fini pour eviter les initialisations repete.
                        SDL_Delay(50);
                    }
                    else if (tableau[i][j]==tableau[TAILLE-1][end])         //cas de la victoire
                    {
                        win = SDL_TRUE;
                    }

                }*/



                while(!quit2)
                    {
                        SDL_WaitEvent(&event);
                        if(event.type == SDL_QUIT)
                        {                                               //quitter programme (cross)
                            quit = SDL_TRUE;
                            quit2 = SDL_TRUE;
                            principal = SDL_TRUE;

                        }
                        boutons = SDL_GetMouseState( &x, &y);
                        if(x>0 &&x<200 && y>0 && y<50 && event.button.button == SDL_BUTTON(SDL_BUTTON_LEFT))        //bouton retour
                        {
                            restart = 1;
                            quit2 = SDL_TRUE;
                        }


                    }
                quit = SDL_TRUE;


            }
            if(x>200 &&x<600 && y>250 && y<350 && event.button.button == SDL_BUTTON(SDL_BUTTON_LEFT))
                quit = SDL_TRUE;
            if(x>200 &&x<600 && y>450 && y<550 && event.button.button == SDL_BUTTON(SDL_BUTTON_LEFT))
                quit = SDL_TRUE;
            if(x>100 &&x<700 && y>600 && y<700 && event.button.button == SDL_BUTTON(SDL_BUTTON_LEFT))
            {
                quit = SDL_TRUE;
                principal = SDL_TRUE;
            }

        }
        }

    }




    printf("Fermeture SDL\n");
    SDL_Quit();
    printf("Fin du programme\n");

    return EXIT_SUCCESS;
}
