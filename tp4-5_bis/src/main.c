#include <stdio.h>
#include <stdlib.h>
#include <glpk.h>

#include "util.h"
#include "extern.h"

void initial_matrix()
{
	nbr_rows = nbr_finals;
	nbr_cols = nbr_finals;
	nbr_coefs = nbr_rows * nbr_cols;
	
	coefs = malloc(sizeof(double) * (nbr_coefs + 1));
	row_of_coef = malloc(sizeof(int) * (nbr_coefs + 1));
	col_of_coef = malloc(sizeof(int) * (nbr_coefs + 1));
	
	obj_coefs = malloc(sizeof(double) * (nbr_cols + 1));
	
	int k = 1;
	for(int row_index = 1; row_index <= nbr_rows; row_index++) {
		obj_coefs[row_index] = 1;
		for(int col_index = 1; col_index <= nbr_cols; col_index++) {
			row_of_coef[k] = row_index;
			col_of_coef[k] = col_index;
			
			if(row_index == col_index)
				coefs[k] = (int) (initial_sizes[1] / final_sizes[row_index]);
			else
				coefs[k] = 0;
			
			k++;
		}
	}
}

void add_column_lp(double *knapsack_solution)
{
	nbr_cols++;
	nbr_coefs = nbr_rows * nbr_cols;
	
	coefs = realloc(coefs, sizeof(double) * (nbr_coefs + 1));
	row_of_coef = realloc(row_of_coef, sizeof(int) * (nbr_coefs + 1));
	col_of_coef = realloc(col_of_coef, sizeof(int) * (nbr_coefs + 1));
	
	obj_coefs = malloc(sizeof(double) * (nbr_cols + 1));
	
	int k = 1;
	double local_coefs[nbr_coefs];
	for(int row_index = 1; row_index <= nbr_rows; row_index++) {
		for(int col_index = 1; col_index <= nbr_cols; col_index++) {
			row_of_coef[k] = row_index;
			col_of_coef[k] = col_index;
			local_coefs[k] = coefs[k];
			k++;
		}
	}
	
	k = 1;
	int offset = 0;
	for(int row_index = 1; row_index <= nbr_rows; row_index++) {
		for(int col_index = 1; col_index <= nbr_cols; col_index++) {
			row_of_coef[k] = row_index;
			col_of_coef[k] = col_index;
			// on recopie
			if(col_index != nbr_cols) {
				coefs[k] = local_coefs[k-offset];
			}
			// on insère la nouvelle colonne
			else {
				coefs[k] = knapsack_solution[row_index];
				offset++;
			}
			k++;
		}
	}
	
	for(int col = 1; col <= nbr_cols; col++)
		obj_coefs[col] = 1;
}

int main(int argc, char **argv)
{
	if(argc < 2)
	{
		fprintf(stderr, "Usage : %s input_file\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	// lecture fichier
	parse(argv[1]);

	printf("Initials : \n");
	for(int i = 1; i <= nbr_initials; i++)
		printf("\t%lf %lf\n", initial_sizes[i], initial_demands[i]);
	printf("Finals : \n");
	for(int i = 1; i <= nbr_finals; i++)
		printf("\t%lf %lf\n", final_sizes[i], final_demands[i]);

	printf("\nRunning...\n");

	// on démarre d'une matrice carrée
	initial_matrix();
	
	glp_prob *master_lp;
	glp_prob *knapsack_lp;
	double knapsack_sol = 0.0;
	
	while(1)
	{
		double solution[1+nbr_cols];
		// Récupération du résultat dual
		master_lp = create_master_lp("Cutting stock problem");
		print_master_lp();
		solve_problem(master_lp);
		get_dual_solution(master_lp, solution);

		// résolution sac à dos
		knapsack_lp = create_knapsack_lp("Knapsack problem", solution);
		print_knapsack_lp();
		solve_mixed_integer_problem(knapsack_lp);
		knapsack_sol = get_integer_solution(knapsack_lp, solution);

		// test optimalité
		if(knapsack_sol < 1.0) {
			printf("============================================================\n");
			printf("Optimal solution found : \n");
/*
			int k = 1;
			for(int row_index = 1; row_index <= nbr_rows; row_index++) {
				printf("\t");
				for(int col_index = 1; col_index <= nbr_cols; col_index++) {
					printf("%lf  ", coefs[k]);
					k++;
				}
				printf("\n");
			}
*/

			printf("\t");
			for(int col_index = 1; col_index <= nbr_cols; col_index++) {
				printf("%.5lf ", glp_get_col_prim(master_lp, col_index));
			}
			printf("\n");

			glp_delete_prob(master_lp);
			glp_delete_prob(knapsack_lp);
			break;
		}
		else {
			add_column_lp(solution);
		}

		glp_delete_prob(master_lp);
		glp_delete_prob(knapsack_lp);
	}
	
	clean();
	glp_free_env();
	return EXIT_SUCCESS;
}
