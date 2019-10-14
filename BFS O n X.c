/*
Given N * M string array of O's and X's
Return the number of 'X' total shapes. 'X' shape consists of one or more adjacent X's (diagonals not included).

OOOXOOO
OXXXXXO
OXOOOXO

answer is 1 , shapes are  :
(i)     X
X X X X
X     X

XXX
OOO
XXX

answer is 2, shapes are
(i)  XXX

(ii) XXX

1<=T<=100

1<=N,M<=50

*/

#include "Functions.h"

typedef struct Cell	//to store X cell co-ordinate in queue
{
	int r;
	int c;
}Cell;

int isAllVisited(int **T, int N, int M, int* r, int* c)	//if T has all 1 return 1 else 0
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			if (!T[i][j])
			{
				*r = i;
				*c = j;
				return 0;
			}
				
		}
	}
	return 1;
}

void BFS_O_X_Util(char **G, int **T, int N, int M, int count, int r, int c)	//N rows, M columns, count = X total, r & c first X co-ordinates
{

	if (r < 0 || c < 0)	//means whole matrix is O, no X
	{
		printf("%d", 0);
		return;
	}

	//Now count will be used to build Queue for BFS. Since count is the total no. of X in matrix, hence at max count elements will be there in the queue in case all X connected
	struct Cell* q = (Cell *)malloc(sizeof(Cell)*count);
	if (!q)
		return;

	

	int pattern = 0;	//for final answer, atleast 1 should be there since r & c are > -1
	while (!isAllVisited(T, N, M, &r, &c))	//r & c to get co-ordinates of X found so that we need not to traverse matrix again for next X
	{
		int front = -1, rear = -1;	//q is NULL
		q[++rear].r = r;
		q[rear].c = c;
		front = rear;

		T[r][c] = 1;	//First X found made 1 i.e. visited

		pattern++;
		while (front <= rear) //while queue is not empty
		{
			if (q[front].r - 1 >= 0)	//Up element
			{
				if (!T[q[front].r - 1][q[front].c])	//means an X exist in the range
				{
					//Add cell to queue and make it visited
					q[++rear].r = q[front].r - 1;
					q[rear].c = q[front].c;
					T[q[front].r - 1][q[front].c] = 1;
				}
			}
			if (q[front].r + 1 < N)	//Down element
			{
				if (!T[q[front].r + 1][q[front].c])	//means an X exist in the range
				{
					//Add cell to queue and make it visited
					q[++rear].r = q[front].r + 1;
					q[rear].c = q[front].c;
					T[q[front].r + 1][q[front].c] = 1;
				}
			}
			if (q[front].c - 1 >= 0)	//Left element
			{
				if (!T[q[front].r][q[front].c - 1])	//means an X exist in the range
				{
					//Add cell to queue and make it visited
					q[++rear].r = q[front].r;
					q[rear].c = q[front].c - 1;
					T[q[front].r][q[front].c - 1] = 1;
				}
			}
			if (q[front].c + 1 < M)	//Right element
			{
				if (!T[q[front].r][q[front].c + 1])	//means an X exist in the range
				{
					//Add cell to queue and make it visited
					q[++rear].r = q[front].r;
					q[rear].c = q[front].c + 1;
					T[q[front].r][q[front].c + 1] = 1;
				}
			}

			front++;
		}
	}

	printf("%d\n", pattern);

	//free mem after usage
	if (q)
	{
		free(q);
		q = NULL;
	}


}

void BFS_O_X()
{
	FILE *fp = fopen("Test.txt", "r");

	int T;
	fscanf(fp, "%d", &T);
	while (T--)
	{
		int N, M;	//N rows, M Columns
		fscanf(fp, "%d", &N);
		fscanf(fp, "%d", &M);

		if (N==1 && M==1)
		{
			char c;
			fscanf(fp, "%s", &c);
			if (c == 'X')
				printf("%d\n", N);
			else
				printf("%d\n", N-1);
		}

		//Build 2D Adjacency Matrix, rather than Adjacency List here, as ques is like that only Matrix in the form of O and X chars
		char** G = (char **)malloc(sizeof(char*)*N);
		if (!G)
			return;

		for (int i = 0; i < N; i++)
		{
			G[i] = (char *)malloc(sizeof(char)*M);
			if (!G[i])
				return;

			//memset(G[i], '\0', sizeof(char));
		}

		//Visited Matrix T to store the cells which have already been visited, will initialise all with 0 and then wherever 'O' comes we make it 1, so that we can run loop till all becomes 1
		int** T = (int **)malloc(sizeof(int*)*N);
		if (!T)
			return;

		for (int i = 0; i < N; i++)
		{
			T[i] = (int *)malloc(sizeof(int)*M);
			if (!T[i])
				return;

			memset(T[i], 0, sizeof(int)*M);
		}

		int count = 0; //Total X in matrix
		int r=-1, c=-1;
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < M; j++)
			{
				fscanf(fp, "%c", &G[i][j]);

				while (G[i][j] != 'O' && G[i][j] != 'X')
					fscanf(fp, "%c", &G[i][j]);

				if (G[i][j] == 'O')
					T[i][j] = 1;
				else
				{
					count++;
					if (count == 1)	//means first X found
					{
						r = i;
						c = j;
					}
				}
					

					
			}
		}

		BFS_O_X_Util(G, T, N, M, count, r, c);

		//Free mem after usage

		if (T)
		{
			for (int i = 0; i < N; i++)
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

		if (G)
		{
			for (int i = 0; i < N; i++)
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