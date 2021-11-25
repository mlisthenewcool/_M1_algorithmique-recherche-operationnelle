#ifndef __EXTERN_H
#define __EXTERN_H

/* Flag used to display a lot of useful informations during execution */
extern unsigned char DEBUG_MODE;

/* Variables used in nearly all the functions */
extern unsigned long OBJECTS_COUNT;
extern unsigned long BAG_CAPACITY;

/* Counters to measure the performance of the algorithm */
extern unsigned long NODES_VISITED;
extern unsigned long NODES_IN_BOUND_VISITED;

/* */
typedef struct
{
	int id;
	float weight;
	float value;
} Object;

/* */
typedef struct
{
	long deepth;
	unsigned long upper_bound;
	unsigned char in_bag;

	unsigned long bag_profit;
	unsigned long bag_weight;
} Node;

#endif
