#include "Functions.h"

void ud_graph()
{
	char v[]= { 'A', 'B', 'C', 'D', 'E', 'F','G' };

	//int size = sizeof(v) / sizeof(v[0]);

	struct Node1 *e[sizeof(v) / sizeof(v[0])] = { NULL };

	addEdge(e, v[0], v[1]);	//AB, BA
	addEdge(e, v[0], v[5]); //AF, FA

	addEdge(e, v[1], v[2]);	//BC, CB
	addEdge(e, v[1], v[3]);	//BD, DB

	addEdge(e, v[2], v[3]);	//CD, DC
	addEdge(e, v[2], v[4]);	//CE, EC
	
	addEdge(e, v[3], v[6]);	//DG, GD

	printGraph(e, v);
	 
	DFS(e, v);

//	printf("After DFS %c", v[0]);

/*	delEdge(e, v[4], v[2]); //CE, EC
	delEdge(e, v[0], v[2]); //CA, AC
	delEdge(e, v[2], v[3]);	//CD, DC

	printGraph(e, v); */
}

void DFS(struct Node1 *e[], char *v)
{

	if (!(*v))	//vertex list empty
		return;

	int n = 0, index; 
	struct Node1 *edge[arrSize] = { NULL };

	while (*(v + n) >= 'A' && *(v + n) <= 'Z')
	{
		edge[n] = e[n];	//making a copy of edge list to keep head moving
		n++; //total number of Vertices
	}
		
	bool visit[arrSize] = { 0 }; //is visited or not
	char s[arrSize] = { NULL };
	int i = 0, j=0; //i for v & visit, j for stack s

	s[j] = v[i];	//push vertex to stack
	printf("%c -> ", s[j]);
	visit[i] = true;

	while (*s && j < n) //stack not empty
	{
		index = s[j] - 'A';
		if (edge[index]) //push
		{
			i = edge[index]->data - 'A';
			if (!visit[i])
			{
				j++;
				s[j] = v[i];	//push vertex to stack
				printf("%c -> ", s[j]);
				visit[i] = true;
			}
			edge[index] = edge[index]->next;	//set index to point to next element in adjacency list so that we shall not traverse list again n again from start
		}
		else //pop for backtrack
		{
			s[j] = NULL;
			j--;
		}
		
	}

}

void addEdge(struct Node1 *e[], char v1, char v2)
{
	struct Node1 *temp1, *temp2;
	temp1 = (struct Node1 *)malloc(sizeof(struct Node));
	temp2 = (struct Node1 *)malloc(sizeof(struct Node));

	if (temp1 == NULL || temp2 == NULL)
		return; //mem full

	temp1->data = v1;
	temp2->data = v2;

	int index1 = v1 - 'A';
	int index2 = v2 - 'A';

	temp2->next = e[index1]; //edge v1 to v2
	e[index1] = temp2;

	temp1->next = e[index2]; //edge v2 to v1
	e[index2] = temp1;

	temp1 = temp2 = NULL;

	return;
}

void delEdge(struct Node1 *e[], char v1, char v2) //first we need to search whether edge exist or not and then delete if present
{
	struct Node1 *temp, *temp1 = NULL;
	int flag = 0; //0 means edge not found

	int index = v1 - 'A';

	temp = e[index]; //head 

	if (temp && temp->data == v2) //search for v2 in v1 list
	{
		e[index] = temp->next;
		flag = 1; // edge found
	}
	else if (temp)
	{
		while (temp->next)
		{
			if (temp->next->data == v2)
			{
				temp1 = temp->next;
				temp->next = temp1->next;
				flag = 1;
				break;
			}
			temp = temp->next;
		}
	}

	if (flag)
		free(temp1);

	if (flag) //for undirected graph if v1-v2 exists then v2-v1 also exist or vice versa
	{
		index = v2 - 'A';

		temp = e[index]; //head 

		if (temp && temp->data == v1) //search for v1 in v2 list
		{
			e[index] = temp->next;
			flag = 1; // edge found
		}
		else if (temp)
		{
			while (temp->next)
			{
				if (temp->next->data == v1)
				{
					temp1 = temp->next;
					temp->next = temp1->next;
					flag = 1;
					break;
				}
				temp = temp->next;
			}
		}
	}

	if (flag)
	{
		free(temp1);
		printf("*%c - %c Edge Deletion Done !!*\n\n", v1, v2);
	}

	if (flag == 0)
		printf("*%c - %c Edge not Found !! *\n\n", v1, v2);

	return;

}

void printGraph(struct Node1 *e[], char *v)
{
	int i=0, size=0;
	struct Node1 *temp;
	while (*(v + i) >= 'A' && *(v + i) <= 'Z')
	{
		size++;
		i++;
	}
		
	for (i = 0; i < size ; i++)
	{
		temp = e[i];
		while (temp)
		{
			printf("%c->", (char)('A' + i));
			printf("%c\t", temp->data);
			temp = temp->next;
		}
		printf("\n\n");
	}
}