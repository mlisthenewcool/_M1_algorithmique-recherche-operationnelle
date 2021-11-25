#ifndef __UTIL_H
#define __UTIL_H

void parse(char *filename);
void clean();
void print_master_lp();
void print_knapsack_lp();

char *get_row_name(int row);
char *get_col_name(int row);

glp_prob *create_master_lp(char *lp_name);
void solve_problem(glp_prob *lp);
void get_dual_solution(glp_prob *lp, double *solution);
void get_primal_solution(glp_prob *lp, double *solution);

glp_prob *create_knapsack_lp(char *lp_name, double *solution);
void solve_mixed_integer_problem(glp_prob *lp);
double get_integer_solution(glp_prob *lp, double *solution);

#endif
