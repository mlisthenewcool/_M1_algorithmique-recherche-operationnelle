#include <stdio.h>
#include <stdlib.h>
#include <glpk.h>

#include "data.h"

#define STRING_LENGTH 255

char *get_row_name(int row)
{
	static char name[STRING_LENGTH];
	sprintf(name, "y_%d", row);
	return name;
}

char *get_column_name(int column)
{
	static char name[STRING_LENGTH];
	sprintf(name, "x_%d", column);
	return name;
}

void add_all_columns(glp_prob *lp)
{
	glp_add_cols(lp, nbr_columns);

	// nommage de la colonne
	// puis ajout de la contrainte de positivité
	// puis définition du coefficient de la variable dans la fonction objectif
	for(int column_index = 1; column_index <= nbr_columns; column_index++) {
		glp_set_col_name(lp, column_index, get_column_name(column_index));
		glp_set_col_bnds(lp, column_index, GLP_LO, 0.0, 0.0);
		glp_set_obj_coef(lp, column_index, obj_coefs[column_index]);
	}
}

void add_all_rows(glp_prob *lp)
{
	glp_add_rows(lp, nbr_rows);

	// nommage de la ligne
	// puis ajout d'une borne inférieure (la demande des commandes à respecter)
	for(int row_index = 1; row_index <= nbr_rows; row_index++) {
		glp_set_row_name(lp, row_index, get_row_name(row_index));
		glp_set_row_bnds(lp, row_index, GLP_LO, row_lower_bounds[row_index], 0.0);
	}
}

void get_dual_solution(glp_prob *lp, double *affectations)
{
	for(int row_index = 1; row_index <= nbr_rows; row_index++)
		affectations[row_index] = glp_get_row_dual(lp, row_index);
}

void get_primal_solution(glp_prob *lp, double *affectations)
{
	for(int column_index = 1; column_index <= nbr_columns; column_index++)
		affectations[column_index] = glp_get_col_prim(lp, column_index);
}

int main(int argc, char **argv)
{
	glp_prob *lp;

	/* Création d'un programme linéaire de minimisation avec un titre */
	lp = glp_create_prob();
	glp_set_prob_name(lp, "Problème de découpe optimale");
	glp_set_obj_dir(lp, GLP_MIN);

	/* Ajout des lignes et colonnes au programme linéaire */
	add_all_rows(lp);
	add_all_columns(lp);

	/* Chargement de la matrice */
	glp_load_matrix(lp, nbr_coefs, row_of_coef, column_of_coef, matrix_coefs);

	/* Résolution du problème primal */
	glp_simplex(lp, NULL);

	/* Récupération du résultat dual */
	double affectations_dual[1 + nbr_rows];
	double optimum = glp_get_obj_val(lp);
	get_dual_solution(lp, affectations_dual);
	/* Affichage dual */
	printf("Best solution objective value z = %lf\n", optimum);
	for(int row_index = 1; row_index <= nbr_rows; row_index++)
		printf("%s = %lf\n", get_row_name(row_index), affectations_dual[row_index]);

	/**************************************************************************/

	/* Récupération du résultat primal */
	double affectations_prim[1 + nbr_columns];
	get_primal_solution(lp, affectations_prim);
	/* Affichage primal */
	for(int column_index = 1; column_index <= nbr_columns; column_index++)
		printf("%s = %lf\n", get_column_name(column_index), affectations_prim[column_index]);

	/* Libération du problème */
	glp_delete_prob(lp);
	glp_free_env();
	return 0;
}
