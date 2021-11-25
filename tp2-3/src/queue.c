#include <stdlib.h>
#include <stdio.h>

#include "queue.h"
#include "extern.h"

void insert(HeapNode aNode, HeapNode* heap, int size)
{
	int idx;
	HeapNode tmp;
	idx = size + 1;
	heap[idx] = aNode;

	while(heap[idx].priority_score < heap[idx/2].priority_score && idx > 1)
	{
		tmp = heap[idx];
		heap[idx] = heap[idx/2];
		heap[idx/2] = tmp;
		idx /= 2;
	}
}

void shiftdown(HeapNode* heap, int size, int idx)
{
	int cidx;        //index for child
	HeapNode tmp;

	while(1)
	{
		cidx = idx * 2;
		if (cidx > size)
		{
			break; // it has no child
		}

		if (cidx < size)
		{
			if (heap[cidx].priority_score > heap[cidx+1].priority_score)
			{
				cidx++;
			}
		}

		//swap if necessary
		if (heap[cidx].priority_score < heap[idx].priority_score)
		{
			tmp = heap[cidx];
			heap[cidx] = heap[idx];
			heap[idx] = tmp;
			idx = cidx;
		}
		else
		{
			break;
		}
	}
}

HeapNode removeMin(HeapNode* heap, int size)
{
	HeapNode rv = heap[1];
	//printf("%d:%d:%dn", size, heap[1].priority_score, heap[size].priority_score);
	heap[1] = heap[size];
	size--;
	shiftdown(heap, size, 1);
	return rv;
}

void enqueue(HeapNode node, PriorityQueue *q)
{
	insert(node, q->heap, q->size);
	//printf("enqueue %d\n", node.n.deepth);
	q->size++;
}

HeapNode dequeue(PriorityQueue *q)
{
	HeapNode rv = removeMin(q->heap, q->size);
	//printf("dequeue %d\n", rv.n.deepth);
	q->size--;
	return rv;
}

void initQueue(PriorityQueue *q, int n)
{
	q->size = 0;
	q->heap = malloc(sizeof(HeapNode) * (n + 1));
}

int queueEmpty(PriorityQueue *q)
{
	return q->size;
}
