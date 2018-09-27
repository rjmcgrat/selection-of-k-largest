// Rodney McGrath 55352053
// Danh Nguyen 65872969
#include <stdlib.h>
#include <math.h>


//This is a growable array.
struct Array
{
	int size;
	int capacity;
	int * elementArray;
};


//This represents a node in a tournament tree
struct node
{
	int index;
	struct Array children;
};

//Functions to expand an array when it is too small to contain all elements that it needs to.
void expandArray(struct Array * arr)
{
	if (arr->capacity < arr->size + 1)
	{
		int * newArray = malloc(sizeof(int) * (arr->capacity) * 2);
		int i;
		for (i = 0; i < (arr->capacity); ++i)
			newArray[i] = (arr->elementArray[i]);
		arr->capacity = (arr->capacity * 2);
		free(arr->elementArray);
		arr->elementArray = newArray;
	}
}

//add an element to an array
void addToArray(int elementToAdd, struct Array * arr)
{
	expandArray(arr);
	arr->elementArray[arr->size] = elementToAdd;
	arr->size += 1;
}

//Clean up all the resources use
void cleanUp(struct node * nodes, int numElement)
{
	int i;
	for (i = 0; i < numElement; ++i)
		free(nodes[i].children.elementArray);
}

//Initialize the nodes array and an array of pointer that points to all nodes that has been initalized.
void init(struct node * nodes, struct node ** nodesPointer, int numElement)
{
	int i;
	for (i = 0; i < numElement; ++i)
	{
		nodes[i].index = i + 1;
		nodes[i].children.capacity = 1;
		nodes[i].children.size = 0;
		nodes[i].children.elementArray = malloc(sizeof(int) * 1);
		nodesPointer[i] = &nodes[i];
	}
}

//The algorithm to generate a tournament tree out of any number of nodes
struct node * createTournamentTree(struct node ** nodesPointers, int number_nodes)
{
	if (number_nodes == 1)
		return nodesPointers[0];
	struct node * newNodes[number_nodes % 2 == 0 ? (number_nodes / 2) : ((number_nodes / 2) + 1)]; //This array represents the nodes that are the winner of the current tournament stage
	int curIndex = 0;
	int increment = 0;
	//The following for loop is used to find the winner of the current tournament stage and populate the current tournament stage array.
	for (; curIndex < number_nodes - 1; curIndex += 2, ++increment)
	{
		int compareResult = COMPARE(nodesPointers[curIndex]->index, nodesPointers[curIndex + 1]->index); 
		if (compareResult == 1)
		{
			newNodes[increment] = nodesPointers[curIndex];
			addToArray(nodesPointers[curIndex + 1]->index, &(nodesPointers[curIndex]->children));
		}
		else if (compareResult == 2)
		{
			newNodes[increment] = nodesPointers[curIndex + 1];
			addToArray(nodesPointers[curIndex]->index, &(nodesPointers[curIndex + 1]->children));
		}
		else if ((compareResult == -1) || compareResult == -1000)
		{
			printf("Error comparison is out of bound/ invalid. This could be due to invalid input\n");
			fflush(stdout);
		}
	}

	//Since each competition in a tournament is done in group of 2, we will need to handle tournament stage where there are odd number of nodes.
	//This will give a buy to last node that happens to be part of a tournament where there are odd number of contestants total.
	if (number_nodes % 2 != 0)
	{
		newNodes[increment] = nodesPointers[number_nodes - 1];
		number_nodes += 1;
	}
	//Recursively find the winner of each tournament stage.
	return createTournamentTree(newNodes, number_nodes / 2);
}


//Get the k largest value from the tournament tree.
void extractKLargest(int k, int * Best, struct node * nodes, struct node * tournamentTree)
{
	int i;
	for (i = 0; i < k; ++i)
	{
		Best[i] = tournamentTree->index;	//The top layer of a tournament tree is the current largest value 
											//We then pop off the top element and recalculate the the next highest 
											//element based on the current highest element children.

		//The algorithm below is essentially simulating that the top node of the tournament tree has been pop off 
		//and next highest tree must be calculated.
		struct node * newNodes[tournamentTree->children.size];
		int j;
		for (j = 0; j < tournamentTree->children.size; ++j)
			newNodes[j] = &(nodes[tournamentTree->children.elementArray[j] - 1]);
		tournamentTree = createTournamentTree(newNodes, tournamentTree->children.size);
	}
}

int doalg(int n, int k, int Best[])
{
	COMPARE(0, n);
	fflush(stdout);
	struct node nodes[n];
	struct node * nodesPointers[n]; //This is nodes pointers array is used to modify the value of the same nodes rather than a copy version during execution.
	init(nodes, nodesPointers, n);
	extractKLargest(k, Best, nodes, createTournamentTree(nodesPointers, n));
	cleanUp(nodes, n);
	return 1;
}
