/*
Consider a directed graph whose vertices are numbered from 1 to n. There is an edge from a vertex i to a vertex j iff either j = i + 1 or j = 3i. The task is to find the minimum number of edges in a path in G from vertex 1 to vertex n.

1<=T<=30
1<=n <=1000

*/

#include "Functions.h"

#define INT_MAX 9999

typedef struct node
{
	int parent;	//if parent=0, means not reachable till now
	int wt;	//if wt = INT_MAX, means not reachable and hence wt is infinity
	int visited;	//after extract_min, it become 1. initially its 0
}node;

int extract_min(node* arr, int V)	//rather than visiting whole array, a priority queue like min Heap can also be made so that we can get min in less time
{
	int min = INT_MAX;
	int index = 0;
	for (int i = 1; i <= V; i++)
	{
		if (!arr[i].visited)
		{
			if (min > arr[i].wt)
			{
				min = arr[i].wt;
				index = i;
				break;	//coz very first element found is the 
			}
		}
	}

	return index;
}

void DijsktraUtil(int** G, int V)
{
	struct node* arr = (node*)malloc(sizeof(node)*(V + 1));
	if (!arr)
		return;

	memset(arr, 0, sizeof(node)*(V + 1));

	for (int i = 1; i <= V; i++)
	{
		if (i == 1)
		{
			arr[i].parent = 1;	//we can make it 0 as well
			arr[i].wt = 0;
			arr[i].visited = 1;
			continue;
		}
		arr[i].parent = 0;
		arr[i].wt = INT_MAX;
	}

	int i = 1;
	while (!arr[V].visited)	//as we need to find edges i.e. wt to reach last V only
	{
		//i+1 & 3i will be its neighbours only as per ques. definition
		if (arr[i + 1].wt > arr[i].wt + 1)
		{
			arr[i + 1].parent = i;
			arr[i + 1].wt = arr[i].wt + 1;
		}

		if (3 * i <= V)
		{
			if (arr[3 * i].wt > arr[i].wt + 1)
			{
				arr[3 * i].parent = i;
				arr[3 * i].wt = arr[i].wt + 1;
			}
		}

		//i = extract_min(arr, V);
		/*if (3 * i <= V)
		{
			if (arr[3 * i].wt <= arr[i + 1].wt)
				i = 3 * i;
			else
				i = i + 1;
		}
		else*/
			i = i + 1;

		arr[i].visited = 1;		
	}

	printf("%d\n", arr[V].wt);
	
	//mem clear
	if (arr)
	{
		free(arr);
		arr = NULL;
	}
}

void Dijsktra()
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
			printf("0\n");
			continue;
		}
		
		if (V < 4)
		{
			printf("1\n");
			continue;
		}

		//Build 2D Adjacency Matrix, rather than Adjacency List here, as ques is like that only
		int** G = (int **)malloc(sizeof(int*)*(V+1));	//took V+1, so as for code simplification, as V starts from 1 to N
		if (!G)
			return;

		for (int i = 0; i < V+1; i++)
		{
			G[i] = (int *)malloc(sizeof(int)*3);	//0 useless, 1 for i+1, 2 for 3*i if exists
			if (!G[i])
				return;

			memset(G[i], 0, sizeof(int)*3);
		}

		for (int i = 1; i < V; i++)	//G[0] is useless
		{
			G[i][1] = i+1;
			if (3*i <= V)
				G[i][2] = 3*i;
		}

		DijsktraUtil(G, V);

		//Free mem after usage

		if (G)
		{
			for (int i = 0; i < V+1; i++)
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