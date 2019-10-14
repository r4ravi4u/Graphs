/*

*/

#include "Functions.h"
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <stdbool.h>

typedef struct SetNode	//for makeset and Union : Here child will point to its parent
{
	int data;
	int rank;
	struct SetNode *parent;
}SetNode;

void qsort_edge(int** edges, int start, int end)
{
	int index = -1; //parition index
	if (start < end)
	{
		index = partition_edge(edges, start, end);
		qsort_edge(edges, start, index - 1);
		qsort_edge(edges, index + 1, end);
	}
}
int partition_edge(int** edges, int start, int end)
{
	int* pivot = edges[end];
	int index = start;
	int* temp;
	for (int i = start; i < end; i++)
	{
		if (edges[i][2] < pivot[2])	//sort on the basis of v1 vertex as we are going linearly
		{
			//swap i, index elements
			temp = edges[index];
			edges[index] = edges[i];
			edges[i] = temp;

			index++;
		}
		else if (edges[i][2] == pivot[2]) //special condition as per Ques
		{
			if ((edges[i][0] + edges[i][1]) < (pivot[0] + pivot[1]))
			{
				//swap i, index elements
				temp = edges[index];
				edges[index] = edges[i];
				edges[i] = temp;

				index++;
			}
		}
	}
	//swap index, pivot end
	temp = edges[index];
	edges[index] = edges[end];
	edges[end] = temp;

	return index;
}

SetNode* makeSet(int data)
{
	SetNode* node = (SetNode*)malloc(sizeof(SetNode));
	if (!node)
		return NULL;

	node->data = data;
	node->rank = 0;
	node->parent = node;	//self loop

	return node;
}

bool findSet(SetNode* v1, SetNode* v2)
{
	/*if (v1->parent != v1->parent->parent)	//path compression 
	{
		SetNode* temp = v1;
		while (temp != temp->parent)
			temp = temp->parent;

		v1->parent = temp;
	}

	if (v2->parent != v2)	//path compression 
	{
		SetNode* temp = v2;
		while (temp != temp->parent)
			temp = temp->parent;

		v2->parent = temp;
	}

	if (v1->parent != v2->parent)
		return true;

	return false;*/

	SetNode* temp1 = v1;
	SetNode* temp2 = v2;
	while (temp1 != temp1->parent || temp2 != temp2->parent)
	{
		temp1 = temp1->parent;
		temp2 = temp2->parent;
	}
	if (temp1 != temp2)
		return true;
	else
		return false;

}

void unionSet(SetNode** v1, SetNode** v2)
{
	if ((*v1)->rank >= (*v2)->rank)	//Union by Rank
	{
		(*v1)->rank += 1;
		(*v2)->rank = 0;
		(*v2)->parent = (*v1);

	}
	else
	{
		(*v2)->rank += 1;
		(*v1)->rank = 0;
		(*v1)->parent = (*v2);

	}

	if ((*v2)->parent != (*v2)->parent->parent)	//path compression 
	{
		SetNode* temp = (*v2)->parent;
		while (temp != temp->parent)
			temp = temp->parent;

		(*v2)->parent = temp;
	}

	if ((*v1)->parent != (*v1)->parent->parent)	//path compression 
	{
		SetNode* temp = (*v1)->parent;
		while (temp != temp->parent)
			temp = temp->parent;

		(*v1)->parent = temp;
	}

}

bool isAllSetConnected(SetNode** v, int n)
{
	for (int i = 1; i < n; i++)
	{
		if (findSet(v[i], v[i + 1]))
			return false;

		/*SetNode* temp1 = v[i];
		SetNode* temp2 = v[i+1];
		while (temp1 != temp1->parent || temp2 != temp2->parent)
		{
			temp1 = temp1->parent;
			temp2 = temp2->parent;
		}
		
		if (temp1 != temp2)
			return false;*/
	}

	return true;
}

int mst(int n, int edges_size_rows, int edges_size_cols, int** edges) 
{
	qsort_edge(edges, 0, edges_size_rows - 1);	//Edges are sorted in non-decreasing order

	FILE *fp = fopen("Out.txt", "w");
	for (int edges_i = 0; edges_i < edges_size_rows; edges_i++)
		fprintf(fp, "%d -> %d : %d\n", edges[edges_i][0], edges[edges_i][1], edges[edges_i][2]);

	//Now we will do Makeset, Union and FindSet to make up the MST
	SetNode** v = (SetNode**)malloc(sizeof(SetNode*)*(n+1));
	if (!v)
		return 0;

	v[0] = NULL;

	for (int i = 1; i <= n; i++)
		v[i] = makeSet(i);	//since node no. start from 1 not 0. n vertices hence n sets will be made

	int wt = 0;
	for (int i = 0; i < edges_size_rows;i++)	//loop will break once all vertex come under same parent i.e. all nodes are connected by n-1 edges 
	{
		if (findSet(v[edges[i][0]], v[edges[i][1]]))	//if their parent is different (1) then Union, else go to next edge
		{
			unionSet(&v[edges[i][0]], &v[edges[i][1]]);	//sending address so that changes gets reflected, as we don't know which becomes parent
			//printf("%d = %d (Data) : %d (Rank) : %d (Parent's data)\n\n", i, v[i]->data, v[i]->rank, v[i]->parent->data);
			wt += edges[i][2];
		}

		if (i >= n - 2)	//As n-1 edges, so we shall check if all nodes comes under same parent or not, n-2 because i starts from 0
		{
			if (isAllSetConnected(v, n))
				break;
		}
	}

	//free mem
	if (v)
	{
		for (int i = 0; i <= n; i++)
		{
			if (v[i])
			{
				free(v[i]);
				v[i] = NULL;
			}
		}
		free(v);
		v = NULL;
	}

	return wt;
	
}

int Kruskal_MST()
{
	FILE *fp = fopen("Test.txt", "r");
	int n;
	int m;
	fscanf(fp, "%i %i", &n, &m);
	int** edges = (int**)malloc(sizeof(int *)*m);	//m rows & 3 columns for test case input
	if (!edges)
		return 0;

	for (int i = 0; i < m; i++)
	{
		edges[i] = (int *)malloc(sizeof(int)* 3);
		if (!edges[i])
			return 0;

		memset(edges[i], 0, sizeof(int)* 3);
	}

	for (int edges_i = 0; edges_i < m; edges_i++) {
		for (int edges_j = 0; edges_j < 3; edges_j++) {

			fscanf(fp, "%i", &edges[edges_i][edges_j]);
		}
	}
	int result = mst(n, m, 3, edges);
	printf("%d\n", result);

	//mem free after usage
	if (edges)
	{
		for (int i = 0; i < m; i++)
		{
			if (edges[i])
			{
				free(edges[i]);
				edges[i] = NULL;
			}
		}
		free(edges);
		edges = NULL;
	}

	fclose(fp);
	return 0;
}