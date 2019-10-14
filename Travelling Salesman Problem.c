/*
Given a matrix M of size N where M[i][j] denotes the cost of moving from city i to city j. Your task is to complete a tour from the city 0 (0 based index) to all other cities such that you visit each city atmost once and then at the end come back to city 0 in min cost.

1<=T<=15
1<=N<=12
1<=M[][]<=10000

*/

#include "Functions.h"
#define INT_MAX 999999

int TSPUtil(int** G, int** T, int V, int mask, int pos)
{
	if (mask == (1 << V) - 1)
		return G[pos][0];

	if (T[mask][pos] != -1)	//memoization
		return T[mask][pos];

	int res = INT_MAX;

	for (int i = 0; i < V; i++)	//i is station here from 0 to V
	{
		if ((mask & (1 << i)) == 0)	//means current station not visited 
		{
			int curr_res = G[pos][i] + TSPUtil(G, T, V, (mask | (1<<i)), i);
			res = (res < curr_res) ? res : curr_res;
		}
	}

	return T[mask][pos] = res;
}

void TSP()
{
	FILE *fp = fopen("Test.txt", "r");

	int N;
	fscanf(fp, "%d", &N);
	while (N--)
	{
		int V;	//V Vertex
		fscanf(fp, "%d", &V);

		if (V == 1)
		{
			int e;
			fscanf(fp, "%d", &e);
			printf("%d\n", e);
			continue;
		}

		//Build 2D Adjacency Matrix, rather than Adjacency List here, as ques is like that only
		int** G = (int **)malloc(sizeof(int*)*V);
		if (!G)
			return;

		for (int i = 0; i < V; i++)
		{
			G[i] = (int *)malloc(sizeof(int)*V);
			if (!G[i])
				return;

			memset(G[i], 0, sizeof(int)*V);
		}

		for (int i = 0; i < V; i++)
			for (int j = 0; j < V; j++)
				fscanf(fp, "%d", &G[i][j]);

		if (V == 2)
			printf("%d\n", G[0][1] + G[1][0]);
		else
		{
			int** T = (int **)malloc(sizeof(int*)*(1<<V));	//1<<V Left shift means multiply by 2 -> equivalent to pow(2,V)
			if (!T)
				return;

			for (int i = 0; i < (1 << V); i++)
			{
				T[i] = (int *)malloc(sizeof(int)*V);
				if (!T[i])
					return;

				//memset(G[i], 0, sizeof(int)*V);
			}

			for (int i = 0; i < (1 << V); i++)
				for (int j = 0; j < V; j++)
					T[i][j] = -1;

			printf("%d\n", TSPUtil(G, T, V, 1, 0));

			if (T)
			{
				for (int i = 0; i < (1 << V); i++)
				{
					if (T[i])
					{
						free(T[i]);
						T[i] = NULL;
					}
				}
				free(T);
				T = NULL;
			}
		}
			

		//Free mem after usage
		if (G)
		{
			for (int i = 0; i < V; i++)
			{
				if (G[i])
				{
					free(G[i]);
					G[i] = NULL;
				}
			}
			free(G);
			G = NULL;
		}
	}

	fclose(fp);
}