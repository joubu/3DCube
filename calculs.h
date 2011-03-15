#ifndef _CALCULS
#define _CALCULS

#include "cube.h"

/* Retourne le produit scalaire entre deux vecteurs */
float ProduitScalaire( Vecteur v, float x, float y, float z ){
	return (v.x * x + v.y * y + v.z * z);
}

/* Convertit des degrés en radian */
float Radian(float alpha){
	return ((alpha * M_PI) / 180);
}

/* Normalise un vecteur dans le but d'avoir une vitesse constante pour la balle */
void NormaliseVecteur( Vecteur* N ){
	
	float norme = sqrt( pow( N->x, 2 ) + pow( N->y, 2 ) + pow( N->z, 2 )) ;
	
	N->x = N->x / norme ;
	N->y = N->y / norme ;
	N->z = N->z / norme ;
}

/* Renvoie le vecteur normale à un plan */
Vecteur VecteurNormal( Point P1, Point P2, Point P3 ) {

	Vecteur N ;
	
	N.x = ( P3.y - P1.y ) * ( P2.z - P1.z ) - ( P3.z - P1.z ) * ( P2.y - P1.y ) ;
	N.y = ( P3.z - P1.z ) * ( P2.x - P1.x ) - ( P3.x - P1.x ) * ( P2.z - P1.z ) ;
	N.z = ( P3.x - P1.x ) * ( P2.y - P1.y ) - ( P3.y - P1.y ) * ( P2.x - P1.x ) ;
	
	NormaliseVecteur(&N);
	
	return N ;	
	
}

/* Applique la même rotations aux points que l'on a appliquée au cube */
/* Le but ici est de conserver une trace en temps réel des coordonnées des points du cube
 * pour pouvoir tester la collision de la balle avec les faces */
void RotationPoint( Point* A, Point O, float* normeVitAngMax, char majVAM ) {

	Point B ;
	B.x = O.x*cosf(Radian(angle))*cosf(Radian(angle)) + O.y*(cosf(Radian(angle))*sinf(Radian(angle))*sinf(Radian(angle)) -cosf(Radian(angle))*sin(Radian(angle))) + O.z*(cosf(Radian(angle))*cosf(Radian(angle))*sinf(Radian(angle)) + sinf(Radian(angle))*sinf(Radian(angle))) ;
	B.y = O.x*cosf(Radian(angle))*sinf(Radian(angle)) + O.y*(cosf(Radian(angle))*cosf(Radian(angle)) + sinf(Radian(angle))*sinf(Radian(angle))*sinf(Radian(angle))) + O.z*(-cos(Radian(angle))*sinf(Radian(angle)) + cosf(Radian(angle))*sinf(Radian(angle))*sinf(Radian(angle))) ;
	B.z = O.x*-sinf(Radian(angle)) + O.y*cosf(Radian(angle))*sinf(Radian(angle)) + O.z*cosf(Radian(angle))*cosf(Radian(angle)) ;
	
	if (majVAM){
		*normeVitAngMax = sqrt( pow( (B.x - A->x), 2 ) + pow( (B.y - A->y), 2 ) + pow( (B.z - A->z), 2 ));
	}
	
	A->x = B.x;
	A->y = B.y;
	A->z = B.z;
}

/* Retourne l'équation du plan définit par 3 points */
EquationPlan Equation( Point P1, Point P2, Point P3 ) {
	
	EquationPlan EP ;
	EP.A = P2.y*P3.z - P2.y*P1.z - P1.y*P3.z - P3.y*P2.z + P3.y*P1.z + P1.y*P2.z ;
	EP.B = -P3.x*P1.z + P2.x*P1.z + P1.x*P3.z + P3.x*P2.z - P1.x*P2.z - P2.x*P3.z ;
	EP.C = P2.x*P3.y - P2.x*P1.y - P1.x*P3.y - P3.x*P2.y + P1.x*P2.y + P3.x*P1.y ;
	EP.D = -P1.x*P2.y*P3.z + P1.x*P3.y*P2.z + P2.x*P1.y*P3.z - P3.x*P1.y*P2.z - P2.x*P3.y*P1.z + P3.x*P2.y*P1.z ;
	
	return EP ;
}

/* Retourne la distance entre un point et un plan */
float DistancePointPlan(Point P, Point P1, Point P2, Point P3){

	EquationPlan EP = Equation( P1, P2, P3 ) ;
	return ( fabs( EP.A * P.x + EP.B * P.y + EP.C * P.z + EP.D ) / sqrt( pow( EP.A, 2 ) + pow( EP.B, 2 ) + pow( EP.C, 2 ))) ;
}

/* Incrémente la position de la bille en fonction de son vecteur directeur */
void NouvellePositionBille( Bille* b ) {
	if ((b->centre.x < 1.5) && (b->centre.y < 1.5) && (b->centre.z < 1.5) && (b->centre.x > -1.5) && (b->centre.y > -1.5) && (b->centre.z > -1.5)){
		(b->centre).x += (b->directeur).x / RALENTIT ;
		(b->centre).y += (b->directeur).y / RALENTIT ;
		(b->centre).z += (b->directeur).z / RALENTIT ;
	}else{ 
		/* SI JAMAIS la balle sortait du cube ( ce qui n'arrive jamais ;) on la remet au centre */
		b->centre.x = 0.0;
		b->centre.y = 0.0;
		b->centre.z = 0.0;
	}
}

/* La balle a touchée une face dont la normale est n, on calcule le rayon réfléchit pour le vecteur directeur */
void Rebond( Bille* b, Vecteur n ) {

	float NormaleXIncident = ProduitScalaire( n, b->directeur.x, b->directeur.y, b->directeur.z) ;

	(b->directeur).x = (b->directeur).x - 2 * n.x * NormaleXIncident ;
	(b->directeur).y = (b->directeur).y - 2 * n.y * NormaleXIncident ;
	(b->directeur).z = (b->directeur).z - 2 * n.z * NormaleXIncident ;

	NormaliseVecteur( &(b->directeur) ) ;	
}

/* Renvoie la norme d'un vecteur */
float Norme(Vecteur v){
	return ( sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2)));
}

/* Test préliminaire pour vérifier que la bille est dans la sphere inscrite dans le cube
 * ce afin d'éviter de faire des tests de collision inutils */
char EstDansSphere( Bille* b, Cube* c ) {

	if (sqrt( pow( b->centre.x, 2 ) + pow( b->centre.y, 2 ) + pow( b->centre.z, 2 )) < (c->cote/2 - b->rayon - Norme(b->directeur) / RALENTIT)){
		return 1;
	}else{
		return 0;
	}
}

/* Test de la collision de la bille avec une des faces du cube */
int TestCollision( Bille* b, Cube* cube ) {
	float d = 0;

	if (!EstDansSphere( b, cube )) {
		if ( (d=DistancePointPlan( b->centre, cube->P1, cube->P2, cube->P3 )) < (b->rayon + Norme(b->directeur) / RALENTIT + cube->normeVitAngMax)){
			Rebond( b, cube->bas ) ;
			tabCollisions[colCourante][0] = b->centre.x;
			tabCollisions[colCourante][1] = b->centre.y - b->rayon;
			tabCollisions[colCourante][2] = b->centre.z;

			colCourante++;nbCollisions++;
			if (colCourante == 10) colCourante = 0;
		}
		else if ( (d=DistancePointPlan(b->centre, cube->P4, cube->P5, cube->P6)) < (b->rayon + Norme(b->directeur) / RALENTIT + cube->normeVitAngMax)){
			Rebond( b, cube->haut ) ;
			tabCollisions[colCourante][0] = b->centre.x;
			tabCollisions[colCourante][1] = b->centre.y + b->rayon;
			tabCollisions[colCourante][2] = b->centre.z;

			colCourante++;nbCollisions++;
			if (colCourante == 10) colCourante = 0;
		}
		else if ( (d=DistancePointPlan(b->centre, cube->P2, cube->P1, cube->P6)) < (b->rayon + Norme(b->directeur) / RALENTIT + cube->normeVitAngMax)){
			Rebond( b, cube->gauche ) ;
			tabCollisions[colCourante][0] = b->centre.x - b->rayon;
			tabCollisions[colCourante][1] = b->centre.y;
			tabCollisions[colCourante][2] = b->centre.z;

			colCourante++;nbCollisions++;
			if (colCourante == 10) colCourante = 0;
		}
		else if ( (d=DistancePointPlan(b->centre, cube->P5, cube->P4, cube->P3)) < (b->rayon + Norme(b->directeur)/ RALENTIT + cube->normeVitAngMax)){
			Rebond( b, cube->droite ) ;
			tabCollisions[colCourante][0] = b->centre.x + b->rayon;
			tabCollisions[colCourante][1] = b->centre.y;
			tabCollisions[colCourante][2] = b->centre.z;

			colCourante++;nbCollisions++;
			if (colCourante == 10) colCourante = 0;
		}
		else if ( (d=DistancePointPlan(b->centre, cube->P4, cube->P3, cube->P2)) < (b->rayon + Norme(b->directeur)/ RALENTIT + cube->normeVitAngMax)){
			Rebond( b, cube->avant ) ;
			tabCollisions[colCourante][0] = b->centre.x;
			tabCollisions[colCourante][1] = b->centre.y;
			tabCollisions[colCourante][2] = b->centre.z - b->rayon;

			colCourante++;nbCollisions++;
			if (colCourante == 10) colCourante = 0;
		}
		else if ( (d=DistancePointPlan(b->centre, cube->P1, cube->P6, cube->P5)) < (b->rayon + Norme(b->directeur)/ RALENTIT + cube->normeVitAngMax)){
			Rebond( b, cube->arriere ) ;
			tabCollisions[colCourante][0] = b->centre.x;
			tabCollisions[colCourante][1] = b->centre.y;
			tabCollisions[colCourante][2] = b->centre.z + b->rayon;

			colCourante++;nbCollisions++;
			if (colCourante == 10) colCourante = 0;
		}
	}
}

#endif
