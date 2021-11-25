#include <stdio.h>
#include <stdlib.h>

#include "util.h"
#include "extern.h"

unsigned long OBJECTS_COUNT = 0;
unsigned long BAG_CAPACITY = 0;

/**
 * Returns the
 */
Object *parse(char *filename)
{
	FILE *fp;

	/* Open @filename */
	fp = fopen(filename, "r");
	if(fp == NULL)
	{
		fprintf(stderr, "Can't open file : %s\n", filename);
		exit(EXIT_FAILURE);
	}

	/* Save the bag capacity and objects size */
	fscanf(fp, "%ld %ld\n", &BAG_CAPACITY, &OBJECTS_COUNT);

	/* Allocate objects structure */
	Object *objects = malloc(sizeof(Object) * OBJECTS_COUNT);
	if(objects == NULL)
	{
		fprintf(stderr, "Can't malloc the objects structure\n");
		exit(EXIT_FAILURE);
	}

	/* Save weight and value for each object */
	for(int i = 0; i < OBJECTS_COUNT; i++)
	{
		fscanf(fp, "%f %f\n", &objects[i].weight, &objects[i].value);
		objects[i].id = i;
	}

	fclose(fp);
	return objects;
}

void display_objects(Object *objects)
{
	if(DEBUG_MODE)
	{
		printf("%15s | %15s | %15s |\n\n", "ID", "WEIGHT", "VALUE");
		for(int i = 0; i < OBJECTS_COUNT; i++)
		{
			printf("%15d | %15.1f | %15.1f |\n", objects[i].id, objects[i].weight, objects[i].value);
		}
	}
}

void display_object(Object obj)
{
	if(DEBUG_MODE)
	{
		printf("OBJECT (%5d, %5.1f, %5.1f)\n", obj.id, obj.weight, obj.value);
	}
}