/*
The government has a plan to pave some roads with asphalt. The newly paved roads should satisfy the following two conditions:

(Condition 1) After the pavement of the new roads, all two villages must be connected by a path consisting of only the new paved roads.
(Condition 2) Minimize the number of the paved roads.

The pavement cost of each unpaved road is given as an integer depending on the length of the road.  The government hopes that the villages shall bear the cost equitably.  
So it will construct the new roads to minimize the difference between the maximum and the minimum among the pavement costs of the newly paved roads.

Time Limit: 3 second for 10 cases. (If your program exceeds this time limit, the answers that have been already printed are ignored and the score becomes 0. 

So, it may be better to print a wrong answer when a specific test case might cause your program to exceed the time limit. One guide for the time limit excess would be the size of the input.)
*/

/*Logic :
1. Sort edges by weight in non-decreasing order : qsort_edge
2. Find an index1 from Top, moving downwards which contains all vertex i.e. index1 edge must be included in the final graph for this
3. Build a graph from index1 to 0, moving upwards, so that max-min is least = diff1
4. Find an index2 from Bottom, moving upwards which contains all vertex i.e. index2 edge must be included in the final graph for this
5. Build a graph from index2 to E-1, moving downwards, so that max-min is least = diff2
6. If index2 > index1 (i.e. not overlap), we need to find an index1 and index2 by traversing edges alternatively in up-down fashion i.e. mid,1 up, 1 down,2 up,2 down ..
7. Build a graph from index2 to index1, moving upwards, so that max-min is least = diff3
8. Result = min(diff1, diff2, diff3)
*/

#include "Functions.h"

#define E 9	//no. of edges between villages/nodes

#define N 6 //no. of villages/nodes

#define INT_MAX 999999

typedef struct Node1	//v1 shall be < v2 .. no. wise, so that sorting can be done easily
{
	int v1;
	int v2;
	int wt;
}Node1;

void qsort_edge(Node1 V[], int start, int end);
int partition_edge(Node1 V[], int start, int end);
int AllInSameSet(int A[]);
int MST_mindiff(Node1 V[], int start, int end, int flag);

void pavement_road_minimize_difference()
{
	Node1 V[E] = {
		{ 1, 2, 8 },
		{ 1, 4, 10 },
		{ 4, 2, 4 },
		{ 2, 5, 1 },
		{ 3, 2, 9 },
		{ 2, 6, 6 },
		{ 6, 3, 3 },
		{ 4, 5, 11 },
		{ 5, 6, 7 },
	};	//Edge representation between villages

	for (int i = 0; i < E; i++)
	{
		if (V[i].v1 > V[i].v2)
		{
			//swap so that v1 < v2. as we going to traverse vertices 1 by 1
			int temp = V[i].v2;
			V[i].v2 = V[i].v1;
			V[i].v1 = temp;
		}
	}

	qsort_edge(V, 0, E - 1);	//quick sort call

	// printf("\n\nAfter Sort : \n");

	for (int i = 0; i < E; i++)
		printf("%d\t%d\t%d\n", V[i].v1, V[i].v2, V[i].wt);

	//making N sets array
	int A[N + 1]; //1 extra A[0] for simplicity of coding . initialise all with 0
	for (int i = 1; i < N + 1; i++)
		A[i] = 0;

	//top to bottom in sorted Edge list and finding the index at which all vertex come to same set : Downwards
	int index1;
	for (int i = 0; i < E; i++)
	{
		if (A[V[i].v1] == 0 && A[V[i].v2] == 0)	//base initial case
		{
			A[V[i].v2] = A[V[i].v1] = V[i].v1;
		}
		else if (A[V[i].v1] != 0 && A[V[i].v2] == 0)
		{
			A[V[i].v2] = A[V[i].v1];
		}
		else if (A[V[i].v1] == A[V[i].v2]) //both equal and != 0 hence no need to update any, and move on to next iteration directly
		{
			continue;
		}
		else if (A[V[i].v1] == 0) //first v1 index is 0 but second v2 index has already been present in the set
		{
			A[V[i].v1] = A[V[i].v2]; // hence to save scanning of whole of the array, lets put v2 value in v1, no matter which is smaller
		}
		else //means both not 0 and hence minimum of two shall be taken and update all corresponding related positions
		{
			int backup = -1, min = -1;
			
			if (A[V[i].v2] < A[V[i].v1])
			{
				backup = A[V[i].v1];
				min = A[V[i].v2];
			}
			else
			{
				backup = A[V[i].v2];
				min = A[V[i].v1];
			}
			for (int j = 1; j < N+1; j++)
			{
				if (A[j] == backup)
					A[j] = min;
			}
		}

		if (i >= N - 1)	//atleast N-1 combinations is necessary to put all vertex in same set
		{
			if (AllInSameSet(A))
			{
				index1 = i;
				break;
			}
		}
	}
		
	//Graph : 0 to index1 : Will build graph upwards in below function call
	int diff = MST_mindiff(V, 0, index1, 1);

	
	//bottom to top in sorted Edge list and finding the index at which all vertex come to same set : Upwards
	for (int i = 1; i < N + 1; i++)
		A[i] = 0;

	int index2;
	for (int i = E-1; i >= 0; i--)
	{
		if (A[V[i].v1] == 0 && A[V[i].v2] == 0)	//base initial case
		{
			A[V[i].v2] = A[V[i].v1] = V[i].v1;
		}
		else if (A[V[i].v1] != 0 && A[V[i].v2] == 0)
		{
			A[V[i].v2] = A[V[i].v1];
		}
		else if (A[V[i].v1] == A[V[i].v2]) //both equal and != 0 hence no need to update any, and move on to next iteration directly
		{
			continue;
		}
		else if (A[V[i].v1] == 0) //first v1 index is 0 but second v2 index has already been present in the set
		{
			A[V[i].v1] = A[V[i].v2]; // hence to save scanning of whole of the array, lets put v2 value in v1, no matter which is smaller
		}
		else //means both not 0 and hence minimum of two shall be taken and update all corresponding related positions
		{
			int backup = -1, min = -1;
			
			if (A[V[i].v2] < A[V[i].v1])
			{
				backup = A[V[i].v1];
				min = A[V[i].v2];
			}
			else
			{
				backup = A[V[i].v2];
				min = A[V[i].v1];
			}
			for (int j = 1; j < N + 1; j++)
			{
				if (A[j] == backup)
					A[j] = min;
			}
		}

		if (E - i >= N - 2)	//atleast N-1 combinations is necessary to put all vertex in same set
		{
			if (AllInSameSet(A))
			{
				index2 = i;
				break;
			}
		}
	}

	//means we need to build Graph 2 from E-1 to index 2, but we will make graph downwards in below function call
	int diff1 = MST_mindiff(V, index2, E - 1, 0);
	diff = diff < diff1 ? diff : diff1;

	for (int i = 1; i < N + 1; i++)
		A[i] = 0;
	
	if (1)//(index2 >= index1)	//means we need to build Graph 3 also from middle point i.e. (E+1)/2 i.e. from Median Cost Edge 1 downside and then 1 upside and so on till all edges get connected
	{
		int mid_index = (E + 1) / 2 - 1;
		int flag1 = 1;	//flag1 = 0 means upwards, 1 means downwards
		int k = 0;
		while (1)
		{
			if (flag1)
				mid_index = mid_index + k;
			else
				mid_index = mid_index - k;

			if (A[V[mid_index].v1] == 0 && A[V[mid_index].v2] == 0)	//base initial case
			{
				A[V[mid_index].v2] = A[V[mid_index].v1] = V[mid_index].v1;
			}
			else if (A[V[mid_index].v1] != 0 && A[V[mid_index].v2] == 0)
			{
				A[V[mid_index].v2] = A[V[mid_index].v1];
			}
			else if (A[V[mid_index].v1] == A[V[mid_index].v2]) //both equal and != 0 hence no need to update any, and move on to next iteration directly
			{
				k++;
				flag1 = !flag1;	//flag toggle for alternate traverse
				continue;
			}
			else if (A[V[mid_index].v1] == 0) //first v1 index is 0 but second v2 index has already been present in the set
			{
				A[V[mid_index].v1] = A[V[mid_index].v2]; // hence to save scanning of whole of the array, lets put v2 value in v1, no matter which is smaller
			}
			else //means both not 0 and hence minimum of two shall be taken and update all corresponding related positions
			{
				int backup = -1, min = -1;
				
				if (A[V[mid_index].v2] < A[V[mid_index].v1])
				{
					backup = A[V[mid_index].v1];
					min = A[V[mid_index].v2];
				}
				else
				{
					backup = A[V[mid_index].v2];
					min = A[V[mid_index].v1];
				}
				for (int j = 1; j < N + 1; j++)
				{
					if (A[j] == backup)
						A[j] = min;
				}
			}

			if (k >= N - 2)	//atleast N-1 combinations is necessary to put all vertex in same set
			{
				if (AllInSameSet(A))
				{
					if (flag1)
					{
						index1 = mid_index - k;
						index2 = mid_index;
					}
					else
					{
						index1 = mid_index;
						index2 = mid_index + k;
					}
					
					break;
				}
			}

			k++;
			flag1 = !flag1;	//flag toggle for alternate traverse

		}

	}

	//Graph3 will be made from index1 to index2
	diff1 = MST_mindiff(V, index1, index2, 1);
	diff = diff < diff1 ? diff : diff1;

	printf("\n\nMinimum Diff : %d", diff);
		
}

int MST_mindiff(Node1 V[], int start, int end, int flag)
{
	//int min = INT_MAX, max = -1;
	int C[N - 1];	//N-1 edges will be in final graph. Contains Min and Max cost
	int j = 0;	//for C array to populate with cost
	int A[N + 1];	//to check set subset and remove least weight edge to minimise difference at each level, A[0] is taken extra for code simplicity
	for (int i = 1; i < N + 1; i++)
		A[i] = 0;
	
	int i;
	if (flag) //flag = 1 means traversing upwards 
		i = end;
	else
		i = start;
	while (1)
	{	
		if (A[V[i].v1] == 0 && A[V[i].v2] == 0)	//base initial case
		{
			A[V[i].v2] = A[V[i].v1] = V[i].v1;
			C[j++] = V[i].wt;
			
		}
		else if (A[V[i].v2] == 0)
		{
			A[V[i].v2] = A[V[i].v1];
			C[j++] = V[i].wt;
			
		}
		else if (A[V[i].v1] == 0) //first v1 index is 0 but second v2 index has already been present in the set
		{
			A[V[i].v1] = A[V[i].v2]; 
			C[j++] = V[i].wt;

		}
		else if (A[V[i].v1] != A[V[i].v2]) //means both not 0 and hence minimum of two shall be taken and replace C[j-1] with new weight 
		{
			int backup = -1, min = -1;

			if (A[V[i].v2] < A[V[i].v1])
			{
				backup = A[V[i].v1];
				min = A[V[i].v2];
			}
			else
			{
				backup = A[V[i].v2];
				min = A[V[i].v1];
			}
			for (int j = 1; j < N + 1; j++)
			{
				if (A[j] == backup)
					A[j] = min;
			}

			C[j++] = V[i].wt;
		}

		if (flag) //flag = 1 means traversing upwards 
		{
			i--;
			if (i < start)
				break;
		}
		else
		{
			i++;
			if (i > end)
				break;
		}
	}

	if (flag)
		return C[0] - C[j-1];
	else
		return C[j - 1] - C[0];
}

int AllInSameSet(int A[])	//function to check if all vertex came in same set or not
{
	int i = 1;
	while (i < N)
	{
		if (A[i] != A[i + 1])
			return 0;

		i++;
	}
	return 1;
}

void qsort_edge(Node1 V[], int start, int end)
{
	int index = -1; //parition index
	if (start < end)
	{
		index = partition_edge(V, start, end);
		qsort_edge(V, start, index - 1);
		qsort_edge(V, index + 1, end);
	}
}
int partition_edge(Node1 V[], int start, int end)
{
	Node1 pivot = V[end];
	int index = start;
	Node1 temp;
	for (int i = start; i < end; i++)
	{
		if (V[i].wt <= pivot.wt)	//sort on the basis of v1 vertex as we are going linearly
		{
			//swap i, index elements
			temp = V[index];
			V[index] = V[i];
			V[i] = temp;

			index++;
		}
	}
	//swap index, pivot end
	temp = V[index];
	V[index] = V[end];
	V[end] = temp;

	return index;
}