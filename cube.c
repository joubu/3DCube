#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define RALENTIT 250
#define PERSPECTIVE 45

#include "cube.h"
#include "initialisation.h"
#include "calculs.h"
#include "affichage.h"

/* Fonction de redimensionnement de la fenêtre */
void Reshape(int w, int h){
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(PERSPECTIVE, (float) w/h, 1.0, 10.0);
}

/* Fonction idle, on incrémente l'angle du cube et on redessinne la scène */
void Idle() {
	angle = angle + vitesse;
	if (angle > 360) angle=0;
	glutPostRedisplay();
}

/* Gestion des évènements clavier */
void Event( unsigned char key, int x, int y ) {
	switch (key) {
		case 27: // 'ESC'
			exit(0); 
			break;
	
		case 42: // '*'
			vitesse = vitesse - 1;
			break;
	
		case 43: // '+'
			if (vitesse < 360) vitesse += 0.01;
			break;
	
		case 45: // '-'
			if (vitesse > 0) vitesse -= 0.01;
			break;
	
		case 127: // 'DEL'
			vitesse = 1 ;
			Reshape( 640,480 ) ;
			break;
	}
}

int main( int argc, char** argv ) {
	
	glutInit(&argc, argv);
	Cube cube;
	Bille bille;

	/* Création de la fenêtre */
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Cube 3d");
	
	Init(&cube, &bille);
	
	glutDisplayFunc( DessineScene );
	glutReshapeFunc( Reshape ) ;
	glutIdleFunc( Idle ) ;
	glutKeyboardFunc( Event ) ;
	glutMainLoop() ;
	
	return 0 ;
}
