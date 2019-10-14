/*
Oliver and Bob are best friends. They have spent their entire childhood in the beautiful city of Byteland. The people of Byteland live happily along with the King.
The city has a unique architecture with total N houses. The King's Mansion is a very big and beautiful bungalow having address = 1. Rest of the houses in Byteland have some unique address, (say A), are connected by roads and there is always a unique path between any two houses in the city. Note that the King's Mansion is also included in these houses.

Oliver and Bob have decided to play Hide and Seek taking the entire city as their arena. In the given scenario of the game, it's Oliver's turn to hide and Bob is supposed to find him.
Oliver can hide in any of the houses in the city including the King's Mansion. As Bob is a very lazy person, for finding Oliver, he either goes towards the King's Mansion (he stops when he reaches there), or he moves away from the Mansion in any possible path till the last house on that path.

Oliver runs and hides in some house (say X) and Bob is starting the game from his house (say Y). If Bob reaches house X, then he surely finds Oliver.

Given Q queries, you need to tell Bob if it is possible for him to find Oliver or not.

The queries can be of the following two types:
0 X Y : Bob moves towards the King's Mansion.
1 X Y : Bob moves away from the King's Mansion

1 ≤ N ≤ 10^5
1 ≤ A,B ≤ N
1 ≤ Q ≤ 5*10^5
1 ≤ X,Y ≤ N

Topological Sort. Vertex will have 1 more info i.e. 0 or 1 attached while adding edge to a vertex

*/

#include "Functions.h"

void TopoSortBobOliverUtil(Graph* G, int flag, int x, int y);

int AllVisited(int* visited, int N)
{
	for (int i = 1; i <= N; i++)
	{
		if (!visited[i])
			return 0;
	}
	return 1;
}

int TopoSortGraph(Graph* G, int** T)
{
	int* visited = (int *)malloc(sizeof(int)*(G->V + 1));
	if (!visited)
		return;

	memset(visited, 0, sizeof(int)*(G->V + 1));

	AdjList** temp = (AdjList**)malloc(sizeof(AdjList*)*(G->V + 1));
	if (!temp)
		return;

	memset(temp, NULL, sizeof(AdjList*)*(G->V + 1));

	int* S = (int *)malloc(sizeof(int)*(G->V + 1));
	if (!S)
		return;

	memset(S, 0, sizeof(int)*(G->V + 1));

	int topT = 0;
	int top = 0;	//stack is empty
	S[++top] = 1;	//As 1 shall be starting vertex for Topo Sort
	temp[S[top]] = G->adj[1].head;
	while (top > 0 || !AllVisited(visited, G->V))
	{
		if (temp[S[top]] && temp[S[top]]->flag && !visited[temp[S[top]]->v])	//if not visited than only push else not
		{
			S[++top] = temp[S[top-1]]->v;
			temp[S[top - 1]] = temp[S[top - 1]]->next;

			if (!temp[S[top]])
				temp[S[top]] = G->adj[S[top]].head;
		}
		else if (!temp[S[top]])
		{
			//we need to pop as its all edges have been explored
			(*T)[++topT] = S[top];
			visited[S[top]] = 1;
			top--;

		}
		else if (!temp[S[top]]->flag || visited[temp[S[top]]->v])
		{
			temp[S[top]] = temp[S[top]]->next;
		}
	}

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

	if (visited)
	{
		free(visited);
		visited = NULL;
	}

	return topT;
}

void TopoSortUtil(Graph* G, int* S, int top, int flag, int x, int y, int n)
{
	if (x == y)
	{
		printf("YES\n");
		return;
	}

	if (n < 3)
	{
		if (y > x && !flag)
			printf("YES\n");
		else if (y < x && flag)
			printf("YES\n");
		else
			printf("NO\n");

		return;
	}

	int x_index = 0;
	int y_index = 0;

	while (top > 0)
	{
		if (S[top] == x)
			x_index = top;
		if (S[top] == y)
			y_index = top;

		if (x_index && y_index)
			break;

		top--;
	}

	if (flag)
	{
		if (y_index > x_index)
			//printf("YES\n");
			TopoSortBobOliverUtil(G, flag, x, y);
		else
			printf("NO\n");
	}
	else
	{
		if (y_index > x_index)
			printf("NO\n");
		else
			//printf("YES\n");
			TopoSortBobOliverUtil(G, flag, x, y);
	}

}

void TopoSortBobOliverUtil(Graph* G, int flag, int x, int y)	//From Y to X as per flag direction in Graph G
{
	/*if (x == y)
	{
		printf("YES\n");
		return;
	}

	if (G->V < 3)
	{
		if (y > x && !flag)
			printf("YES\n");
		else if (y < x && flag)
			printf("YES\n");
		else
			printf("NO\n");

		return;
	}*/

	//Now real Algo here, as its an application of Topological Sort
	int* visited = (int *)malloc(sizeof(int)*(G->V + 1));
	if (!visited)
		return;

	memset(visited, 0, sizeof(int)*(G->V + 1));

	AdjList** temp = (AdjList**)malloc(sizeof(AdjList*)*(G->V+1));
	if (!temp)
		return;

	memset(temp, NULL, sizeof(AdjList*)*(G->V + 1));

	int* S = (int *)malloc(sizeof(int)*(G->V + 1));
	if (!S)
		return;

	memset(S, 0, sizeof(int)*(G->V + 1));
	int top = 0;	//stack is empty	

	S[++top] = y;
	visited[S[top]] = 1;

	temp[S[top]] = G->adj[S[top]].head;
	
	int flag1 = 0;
	while (top > 0)
	{
		if (temp[S[top]] && flag == temp[S[top]]->flag && !visited[temp[S[top]]->v])	//if flag matches than only push else not
		{
			S[++top] = temp[S[top-1]]->v;
			visited[S[top]] = 1;
			
			temp[S[top-1]] = temp[S[top-1]]->next;

			if (!temp[S[top]])
				temp[S[top]] = G->adj[S[top]].head;

			if (S[top] == x)
			{
				printf("YES\n");
				flag1 = 1;
				break;
			}

		}
		else
		{
			if (!temp[S[top]])	//pop element
				top--;
			else
				temp[S[top]] = temp[S[top]]->next;
		}

		
	}

	if (!flag1)
		printf("NO\n");

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

	if (visited)
	{
		free(visited);
		visited = NULL;
	}
		

}

void TopoSortBobOliver()
{
	FILE *fp = fopen("Test.txt", "r");

	Graph* G = (Graph*)malloc(sizeof(Graph));
	if (!G)
		return;

	fscanf(fp, "%d", &G->V);	//No. of Nodes / Vertices, Hence N-1 Edges as its connected Acyclic Graph

	G->adj = (AdjListHead*)malloc(sizeof(AdjListHead)*((G->V) + 1));	//Array to store adjacency list of each Node. Array Index = Node no., hence taken 1 extra. 0 is useless. For simplicity of Code
	if (!(G->adj))
		return;

	memset(G->adj, NULL, sizeof(AdjListHead)*((G->V) + 1));

	for (int i = 0; i < (G->V - 1); i++)	//Nodes = V, so edges = V-1, hence input are in pairs, so total no. of inputs = 2*(V-1), but we are doing 2 scanf in single iteration
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
			G->adj[v1].head->flag = 1;	//1 means away from King (address = 1) and 0 means towards king (address = 1)
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
			temp1->flag = 1;
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
			G->adj[v2].head->flag = 0;	//1 means away from King (address = 1) and 0 means towards king (address = 1)
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
			temp1->flag = 0;
			temp1->next = NULL;
			temp->next = temp1;
		}


	}

	//Printing List
	/*for (int i = 1; i <= G->V; i++)
	{
		AdjList* temp = G->adj[i].head;
		printf("\n%d : ", i);
		while (temp)
		{
			printf("%d, %d\t", temp->v, temp->flag);
			temp = temp->next;
		}
	}*/

	//Now take Queries input
	int Q;
	fscanf(fp, "%d", &Q);

	int flag;	//either 0(towards King) or 1(opp. king)
	int X;	//Oliver house
	int Y;	//Bob House, Need to go from Y to X

	int* S = (int *)malloc(sizeof(int)*(G->V + 1));
	if (!S)
		return;

	memset(S, 0, sizeof(int)*(G->V + 1));

	int top = TopoSortGraph(G, &S);

	for (int i = 0; i < Q; i++)
	{
		fscanf(fp, "%d", &flag);
		fscanf(fp, "%d", &X);
		fscanf(fp, "%d", &Y);

		//TopoSortBobOliverUtil(G, flag, X, Y, &S1, &S0);*/

		TopoSortUtil(G, S, top, flag, X, Y, G->V);


	}
	
	

	//mem free after usage
	if (S)
	{
		free(S);
		S = NULL;
	}

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

	fclose(fp);
}