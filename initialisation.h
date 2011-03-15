#ifndef _INIT
#define _INIT

int textures[1];

#include "cube.h"
#include "calculs.h"
#include "affichage.h"

#define ALPHACUBE 0.1

/* Fonction pour charger une image en mémoire pour pouvoir l'appliquer à notre bille */
int ImageLoad(char *filename, Image *image) {
    FILE *file;
    unsigned long size;
    unsigned long i;
    unsigned short int planes;
    unsigned short int bpp;
    char temp;

    if ((file = fopen(filename, "rb"))==NULL)
    {
        printf("File Not Found : %s\n",filename);
        return 0;
    }
    
    fseek(file, 18, SEEK_CUR);

    if ((i = fread(&image->sizeX, 4, 1, file)) != 1) {
        printf("Error reading width from %s.\n", filename);
        return 0;
    }
    
    if ((i = fread(&image->sizeY, 4, 1, file)) != 1) {
        printf("Error reading height from %s.\n", filename);
        return 0;
    }
    
    if ((fread(&planes, 2, 1, file)) != 1) {
        printf("Error reading planes from %s.\n", filename);
        return 0;
    }
    if (planes != 1) {
        printf("Planes from %s is not 1: %u\n", filename, planes);
        return 0;
    }

    if ((i = fread(&bpp, 2, 1, file)) != 1) {
        printf("Error reading bpp from %s.\n", filename);
        return 0;
    }
    if ((bpp != 24) && (bpp!=32)) {
        printf("Bpp from %s is not 24 or 32: %u\n", filename, bpp);
        return 0;
    }
    
    size = image->sizeX * image->sizeY * (bpp/8);

    fseek(file, 24, SEEK_CUR);

    image->data = (char *) malloc(size);
    if (image->data == NULL) {
        printf("Error allocating memory for color-corrected image data");
        return 0;       
    }

    if ((i = fread(image->data, size, 1, file)) != 1) {
        printf("Error reading image data from %s.\n", filename);
        return 0;
    }

    for (i=0;i<size;i+=(bpp/8)) {
        temp = image->data[i];
        image->data[i] = image->data[i+2];
        image->data[i+2] = temp;
    }

    return bpp;
}

/* Cette fois on charge la texture dans le moteur OpenGL */
void LoadTexture(char* filename, int* ident)
 {
  Image im;
  int bpp;

  if(bpp=ImageLoad(filename, &im))
   {
     glGenTextures(1, (GLuint*)ident);
     glBindTexture(GL_TEXTURE_2D, *ident);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     if(bpp==24)
       glTexImage2D(GL_TEXTURE_2D,0,3,
		    im.sizeX,im.sizeY,
		    0,GL_RGB,GL_UNSIGNED_BYTE,im.data);
     else if(bpp==32)
       glTexImage2D(GL_TEXTURE_2D,0,4,
		    im.sizeX,im.sizeY,
		    0,GL_RGBA,GL_UNSIGNED_BYTE,im.data);
   }
}

/* Initialisation du cube */
void InitCube( Cube* cube ) {
	
	/* Définition des sommets du cube */
	(cube->P1).x = -1 ; 		(cube->P1).y = -1 ; 		(cube->P1).z = 1 ;
	(cube->P2).x = -1 ; 		(cube->P2).y = -1 ; 		(cube->P2).z = -1 ;
	(cube->P3).x = 1 ; 			(cube->P3).y = -1 ; 		(cube->P3).z = -1 ;
	(cube->P4).x = 1 ; 			(cube->P4).y = 1 ; 			(cube->P4).z = -1 ;
	(cube->P5).x = 1 ; 			(cube->P5).y = 1 ; 			(cube->P5).z = 1 ;
	(cube->P6).x = -1 ; 		(cube->P6).y = 1 ; 			(cube->P6).z = 1 ;
	
	/* On garde une trace des coordonnées originales du cube
	 * afin de recalculer la nouvelle position des sommets 
	 * sans le faire de façon incrémentale, et ce pour limiter
	 *  la propagation d'erreurs de calculs sur les flottants */
	(cube->OP1).x = -1 ; 		(cube->OP1).y = -1 ; 		(cube->OP1).z = 1 ;
	(cube->OP2).x = -1 ; 		(cube->OP2).y = -1 ; 		(cube->OP2).z = -1 ;
	(cube->OP3).x = 1 ; 		(cube->OP3).y = -1 ; 		(cube->OP3).z = -1 ;
	(cube->OP4).x = 1 ; 		(cube->OP4).y = 1 ; 		(cube->OP4).z = -1 ;
	(cube->OP5).x = 1 ; 		(cube->OP5).y = 1 ; 		(cube->OP5).z = 1 ;
	(cube->OP6).x = -1 ; 		(cube->OP6).y = 1 ; 		(cube->OP6).z = 1 ;

	/* Définition des normales */
	cube->haut = VecteurNormal( cube->P4, cube->P5, cube->P6 ) ;
	cube->bas = VecteurNormal( cube->P1, cube->P2, cube->P3 ) ;
	cube->droite = VecteurNormal( cube->P4, cube->P3, cube->P5 ) ;
	cube->gauche = VecteurNormal( cube->P1, cube->P2, cube->P6 ) ;
	cube->avant = VecteurNormal( cube->P3, cube->P2, cube->P4 ) ;
	cube->arriere = VecteurNormal( cube->P6, cube->P5, cube->P1 ) ;

	/* On garde une trace du côté du carré pour pouvoir réliser les pré-tests
	 * pour déterminer si la bille est dans la sphere inscrite au carré */	
	cube->cote = 2.0f ;
	
	/* Cette valeur représente la vitesse maximum qu'un point du cube peut atteindre
	 * ce dans le but de gérer le dépassement de la bille au temps t + 1 si le cube la rattrappe */
	cube->normeVitAngMax = 0.0f;
	
	glNewList( CUBE, GL_COMPILE ) ;
	
		glBegin( GL_QUADS ) ;
			/* FACE DROITE */
			glNormal3f( 1, 0, 0 ) ;
			glColor4d(0, 255, 255, ALPHACUBE);
			glVertex3i(1, 1, 1) ;
			glVertex3i(1, -1, 1) ;
			glVertex3i(1, -1, -1) ;
			glVertex3i(1, 1, -1) ;
		glEnd() ;
		
		glBegin( GL_QUADS ) ;	
			/* FACE GAUCHE */
			glNormal3f(-1, 0, 0);
			glColor4d(0, 255, 0, ALPHACUBE);
			glVertex3i(-1, 1, 1) ;
			glVertex3i(-1, -1, 1) ;
			glVertex3i(-1, -1, -1) ;
			glVertex3i(-1, 1, -1) ;
		glEnd() ;
		
		glBegin( GL_QUADS ) ;
			/* FACE HAUT */
			glNormal3f(0, 1, 0);
			glColor4d(255, 0, 0, ALPHACUBE);
			glVertex3i(-1, 1, 1);
			glVertex3i(1, 1, 1);
			glVertex3i(1, 1, -1);
			glVertex3i(-1, 1, -1);
		glEnd() ;
		
		glBegin( GL_QUADS ) ;
			/* FACE BAS */
			glNormal3f(0, -1, 0);
			glColor4d(0, 0, 255, ALPHACUBE);
			glVertex3i(-1, -1, 1);
			glVertex3i(1, -1, 1);
			glVertex3i(1, -1, -1);
			glVertex3i(-1, -1, -1);
		glEnd() ;
		
		glBegin( GL_QUADS ) ;
			/* FACE ARRIERE */
			glNormal3f(0, 0, 1);
			glColor4d(255, 255, 255, ALPHACUBE);
			glVertex3i(-1, 1, 1);
			glVertex3i(1, 1, 1);
			glVertex3i(1, -1, 1);
			glVertex3i(-1, -1, 1);
		glEnd() ;

		glBegin( GL_QUADS ) ;
			/* FACE AVANT */
			glNormal3f(0, 0, -1);
			glColor4d(255, 255, 0, ALPHACUBE);
			glVertex3i(-1, 1, -1);
			glVertex3i(1, 1, -1);
			glVertex3i(1, -1, -1);
			glVertex3i(-1, -1, -1);
		glEnd();
	glEndList();
	
	glGetFloatv( GL_MODELVIEW_MATRIX, modelViewCube );
	
}

/* Initialisation de la bille */
void InitBille( Bille* b ) {
	GLfloat ambient[4] = {0.1, 0.1, 0.1, 1.0};
	GLfloat diffuse[4] = {0.8, 0.8, 0.8, 1.0};
	GLfloat Black[4] = {0.0, 0.0, 0.0, 1.0};
	
	glNewList(BILLE, GL_COMPILE);
		glShadeModel(GL_SMOOTH);
		glColorMaterial(GL_FRONT, GL_AMBIENT);
		glEnable(GL_COLOR_MATERIAL);
	
		glColor4f(0,0,255,0.5);
		
		glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
		glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, Black);
		glMaterialfv(GL_FRONT, GL_EMISSION, ambient);
		glMaterialf(GL_FRONT,GL_SHININESS,0.0);
		
		GLUquadricObj* sph1 = gluNewQuadric();
	  	gluQuadricDrawStyle(sph1,GLU_FILL);
 	  	gluQuadricNormals(sph1,GLU_SMOOTH);
  		gluQuadricTexture(sph1,GL_TRUE);
  		gluSphere(sph1,b->rayon,30,30);
  		
		glDisable(GL_COLOR_MATERIAL);
	glEndList();
}

/** Initialisation de toute la scène avant de lancer la simulation */
void Init( Cube* c, Bille* b ) {
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	
	InitCube( c );
	b->rayon = 0.1;
	(b->centre).x = 0.0;
	(b->centre).y = 0.0;
	(b->centre).z = 0.0;
	(b->directeur).x = 0.1;
	(b->directeur).y = 0.2;
	(b->directeur).z = 0.05;
	NormaliseVecteur( &(b->directeur) ) ;
	
	InitBille( b ) ;
	LoadTexture("earth.bmp", &textures[0]);
	
	MajVarGlobales(c, b);
}

#endif
