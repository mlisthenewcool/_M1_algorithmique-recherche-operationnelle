#include <stdio.h>
#include <stdlib.h>
#include <string.h>	// strcmp
#include <time.h>	// clock
#include <math.h>	// pow

#include "extern.h"
#include "util.h"
#include "algo.h"

unsigned char DEBUG_MODE = 0;

void display_help_message(char *program_name)
{
	fprintf(stderr, "Usage : %s input_file [options]\n\n", program_name);
	fprintf(stderr, "\t-v debug mode\n");
	fprintf(stderr, "\t-h display that message\n\n");

	fprintf(stderr, "\tThe input_file's format expected is :\n");
	fprintf(stderr, "\t\tBag_Capacity Objects_Count\n\t\tValue_#1 Weight_#1\n\t\tValue_#2 Weight_#2\n\t\t...\n");
}

int main(int argc, char **argv)
{
	if(argc < 2)
	{
		display_help_message(argv[0]);
		exit(EXIT_FAILURE);
	}

	/* Try to parse the given file */
	Object *objects = parse(argv[1]);

	for(int i = 2; i < argc; i++)
	{
		if( ! strcmp(argv[i], "-v"))
		{
			DEBUG_MODE = 1;
		}
		else if( ! strcmp(argv[i], "-h"))
		{
			display_help_message(argv[0]);
			exit(EXIT_SUCCESS);
		}
		else
		{
			fprintf(stderr, "Unknown option\n\n");
			display_help_message(argv[0]);
			exit(EXIT_SUCCESS);
		}
	}

	printf("----------------------------------------------------------------------\n");
	printf("Number of objects : %50ld\n", OBJECTS_COUNT);
	printf("Capacity of the bag : %48ld\n", BAG_CAPACITY);
	printf("Theorical number of nodes to explore (2 ^%4ld) : %21lg\n", OBJECTS_COUNT, pow(2, OBJECTS_COUNT));
	printf("----------------------------------------------------------------------\n");
	printf("\nRunning...\n\n");

	clock_t start, end;
	start = clock();
	unsigned long max_profit = knapsack(objects);
	end = clock();

	printf("----------------------------------------------------------------------\n");
	printf("Maximum profit found : %47ld\n", max_profit);
	printf("Nodes visited : %54ld\n", NODES_VISITED);
	printf("Nodes visited in upper bound function : %30ld\n", NODES_IN_BOUND_VISITED);
	printf("Execution time (seconds) : %43lf\n", (double)(end - start) / CLOCKS_PER_SEC);
	printf("----------------------------------------------------------------------\n");

	free(objects);
	return EXIT_SUCCESS;
}
