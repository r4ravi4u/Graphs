#include "Functions.h"

void di_graph()
{
	char v[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J' };

	struct Node1 *e[sizeof(v) / sizeof(v[0])] = { NULL };

	add_di_Edge(e, v[0], v[5]); //AF
	add_di_Edge(e, v[0], v[1]);	//AB
	
	add_di_Edge(e, v[1], v[7]);	//BH

	add_di_Edge(e, v[3], v[8]);	//DI
	add_di_Edge(e, v[3], v[7]);	//DH
	add_di_Edge(e, v[3], v[4]);	//DE
	add_di_Edge(e, v[3], v[2]);	//DC
	
	add_di_Edge(e, v[4], v[8]);	//EI
	

	add_di_Edge(e, v[6], v[2]);	//GC
	add_di_Edge(e, v[6], v[1]);	//GB
	add_di_Edge(e, v[6], v[0]);	//GA

	add_di_Edge(e, v[8], v[2]);	//IC
//	add_di_Edge(e, v[8], v[3]);	//ID

	add_di_Edge(e, v[9], v[4]);	//JE

	print_di_Graph(e, v);

	toposort(e, v);

	//	printf("After DFS %c", v[0]);

	/*	del_di_Edge(e, v[4], v[2]); //CE, EC
	del_di_Edge(e, v[0], v[2]); //CA, AC
	del_di_Edge(e, v[2], v[3]);	//CD, DC

	print_di_Graph(e, v); */
}

bool push(struct Node1 **head, char data)
{
	struct Node1 *temp;
	
	temp = *head;
	while (temp)
	{
		if (temp->data == data)
			return false;
		temp = temp->next;
	}

	temp = (struct Node1 *)malloc(sizeof(struct Node1));

	if (!temp)
		return false;

	temp->data = data;
	temp->next = *head;
	*head = temp;
	temp = NULL;
	return true;
}

int pop(struct Node1 **head)
{
	if (!(*head))
		return -1;
	struct Node1 *temp;
	char data;
	temp = *head;

	*head = temp->next;
	data = temp->data;
	free(temp);
	return (data-'A');
}

void toposort(struct Node1 *e[], char *v)
{

	if (!(*v))	//vertex list empty
		return;

	int n = 0, index;
	struct Node1 *edge[arrSize] = { NULL };

	while (v[n] >= 'A' && v[n] <= 'Z')
	{
		edge[n] = e[n];	//making a copy of edge list to keep head moving
		n++; //total number of Vertices
	}

	bool visit[arrSize] = { 0 }; //is visited or not
	char s[arrSize] = { NULL };
	int i = 0, j = 0 ; //i for v & visit, j for stack s

	struct Node1 *head = NULL;	//stack linked list for maintaning parent nodes

	while (j < n) //stack not empty
	{
		index = v[i] - 'A';
		if (edge[index]) //push
		{
			if (!push(&head, v[i]))	//stack linked list to get previous nodes after going till last depth && if condition to check for cyclic graph or not
			{
				printf("\nThis is a directed cyclic graph. Topological sort is not possible\n");
				return;
			}
			i = edge[index]->data - 'A';
			if (edge[index]->data == v[index])	//there is self loop in the graph
			{
				printf("\nThere is a self loop in the graph. Topological sort is not possible\n");
				return;
			}
				
			/*if (!visit[i])
			{
				visit[i] = true;
			}*/
			edge[index] = edge[index]->next;	//set index to point to next element in adjacency list so that we shall not traverse list again n again from start
		}
		else //push to stack
		{
			if (!visit[i])
			{
				visit[i] = true;
				s[j] = v[i];
				j++;
			}
							
			i = pop(&head);
			if(i == -1) //backtrack done, choose a new unvisited node from Vertex list
			{
				i = 0;
				while (visit[i]) //find a new node which in not visited, if all true come out of loop
				{
					i++;
					if (i >= arrSize) //process finished, print stack
					{
						j = n;
						break;
					}
				}
				//visit[i] = true;
			}

		}

	}

	printf("\nTopological Sort is below : \n\n");
	while (j >= 1)
		printf("%c->\t", s[--j]);

}

void print_di_Graph(struct Node1 *e[], char *v)
{
	int i, size = 0;
	struct Node1 *temp;
	while (v[size] >= 'A' && v[size] <= 'Z')
		size++;

	for (i = 0; i < size; i++)
	{
		temp = e[i];
		while (temp)
		{
			printf("%c->", (char)('A' + i));
			printf("%c\t", temp->data);
			temp = temp->next;
		}
		printf("\n");
	}
}

void add_di_Edge(struct Node1 *e[], char v1, char v2)
{
	struct Node1 *temp;
	temp = (struct Node1 *)malloc(sizeof(struct Node));
	
	if (temp == NULL)
		return; //mem full

	temp->data = v2;

	int index = v1 - 'A';

	temp->next = e[index]; //edge v1 to v2
	e[index] = temp;

	temp = NULL;

	return;
}

void del_di_Edge(struct Node1 *e[], char v1, char v2) //first we need to search whether edge exist or not and then delete if present
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

