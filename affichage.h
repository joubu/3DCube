#ifndef _AFFICHAGE
#define _AFFICHAGE

#include "cube.h"
#include "calculs.h"
#include "initialisation.h"

Cube* cubeGlobal ;
Bille* billeGlobal ;

GLfloat modelViewCube[16] ;

/* Fonction qui permet de récupérer les cube et bille du main en variable globale
 *  à cause de Glut qui ne permet pas de passer des paramètres 
 * à la fonction de ré-affichage de la scène */
void MajVarGlobales(Cube* c, Bille* b) {
	cubeGlobal = c ;
	billeGlobal = b ;
}
/* On redessine le cube */
void DessinerCube() {
	glCallList(CUBE);
}
/* On redessine la bille */
void DessinerBille(Bille* b) {
	glColorMaterial(GL_FRONT, GL_AMBIENT);
	glEnable(GL_COLOR_MATERIAL);
	
	glutSolidSphere(b->rayon, 20., 20.);
	glDisable(GL_COLOR_MATERIAL);

	glBindTexture(GL_TEXTURE_2D, textures[0]);

	glCallList(BILLE);
}

/* Cette fonction affiche une petite sphère à l'endroit où il y a la collision de la balle avec une face */
void DessinerCollisions(){
	glShadeModel(GL_SMOOTH);
	int i = 0;

	for(i = 0 ; i < 10 ; i++){
			glPushMatrix();

				glColorMaterial(GL_FRONT, GL_AMBIENT);
				glEnable(GL_COLOR_MATERIAL);
				glColor4f(255,0,0,0.5);

				glTranslatef(tabCollisions[i][0], tabCollisions[i][1], tabCollisions[i][2]);

				glutSolidSphere(0.01, 20., 20.);
				glDisable(GL_COLOR_MATERIAL);

			glPopMatrix();
	}
}

/* Fonction principale de maj des coordonnées de la bille */
void MajBille( Bille* b, Cube * c) {
	
	NouvellePositionBille( b ) ;
	
	TestCollision( b , c) ;
	
	glPushMatrix();
		glTranslatef( (b->centre).x, (b->centre).y, (b->centre).z) ;
		DessinerBille(b) ;
	glPopMatrix();

}

/* Fonction principale de maj du cube (en fonction de son angle de rotation s'il y en a un */
void MajCube( Cube* cube ) {

	glPushMatrix();
		glRotated(angle,1,0,0);
		glRotated(angle,0,1,0);
		glRotated(angle,0,0,1);
		DessinerCube() ;
	glPopMatrix();

	RotationPoint(&(cube->P1), cube->OP1, &(cube->normeVitAngMax), 1);
	RotationPoint(&(cube->P2), cube->OP2, &(cube->normeVitAngMax), 0);
	RotationPoint(&(cube->P3), cube->OP3, &(cube->normeVitAngMax), 0);
	RotationPoint(&(cube->P4), cube->OP4, &(cube->normeVitAngMax), 0);
	RotationPoint(&(cube->P5), cube->OP5, &(cube->normeVitAngMax), 0);
	RotationPoint(&(cube->P6), cube->OP6, &(cube->normeVitAngMax), 0);

	
	cube->haut = VecteurNormal( cube->P4, cube->P5, cube->P6 ) ;
	cube->bas = VecteurNormal( cube->P1, cube->P2, cube->P3 ) ;
	cube->droite = VecteurNormal( cube->P4, cube->P3, cube->P5 ) ;
	cube->gauche = VecteurNormal( cube->P1, cube->P2, cube->P6 ) ;
	cube->avant = VecteurNormal( cube->P3, cube->P2, cube->P4 ) ;
	cube->arriere = VecteurNormal( cube->P6, cube->P5, cube->P1 ) ;
	
}

// Quelques variables globales pour faire bouger la caméra de façon pretty cool */
float atX = 0;float atY = 5;float atZ = -5;

/* Notre fonction d'affichage de toute la scène */
void DessineScene() {
	
	glClearColor(0, 0, 0, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/* On bouge la caméra */
	if ((atX >= 0) && (atX < 5)){
		if ((atZ >= -5) && (atZ < 0)){
			atX += 0.001;
			atZ += 0.001;
		}else if ((atZ >= 0) && (atZ <= 5)){
			atX -= 0.001;
			atZ += 0.001;
		}else if (atZ > 5){
			atZ -= 0.001;
		}else if (atZ < -5){
			atZ += 0.001;
		}
	}else if ((atX < 0) && (atX > -5)){
		if ((atZ >= -5) && (atZ < 0)){
			atX += 0.001;
			atZ -= 0.001;
		}else if ((atZ >= 0) && (atZ <= 5)){
			atX -= 0.001;
			atZ -= 0.001;
		}else if (atZ > 5){
			atZ -= 0.001;
		}else if (atZ < -5){
			atZ +- 0.001;
		}
	}else if (atX > 5){
		atX -= 0.001;
	}else if (atX < -5){
		atX += 0.001;
	}
	gluLookAt(atX, atY, atZ, 0, 0, 0, 0, 1, 0);

	
	MajCube( cubeGlobal ) ;
	MajBille( billeGlobal, cubeGlobal ) ;
	DessinerCollisions();
	
	glutSwapBuffers();

}

#endif
