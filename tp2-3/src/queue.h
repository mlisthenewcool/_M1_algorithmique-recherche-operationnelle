#ifndef __QUEUE_H
#define __QUEUE_H

#include "extern.h"

typedef struct HeapNode
{
	int priority_score;
	Node n;
} HeapNode;

typedef struct PriorityQueue
{
    HeapNode* heap;
    int size;
} PriorityQueue;


void shiftdown(HeapNode* heap, int size, int idx);
void insert(HeapNode aNode, HeapNode* heap, int size);
HeapNode removeMin(HeapNode* heap, int size);
void enqueue(HeapNode node, PriorityQueue *q);
HeapNode dequeue(PriorityQueue *q);
void initQueue(PriorityQueue *q, int n);
int queueEmpty(PriorityQueue *q);

#endif
