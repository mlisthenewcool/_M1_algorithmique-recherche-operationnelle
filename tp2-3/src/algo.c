#include <stdio.h>
#include <stdlib.h>

#include "extern.h"
#include "util.h"
#include "queue.h"

unsigned long NODES_VISITED = 0;
unsigned long NODES_IN_BOUND_VISITED = 0;

/**
 * Decreasing sort by value/weight ratio
 */
int compare_by_ratio(void const *a, void const *b)
{
	const Object *object_a = a;
	const Object *object_b = b;

	return object_b->value / object_b->weight > object_a->value / object_a->weight;
}

/**
 * That function will return the upper bound of the given node @n
 */
float upper_bound(Node n, Object *objects)
{
	/* If the bag capacity is exceeded, there is no profit at all */
	if(n.bag_weight >= BAG_CAPACITY)
	{
		return 0;
	}

	/* We start to include objects right after the given priority_scorenode @n */
	long local_deepth = n.deepth + 1;
	/* Profit and weight are copied from the given node @n */
	float local_profit = n.bag_profit;
	long local_weight = n.bag_weight;

	/* Adding objects till the bag capacity is exceeded or there is no more objects */
	while((local_weight + objects[local_deepth].weight <= BAG_CAPACITY) && (local_deepth < OBJECTS_COUNT))
	{
		NODES_IN_BOUND_VISITED++;
		local_weight += objects[local_deepth].weight;
		local_profit += objects[local_deepth].value;
		local_deepth++;
	}

	/* We cannot add a full object, take a fraction of the next one to fill the bag */
	if(local_deepth < OBJECTS_COUNT)
	{
		local_profit += (BAG_CAPACITY - local_weight) * objects[local_deepth].value / objects[local_deepth].weight;
	}

	return local_profit;
}

unsigned long knapsack(Object *objects)
{
	printf("\t(1) Sorting objects by value/weight ratio...\n\n");

	/* Sort objects by value/weight ratio */
	qsort(objects, OBJECTS_COUNT, sizeof(Object), compare_by_ratio);

	if(DEBUG_MODE)
	{
		display_objects(objects);
		printf("\n");
	}

	Node parent, current;

	parent.deepth = -1;
	parent.upper_bound = 0;
	parent.in_bag = 0;

	parent.bag_profit = 0;
	parent.bag_weight = 0;

	float fractionnal_max_profit = upper_bound(parent, objects);
	long max_profit = 0;

	printf("\t(2) Computing fractionnal maximum profit...\n\n\t\t%f\n\n", fractionnal_max_profit);

	PriorityQueue q;
	initQueue(&q, 1000000);

	HeapNode packet;
	packet.n = parent;
	packet.priority_score = 0;
	enqueue(packet, &q);

	printf("\t(3) Looking for better solutions... (profit, weight)\n\n");

	while(queueEmpty(&q) != 0)
	{
		NODES_VISITED++;

		/* Dequeue the first element */
		packet = dequeue(&q);
		parent = packet.n;

		/* Copy all data from parent to current node */
		current.deepth = parent.deepth + 1;
		current.bag_profit = parent.bag_profit + objects[current.deepth].value;
		current.bag_weight = parent.bag_weight + objects[current.deepth].weight;

		/* If profit of the current solution is greater than @max_profit, update it */
		if((current.bag_weight <= BAG_CAPACITY) && (current.bag_profit > max_profit))
		{
			current.in_bag = 1;
			max_profit = current.bag_profit;
			printf("\t\t(%10ld, %10ld)\n", current.bag_profit, current.bag_weight);

			/* We won't find a better solution */
			if(fractionnal_max_profit - max_profit < 1)
			{
				printf("\n\t\tOptimal solution found.\n");
				return max_profit;
			}
		}

		/* To know if we add object to the queue */
		current.upper_bound = upper_bound(current, objects);

		/* We take the current object */
		if(current.upper_bound > max_profit)
		{
			packet.n = current;
			packet.priority_score = 0 - current.upper_bound;
			enqueue(packet, &q);
		}

		/* We don't take the current object so reset node weight, profit and upper_bound */
		current.bag_weight = parent.bag_weight;
		current.bag_profit = parent.bag_profit;
		current.upper_bound = upper_bound(current, objects);

		if(current.upper_bound > max_profit)
		{
			current.in_bag = 0;
			packet.n = current;
			packet.priority_score = 0 - current.upper_bound;
			enqueue(packet, &q);
		}
	}

	return max_profit;
}
