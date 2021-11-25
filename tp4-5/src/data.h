#ifndef __DATA_H
#define __DATA_H

/* Nombre de lignes (contraintes) */
const int nbr_rows = 4;

/* Nombre de colonnes (variables) */
const int nbr_columns = 4;

/* Nombre de coefficients dans la matrice (lignes*colonnes) */
const int nbr_coefs = 16;

/* Tableau des coefficients de la matrice */
double matrix_coefs[] = {0,
	2, 0, 0, 0,
	0, 2, 0, 0,
	0, 0, 3, 0,
	0, 0, 0, 7
};

/* Indice de ligne de chaque coefficient */
int row_of_coef[] = {0,
	1, 1, 1, 1,
	2, 2, 2, 2,
	3, 3, 3, 3,
	4, 4, 4, 4
};

/* Indice de colonne de chaque coefficient */
int column_of_coef[] = {0,
	1, 2, 3, 4,
	1, 2, 3, 4,
	1, 2, 3, 4,
	1, 2, 3, 4
};

/* Les bornes inférieures des lignes */
const double row_lower_bounds[] = {0, 97, 610, 395, 211};

/* Les coefficients des variables dans la fonction objectif */
const double obj_coefs[] = {0, 1, 1, 1, 1};

/* La taille des découpes */
const double roll_sizes[] = {45, 36, 31, 14};

#endif
