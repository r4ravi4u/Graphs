#include "Functions.h"

void di_wt_graph()
{
	//Taking graph input from user
	/* 
	int n = 0;
	printf("Enter Total no. of Vertex : ");
	scanf_s("%d", &n);
	
	if (n < 1 || n > 26)	//Total 26 Alphabets present only
		return;

	char *v;
	v = (char *)malloc(n*sizeof(char));
	if (!v)
		return;
	memset(v, '\0', (n+1)*sizeof(char));	//memset n+1 so that last character gets NULL

	for (int i = 0; i < n; i++)
	{
		//getchar();	//to avoid \n as to get taken into buffer
		printf("Enter Name of Vertices (only 1 alphabet) : ");
		scanf_s(" %c", &v[i]);
		v[i] = (char)toupper(v[i]);
		//fgets(&v[i], 1, stdin);
		//gets_s(&v[i], 1);
	}

	sort_vertex(v, n);	//sorting so that we can refer index directly i.e. A to Z

	NodeW *e;
	e = (NodeW *)malloc(n*sizeof(NodeW));
	if (!e)
		return;
	memset(e, NULL, n*sizeof(NodeW));

	int n_e = 0;
	for (int i = 0; i < n; i++)
	{
		printf("Enter Total no. of Directed Edges from %c: ", v[i]);
		scanf_s("%d", &n_e);
		for (int j = 0; j < n_e; j++)
		{
			char c;
			int wt;
			getchar();
			printf("%c -> %d Destination & Weight", v[i], j+1);
			scanf_s(" %c", &c);
			scanf_s("%d", &wt);
			add_wt_Edge(e, v[i], (char)toupper(c), wt);
		}
	}
	
	print_wt_Graph(e, v);*/

	char v[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G' ,'\0' };

	struct NodeW *e[sizeof(v) / sizeof(v[0])] = { NULL };

	add_wt_Edge(e, v[0], v[1], 3);	//AB
	add_wt_Edge(e, v[0], v[3], 9);	//AD
	add_wt_Edge(e, v[0], v[4], 2);	//AE
	add_wt_Edge(e, v[0], v[0], 1);	//AA Self loop

	add_wt_Edge(e, v[1], v[2], 2);	//BC
	add_wt_Edge(e, v[1], v[0], 5);	//BA
	add_wt_Edge(e, v[1], v[2], 3);	//BC	parallel edge

	add_wt_Edge(e, v[2], v[1], 2);	//CB
	add_wt_Edge(e, v[2], v[3], 1);	//CD


	add_wt_Edge(e, v[3], v[2], 3);	//DC
	add_wt_Edge(e, v[3], v[0], 9);	//DA
	add_wt_Edge(e, v[3], v[5], 2);	//DF

	add_wt_Edge(e, v[4], v[5], 3);	//EF
	add_wt_Edge(e, v[4], v[0], 2);	//EA

	add_wt_Edge(e, v[5], v[3], 2);	//FD
	add_wt_Edge(e, v[5], v[4], 3);	//FE
	add_wt_Edge(e, v[5], v[5], 2);	//FF Self loop

	//No incoming edge to G, only outgoing edges from G
	add_wt_Edge(e, v[6], v[5], 2);	//GF
	add_wt_Edge(e, v[6], v[1], 3);	//GB


//	print_wt_Graph(e, v);
	
	dijsktra(e, v, v[6], v[0], sizeof(v) / sizeof(v[0])-1);
}

void dijsktra(NodeW *e[], char *v, char v1, char v2, int size)
{
	if (v1 == v2)
	{
		printf("Source & Destination Same : Cost is 0");
		return;
	}

	bool *visited;
	visited = (bool *)malloc(size*sizeof(bool));
	if (!visited)
		return;
	memset(visited, false, size*sizeof(bool));	//to keep track of visited nodes

	v_wt *path;
	path = (v_wt *)malloc(size*sizeof(v_wt));
	if (!path)
		return;

	memset(path, 0, size*sizeof(v_wt));
	for (int i = 0; i < size; i++)
		path[i].wt = INT_MAX;

	int index = v1 - 'A';
	//initialisation for source node
	path[index].via = NULL;
	path[index].wt = 0;

	//visited[index] = true;
	
	while (visit(visited, size))
	{
		NodeW *temp = e[index];
		while (temp)
		{
			if (temp->data != v[index] && temp->wt + path[index].wt < path[temp->data - 'A'].wt)	//temp->data != v[index] : to check for self loop, parallel edges will also be discarded by this whole if condition
			{
				path[temp->data - 'A'].via = v[index];
				path[temp->data - 'A'].wt = temp->wt + path[index].wt;
			}
			temp = temp->next;
		}
		visited[index] = true;
		index = find_min(path, visited, size);
		if (index < 0)
			break;

	}
	for (int i = 0; i < size; i++)
		printf("%c <- %c : %d\n", v[i], path[i].via, path[i].wt);

	index = v2 - 'A';
	if (path[index].wt == INT_MAX)
	{
		printf("Path does not exist Destination %c <- Source %c [Cost = Infinite] : ", v2, v1);
		return;
	}
	
	printf("Path Destination %c <- Source %c [Cost = %d] : ", v2, v1, path[index].wt);
	
	while (path[index].via)
	{
		printf("%c <- ", v[index]);
		index = path[index].via - 'A';
	}
	printf("%c \n ", v[index]);
}

int find_min(v_wt *path, bool *visited, int size)
{
	int min = INT_MAX, index = -1;
	for (int i = 0; i < size; i++)
	{
		if (path[i].wt < min && !visited[i])
		{
			min = path[i].wt;
			index = i;
		}
	}
	return index;
}

bool visit(bool *visited, int size)
{
	for (int i = 0; i < size; i++)
	{
		if (visited[i] == false)
			return true;
	}
	return false;
}

void sort_vertex(char *v, int n)
{
	for (int i = 0; i < n-1; i++)
	{
		for (int j = 0; j < n - i; j++)
		{
			if (v[j] > v[j + 1] && v[j] && v[j+1]) //swap
			{
				char temp = v[j];
				v[j] = v[j + 1];
				v[j + 1] = temp;
			}
		}
	}
}

void print_wt_Graph(NodeW *e[], char *v)
{
	int i, size = 0;
	NodeW *temp;
	while (v[size] >= 'A' && v[size] <= 'Z')
		size++;

	for (i = 0; i < size; i++)
	{
		temp = e[i];	//to not to alter original e array
		while (temp)
		{
			printf("%c->", (char)('A' + i));
			printf("%c : %d\t", temp->data, temp->wt);
			temp = temp->next;
		}
		printf("\n");
	}
}

void add_wt_Edge(NodeW *e[], char v1, char v2, int wt)	//wt = weight of the edge
{
	NodeW *temp;
	temp = (NodeW *)malloc(sizeof(struct Node));

	if (temp == NULL)
		return; //mem full

	temp->data = v2;
	temp->wt = wt;

	int index = v1 - 'A';

	temp->next = e[index]; //edge v1 to v2
	e[index] = temp;

	temp = NULL;

	return;
}