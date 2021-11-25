#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glpk.h>

#include "util.h"

int nbr_initials;
int nbr_finals;

double *initial_sizes;
double *final_sizes;

double *initial_demands;
double *final_demands;

int nbr_rows;
int nbr_cols;
int nbr_coefs;

double *coefs;
int *row_of_coef;
int *col_of_coef;

double *obj_coefs;

#define STRING_LENGTH 255

char *get_row_name(int row)
{
	static char name[STRING_LENGTH];
	sprintf(name, "x_%d", row);
	return name;
}

char *get_col_name(int col)
{
	static char name[STRING_LENGTH];
	sprintf(name, "y_%d", col);
	return name;
}

void add_all_columns(glp_prob *lp)
{
	glp_add_cols(lp, nbr_cols);

	// nommage de la colonne
	// puis ajout de la contrainte de positivité
	// puis définition du coefficient de la variable dans la fonction objectif
	for(int column_index = 1; column_index <= nbr_cols; column_index++) {
		glp_set_col_name(lp, column_index, get_col_name(column_index));
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
		glp_set_row_bnds(lp, row_index, GLP_LO, final_demands[row_index], 0.0);
	}
}

glp_prob *create_master_lp(char *lp_name)
{
	glp_prob *lp = glp_create_prob();
	glp_set_prob_name(lp, lp_name);
	glp_set_obj_dir(lp, GLP_MIN);

	// Ajout des lignes et colonnes
	add_all_rows(lp);
	add_all_columns(lp);

	// Chargement de la matrice
	glp_load_matrix(lp, nbr_coefs, row_of_coef, col_of_coef, coefs);

	return lp;
}

void get_primal_solution(glp_prob *lp, double *solution)
{
	double optimum = glp_get_obj_val(lp);
	printf("Best solution objective value z = %lf\n", optimum);
	
	for (int row_index = 1; row_index <= nbr_rows; row_index++) {
		solution[row_index] = glp_get_col_prim(lp, row_index);
		printf("%s = %lf\n", get_col_name(row_index), solution[row_index]);
	}
}

void get_dual_solution(glp_prob *lp, double *solution)
{
	double optimum = glp_get_obj_val(lp);
	printf("Objective function value : %lf\n\n", optimum);
	printf("Dual solution is :\n");
	
	for (int row_index = 1; row_index <= nbr_rows; row_index++) {
		solution[row_index] = glp_get_row_dual(lp, row_index);
		printf("\t%lf\n", solution[row_index]);
	}
}

void solve_problem(glp_prob *lp)
{
	glp_smcp param;
	glp_init_smcp(&param);
	param.meth = GLP_DUALP;
	param.presolve = GLP_ON;
	param.msg_lev = GLP_MSG_OFF;
	glp_simplex(lp, &param);
}

void print_knapsack_lp()
{
	printf("============================================================\n");
}

glp_prob *create_knapsack_lp(char *lp_name, double *solution)
{
	glp_prob *lp = glp_create_prob();
	glp_set_prob_name(lp, lp_name);
	glp_set_obj_dir(lp, GLP_MAX);
	
	// ajout des lignes
	glp_add_rows(lp, 1);
	glp_set_row_name(lp, 1, get_row_name(1));
	glp_set_row_bnds(lp, 1, GLP_UP, 0.0, initial_sizes[1]);
	
	// ajout des colonnes
	glp_add_cols(lp, nbr_cols);
	for(int column_index = 1; column_index <= nbr_cols; column_index++) {
		glp_set_col_name(lp, column_index, get_col_name(column_index));
		glp_set_col_kind(lp, column_index, GLP_IV);
		glp_set_col_bnds(lp, column_index, GLP_LO, 0.0, 0.0);
		glp_set_obj_coef(lp, column_index, solution[column_index]);
	}
	
	int local_nbr_coefs = nbr_finals;
	int local_row_of_coef[local_nbr_coefs];
	int local_col_of_coef[local_nbr_coefs];
	double local_coefs[local_nbr_coefs];
	
	for(int i = 1; i <= local_nbr_coefs; i++) {
		local_row_of_coef[i] = 1;
		local_col_of_coef[i] = i;
		local_coefs[i] = final_sizes[i];
	}

	// Chargement de la matrice
	glp_load_matrix(lp, local_nbr_coefs, local_row_of_coef, local_col_of_coef, local_coefs);
	
	return lp;
}

double get_integer_solution(glp_prob *lp, double *solution)
{
	double optimum = glp_mip_obj_val(lp);
	printf("Knapsack solution = %lf\n", optimum);
	
	for (int col_index = 1; col_index <= nbr_cols; col_index++) {
		solution[col_index] = glp_mip_col_val(lp, col_index);
		printf("\t%lf\n", solution[col_index]);
	}
	
	return optimum;
}

void solve_mixed_integer_problem(glp_prob *lp)
{
	glp_iocp param;
	glp_init_iocp(&param);
	param.presolve = GLP_ON;
	param.msg_lev = GLP_MSG_OFF;
	glp_intopt(lp, &param);
}

void clean()
{
	free(initial_sizes);
	free(final_sizes);
	free(initial_demands);
	free(final_demands);

	free(coefs);
	free(row_of_coef);
	free(col_of_coef);
	free(obj_coefs);
}

void print_master_lp()
{
	printf("============================================================\n");
	int k = 1;
	for(int row_index = 1; row_index <= nbr_rows; row_index++) {
		printf("\t");
		for(int col_index = 1; col_index <= nbr_cols; col_index++) {
			printf("%lf ", coefs[k]);
			k++;
		}
		printf("\n");
	}
	printf("\n");
}

void parse(char *filename)
{
	FILE *fp;

	fp = fopen(filename, "r");
	if(fp == NULL) {
		fprintf(stderr, "Can't open file : %s\n", filename);
		exit(EXIT_FAILURE);
	}

	char buffer[STRING_LENGTH];
	
	// find keyword "initials"
	fscanf(fp, "%s", buffer);
	if(strcmp(buffer, "initials") != 0) {
		fprintf(stderr, "Incorrect format : expected initials\n");
		exit(EXIT_FAILURE);
	}

	fscanf(fp, "%d", &nbr_initials);
	initial_sizes = malloc(sizeof(double) * (nbr_initials + 1));
	initial_demands = malloc(sizeof(double) * (nbr_initials + 1));

	for(int i = 1; i <= nbr_initials; i++)
		fscanf(fp, "%lf %lf", &initial_sizes[i], &initial_demands[i]);

	// find keyword "finals"
	fscanf(fp, "%s", buffer);
	if(strcmp(buffer, "finals") != 0) {
		fprintf(stderr, "Incorrect format : expected finals\n");
		exit(EXIT_FAILURE);
	}

	fscanf(fp, "%d", &nbr_finals);
	final_sizes = malloc(sizeof(double) * (nbr_finals + 1));
	final_demands = malloc(sizeof(double) * (nbr_finals + 1));

	for(int i = 1; i <= nbr_finals; i++)
		fscanf(fp, "%lf %lf", &final_sizes[i], &final_demands[i]);

	fclose(fp);
}
