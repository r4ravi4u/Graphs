/*

You're researching friendships between groups of  new college students where each student is distinctly numbered from  to . At the beginning of the semester, no student knew any other student; instead, they met and formed individual friendships as the semester went on. The friendships between students are:

Bidirectional. If student  is friends with student , then student  is also friends with student .
Transitive. If student  is friends with student  and student  is friends with student , then student  is friends with student . In other words, two students are considered to be friends even if they are only indirectly linked through a network of mutual (i.e., directly connected) friends.
The purpose of your research is to find the maximum total value of a group's friendships, denoted by . Each time a direct friendship forms between two students, you sum the number of friends that each of the  students has and add the sum to .

You are given  queries, where each query is in the form of an unordered list of  distinct direct friendships between  students. For each query, find the maximum value of  among all possible orderings of formed friendships and print it on a new line.

Set : Union & Find
*/

#include "Functions.h"

typedef struct Node	//for makeset and Union : Here child will point to its parent
{
	int parent;
	long long rank;
	//struct Node *parent;
}Node;

int find(Node* arr, int i)
{
	if (arr[i].parent != i)
		arr[i].parent = find(arr, arr[i].parent);	//Path Compression by Rank : Recursion

	return arr[i].parent;
}

long long Union(Node* arr, int x, int y)	//flag = 0 means x becomes parent else flag =1 , means y is parent
{
	//Find parents first of x & y, then compare ranks
/*	x = find(arr, x);
	y = find(arr, y);*/
	
	if (arr[x].rank >= arr[y].rank)
	{
		//make x as parent of y
		arr[y].parent = x;
		long long last = (arr[x].rank*(arr[x].rank + 1)) + (arr[y].rank*(arr[y].rank + 1));
		arr[x].rank += arr[y].rank+1;	//for total 		

		return (arr[x].rank*(arr[x].rank + 1)) - last;
	}
	
	//make y as parent of x
	arr[x].parent = y;
	long long last = (arr[x].rank*(arr[x].rank + 1)) + (arr[y].rank*(arr[y].rank + 1));
	arr[y].rank += arr[x].rank + 1;	//for total 
	
	return (arr[y].rank*(arr[y].rank + 1)) - last;

}

void Friendship()
{
	FILE *fp = fopen("Test.txt", "r");

	int t;
	fscanf(fp, "%d", &t);
	for (int a0 = 0; a0 < t; a0++)
	{
		int n;
		int m;
		fscanf(fp, "%d %d", &n, &m);

		Node* arr = (Node*)malloc(sizeof(Node)*(n+1));	//arr[0] is useless
		if (!arr)
			return;

		for (int i = 1; i <= n; i++)	//makeset
		{
			arr[i].parent = i;
			arr[i].rank = 0;
		}

		long long total = 0;
		long long curr_sum = 0;
		
		for (int a1 = 0; a1 < m; a1++)
		{
			int x;
			int y;
			fscanf(fp, "%d %d", &x, &y);

			//parent check, whether in same Set or Not
			x = find(arr, x);
			y = find(arr, y);

			if (x != y)	//means both not in same Set, we need to union them and check new Sum
			{
				long long sum = Union(arr, x, y);
				
				curr_sum += sum;
			}
				/*if (flag) //means y is parent of x
				{

					last_parent = y;
				}
					
				else
				{
					if (last_parent == x || last_parent == y)
						last_sum = sum;

					else
						last_sum += sum;

					last_parent = x;
				}*/

			total += curr_sum;
			
		}

		printf("%lld\n", total);

		if (arr)
		{
			free(arr);
			arr = NULL;
		}
	
	}

	fclose(fp);
}
