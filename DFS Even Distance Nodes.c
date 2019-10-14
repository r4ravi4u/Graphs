/*
Given a connected acyclic graph with N nodes and N-1 edges, find out the pair of nodes that are at even distance from each other.

1<=T<=10

1<=N<=10000

1<=xi,yi<=N

*/

#include "Functions.h"

int isVisited(int *T, int N)
{
	for (int i = 0; i < N; i++)
	{
		if (!T[i])
			return 0;
	}
	return 1;
}

void DFSEvenDistanceUtil(Graph* G)
{
	/*
	Need to maintain 2 things : 
	1. Stack : For DFS of the current node : Once it becomes empty, exit the loop for the current node. Its index tell us about the distance from particular node
	2. Visited Set :
	
	Stack Size = Visited Set Size = G->V = No. of Nodes
	*/

	int *S = (int *)malloc(sizeof(int)*(G->V + 1));
	if (!S)
		return;

	int top = 0;

	int *T = (int *)malloc(sizeof(int)*(G->V + 1));	//T = Visited Set. + 1 done, for ease of code : Now T[i] = G->adj[i].head .. like this we can do
	if (!T)
		return;

	struct AdjList **temp = (AdjList **)malloc(sizeof(AdjList*)*(G->V + 1));	//so that we shall not traverse the edge list again from start each time 
	if (!temp)
		return;

	memset(temp, NULL, sizeof(AdjList*)*((G->V) + 1));

	memset(T, 0, sizeof(int)*(G->V + 1));
	T[0] = 1;	//As its of no use, All nodes are not visited yet

	int even = 0;
	int odd = 0;
	for (int i = 1; i <= 1; i++)
	{
		S[++top] = i;
		T[i] = 1;	//node visited for particular node
		while (top >= 1)	//while stack is not empty, when top = 0, no use
		{
			if (!T[G->adj[S[top]].head->v])
			{
				S[++top] = G->adj[S[top-1]].head->v;
				T[G->adj[S[top - 1]].head->v] = 1;
			}
			
			else
			{
				if (!temp[S[top]])
					temp[S[top]] = G->adj[S[top]].head;
			
				while (temp[S[top]] && T[temp[S[top]]->v])
					temp[S[top]] = temp[S[top]]->next;

				if (temp[S[top]])
				{
					S[++top] = temp[S[top-1]]->v;
					T[temp[S[top-1]]->v] = 1;
				}
				else //means we need to pop out from Stack as we reached end of the edge list of this particular top and hence we shall also check if its even distance or odd distance
				{
					/*if (S[top] > S[1] && ((top - 1) % 2 == 0))	//-1 since we have started stack from 1 not 0 ; S[top] > S[1] condition to eradicate duplicates
					{
						count++;
					}*/
					if ((top - 1) % 2 == 0)
						even++;
					else
						odd++;

					top--;
				}
			}
		}
		//Now since stack is empty, hence our visited array shall also be true : We need to make all memset to 0 and False
		memset(T, 0, sizeof(int)*(G->V + 1));
		top = 0;
	}

	int count = (odd*(odd - 1) / 2) + (even*(even - 1) / 2);
	
	printf("%d\n", count);

	//free mem after usage
	if (temp)
	{
		free(temp);
		temp = NULL;
	}

	if (S)
	{
		free(S);
		S = NULL;
	}

	if (T)
	{
		free(T);
		T = NULL;
	}


}

void DFS_EvenDistanceNodePairCount()
{
	FILE *fp = fopen("Test.txt", "r");

	int T;
	fscanf(fp, "%d", &T);
	while (T--)
	{
		Graph* G = (Graph*)malloc(sizeof(Graph));
		if (!G)
			return;

		fscanf(fp, "%d", &G->V);	//No. of Nodes / Vertices, Hence N-1 Edges as its connected Acyclic Graph

		if (G->V < 3)
		{
			printf("0");
			return;
		}

		G->adj = (AdjListHead*)malloc(sizeof(AdjListHead)*((G->V) + 1));	//Array to store adjacency list of each Node. Array Index = Node no., hence taken 1 extra. 0 is useless. For simplicity of Code
		if (!(G->adj))
			return;

		memset(G->adj, NULL, sizeof(AdjListHead)*((G->V) + 1));

		for (int i = 0; i < (G->V-1); i++)	//Nodes = V, so edges = V-1, hence input are in pairs, so total no. of inputs = 2*(V-1), but we are doing 2 scanf in single iteration
		{
			//For every pair, we need to insert 2 edges v1 to v2 and v2 to v1 as its an undirected graph
			
			int v1, v2;
			fscanf(fp, "%d", &v1);
			fscanf(fp, "%d", &v2);
			
			//for v1 to v2 edge
			if (!G->adj[v1].head)	//index of the array will behave as a node no.
			{
				G->adj[v1].head = (AdjList *)malloc(sizeof(AdjList));
				if (!G->adj[v1].head)
					return;

				G->adj[v1].head->v = v2;
				G->adj[v1].head->next = NULL;
			
			}

			else
			{
				AdjList* temp = G->adj[v1].head;
				while (temp->next)
					temp = temp->next;

				AdjList* temp1 = (AdjList *)malloc(sizeof(AdjList));
				if (!temp1)
					return;

				temp1->v = v2;
				temp1->next = NULL;
				temp->next = temp1;
			}
			

			//for v2 to v1 edge
			if (!G->adj[v2].head)	//index of the array will behave as a node no.
			{
				G->adj[v2].head = (AdjList *)malloc(sizeof(AdjList));
				if (!G->adj[v2].head)
					return;

				G->adj[v2].head->v = v1;
				G->adj[v2].head->next = NULL;
				continue;

			}

			else
			{
				AdjList* temp = G->adj[v2].head;
				while (temp->next)
					temp = temp->next;

				AdjList* temp1 = (AdjList *)malloc(sizeof(AdjList));
				if (!temp1)
					return;

				temp1->v = v1;
				temp1->next = NULL;
				temp->next = temp1;
			}
			
			
		}

		//Printing List
		/*for (int i = 1; i <= G->V; i++)
		{
			AdjList* temp = G->adj[i].head;
			printf("\n%d : ", i);
			while (G->adj[i].head)
			{
				printf("%d  ", G->adj[i].head->v);
				G->adj[i].head = G->adj[i].head->next;
			}
			G->adj[i].head = temp;	//head restore
		}*/

		DFSEvenDistanceUtil(G);

		//mem free after usage
		for (int i = 0; i <= G->V; i++)
		{
			AdjList* temp;
			if (G->adj[i].head)
			{
				while (G->adj[i].head)
				{
					temp = G->adj[i].head;
					G->adj[i].head = G->adj[i].head->next;
					free(temp);
					temp = NULL;
				}
			}
		}

		if (G->adj)
		{
			free(G->adj);
			G->adj = NULL;
		}
		
		if (G)
		{
			free(G);
			G = NULL;
		}



	}

	fclose(fp);
}