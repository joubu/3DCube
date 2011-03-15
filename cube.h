#ifndef _CUBE
#define _CUBE
/* Définition des structures utiles à la simulation */

float angle = 0.0;
float angle2=0.0;
float vitesse = 0.0;

float tabCollisions[10][3];
int colCourante = 0;
int nbCollisions = 0;

typedef enum {
	RIEN ,
	BILLE ,
	CUBE
} ID_OBJECT ;

typedef struct {
	float x ; 
	float y ; 
	float z ;
} Vecteur ; 

typedef struct { 
	float x ; 
	float y ; 
	float z ;
} Point ;


typedef struct {
	// On garde les 6 vecteurs normaux aux faces du cube
	Vecteur haut ;
	Vecteur bas ;
	Vecteur droite ;
	Vecteur gauche ;
	Vecteur avant ;
	Vecteur arriere ;
	// ainsi que 6 points pour calculer la distance bille/face
	Point P1 ;
	Point P2 ;
	Point P3 ;
	Point P4 ;
	Point P5 ;
	Point P6 ;
	// Position d'origine du cube
	Point OP1 ;
	Point OP2 ;
	Point OP3 ;
	Point OP4 ;
	Point OP5 ;
	Point OP6 ;
	// Côté
	float cote ;
	float normeVitAngMax;
} Cube ;

typedef struct {
	Point centre ;
	Vecteur directeur ;
	float rayon ;
} Bille ;

typedef struct {
	float A ;
	float B ;
	float C ;
	float D ;
} EquationPlan ;

typedef struct {
    unsigned long sizeX;
    unsigned long sizeY;
    char *data;
} Image;



#endif
