/*
You have got a maze, which is a N*N Grid. Every cell of the maze contains these numbers 1, 2 or 3.
If it contains 1 : means we can go Right from that cell only.
If it contains 2 : means we can go Down from that cell only.
If it contains 3 : means we can go Right and Down to both paths from that cell.
We cant go out of the maze at any time.

Initially, You are on the Top Left Corner of The maze(Entry). And, You need to go to the Bottom Right Corner of the Maze(Exit).
You need to find the total number of paths from Entry to Exit Point.

There may be many paths but you need to select that path which contains the maximum number of Adventure.
The Adventure on a path is calculated by taking the sum of all the cell values thatlies in the path.
If no valid path exists, Adventure is zero.Since the total paths may be very large, Output it Modulo (1e9+7).

1<=T<=10
1<=N<=1000


*/

#include "Functions.h"

#define MOD 1000000007

int max(int a, int b)
{
	return (a >= b ? a : b);
}

int AdventureMazeWays(int** M, int** T, int n, int i, int j)
{
	if (i >= n || j >= n)	//Not in boundary
		return 0;

	if (i == n - 1 && j == n - 1)	//Base case for return, last cell
		return 1;

	if (T[i][j] != -1)	//memoization
		return T[i][j];

	if (M[i][j] == 1)	//Right
		return T[i][j] = AdventureMazeWays(M, T, n, i, j + 1) % MOD;

	if (M[i][j] == 2)	//Down
		return T[i][j] = AdventureMazeWays(M, T, n, i+1, j) % MOD;

	//means its 3 : Right or Down
	return T[i][j] = (AdventureMazeWays(M, T, n, i + 1, j) + AdventureMazeWays(M, T, n, i, j + 1)) % MOD;
	
}

int AdventureMazeMax(int** M, int** T, int n, int i, int j)
{
	if (i >= n || j >= n)	//Not in boundary
		return 0;

	if (i == n - 1 && j == n - 1)	//Base case for return, last cell
		return M[i][j];

	if (T[i][j] != -1)	//memoization
		return T[i][j];

	if (M[i][j] == 1)	//Right
		return T[i][j] = (AdventureMazeMax(M, T, n, i, j + 1) + M[i][j]) % MOD;

	if (M[i][j] == 2)	//Down
		return T[i][j] = (AdventureMazeMax(M, T, n, i + 1, j) + M[i][j]) % MOD;

	//means its 3 : Right or Top
	return T[i][j] = ((max(AdventureMazeMax(M, T, n, i + 1, j), AdventureMazeMax(M, T, n, i, j + 1))) + M[i][j]) % MOD;

}

void AdventureMaze()
{
	FILE *fp = fopen("Test.txt", "r");

	int N;
	fscanf(fp, "%d", &N);
	while (N--)
	{
		int n;
		fscanf(fp, "%d", &n);

		int** M = (int **)malloc(sizeof(int*)*n);
		if (!M)
			return;

		for (int i = 0; i < n; i++)
		{
			M[i] = (int *)malloc(sizeof(int)*n);
			if (!M[i])
				return;

			//memset(M[i], 0, sizeof(int)*n);
		}

		//DP Table
		int** T = (int **)malloc(sizeof(int*)*n);
		if (!T)
			return;

		for (int i = 0; i < n; i++)
		{
			T[i] = (int *)malloc(sizeof(int)*n);
			if (!T[i])
				return;

			//memset(T[i], 0, sizeof(int)*n);
		}

		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				fscanf(fp, "%d", &M[i][j]);
				T[i][j] = -1;
			}
		}
			
				
		printf("%d ", AdventureMazeWays(M, T, n, 0, 0));

		/*for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				printf("%d ", T[i][j]);
			}
			printf("\n");
		}*/

		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				T[i][j] = -1;
			}
		}

		printf("%d", AdventureMazeMax(M, T, n, 0, 0));

		/*for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				printf("%d ", T[i][j]);
			}
			printf("\n");
		}*/

		//Free mem after usage

		if (T)
		{
			for (int i = 0; i < n; i++)
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

		if (M)
		{
			for (int i = 0; i < n; i++)
			{
				if (M[i])
				{
					free(M[i]);
					M[i] = NULL;
				}
			}
			free(M);
			M = NULL;
		}
	}
	fclose(fp);
}			