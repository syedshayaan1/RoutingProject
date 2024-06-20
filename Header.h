/*
20i-0436
Shayan Faisal
CS-A
Data Structures Project
Header file for classes and structures
*/

#include <iostream>
using namespace std;

//routing pairs
struct RoutingPairs
{
	string dest;
	string outgoingQueue;

	RoutingPairs()
	{
		dest = "";
		outgoingQueue = "";
	}
};

//routing table
struct RoutingTable
{
	RoutingPairs* rPairs;

	//function to find the corresponding outgoing queue of a specific destination in routing table
	string findQueueByDest(string d, int n)
	{
		for (int i = 0; i<n; i++)
		{
			if (rPairs[i].dest == d)
				return rPairs[i].outgoingQueue;
		}
		return "";
	}
};

//message structure
struct Message
{
	int id;
	int priority;	//value from 1 to 10
	string src_address;
	string dest_address;
	string payload;
	string trace;

	//constructors
	Message()
	{

	}
	Message(int i, int p, string src, string dest, string m, string t)
	{
		id = i;
		priority = p;
		src_address = src;
		dest_address = dest;
		payload = m;
		trace = t;
	}
};

//Queue structure (outgoing queue)
//Queue node
struct QNode
{
	Message* data;
	QNode* next;

	QNode(Message* d)
	{
		data = d;
		next = NULL;
	}
};

//Queue
struct Queue
{
	QNode* front;
	QNode* rear;

	Queue()
	{
		front = NULL;
		rear = NULL;
	}

	//enqueue function
	void enQueue(Message* x)
	{

		//creating a new node
		QNode* temp = new QNode(x);

		//if queue is empty
		if (rear == NULL)
		{
			front = temp;
			rear = temp;
			return;
		}

		//otherwise add it at the end of queue and update rear
		rear->next = temp;
		rear = temp;
	}

	//dequeue function
	Message* deQueue()
	{
		//return null if queue is empty
		if (front == NULL)
			return NULL;

		//moving front one node ahead
		QNode* temp = front;
		front = front->next;

		//in case front becomes null then rear also becomes null
		if (front == NULL)
			rear = NULL;

		return temp->data;

		//delete (temp);
	}
};

//main node class also used in graphs
template <class T>
class Node
{
public:
	T data;
	string name;
	int weight;
	Queue incomingQueue;
	Queue outgoingQueue;
	RoutingTable* routingTable;
	Node<T>* next;

	//default constructor
	Node()
	{
		data = 0;
		weight = 0;
		routingTable = new RoutingTable;
		next = NULL;
	}
	//parameterized constructor
	Node(T d, Node<T>* n)
	{
		data = d;
		next = n;
	}
	//getters and setters
	T getData()
	{
		return data;
	}
	Node<T>* getNext()
	{
		return next;
	}
	void setData(T d)
	{
		data = d;
	}
	void setNext(Node<T>* n)
	{
		next = n;
	}
};

//linked list class
template <class T>
class LinkedList
{
public:
	Node<T>* head;

	//default constructor
	LinkedList(void)
	{
		head = NULL;
	}

	//destructor
	~LinkedList(void)
	{
		Node<T>* currNode = head;
		Node<T>* nextNode = NULL;
		while (currNode != NULL)
		{
			nextNode = currNode->next;
			delete currNode;	// destroy the current node
			currNode = nextNode;
		}
		head = NULL;
	}

	//function to check if list is empty
	bool IsEmpty()
	{
		if (head == NULL)
		{
			return true;
		}
		return false;
	}

	//function to insert at end
	Node<T>* insert(T x, string n, int w)
	{
		Node<T>* newNode = new Node<T>;
		newNode->data = x;
		newNode->name = n;
		newNode->weight = w;
		Node<T>* currNode = head;
		if (currNode == NULL)
		{
			head = newNode;
		}
		else
		{
			while (currNode->next != NULL)
			{
				currNode = currNode->next;
			}
			currNode->next = newNode;
		}
		return newNode;
	}

	//function to insert at head
	Node<T>* insertAtHead(T x)
	{
		Node<T>* newNode = new Node<T>;
		newNode->data = x;
		newNode->next = head;
		head = newNode;
		return newNode;
	}

	//function to insert at index
	Node<T>* InsertAtIndex(T x, int index)
	{
		if (index < 0) return NULL;
		int currIndex = 1;
		Node<T>* currNode = head;
		while (currNode && index > currIndex)
		{
			currNode = currNode->next;
			currIndex++;
		}
		if (index > 0 && currNode == NULL) return NULL;
		Node<T>* newNode = new Node<T>;
		newNode->data = x;
		if (index == 0)
		{
			newNode->next = head;
			head = newNode;
		}
		else
		{
			newNode->next = currNode->next;
			currNode->next = newNode;
		}
		return newNode;
	}

	//function to search for some element in list
	int search(T x)
	{
		Node<T>* currNode = head;
		int currIndex = 1;
		while (currNode && currNode->data != x)
		{
			currNode = currNode->next;
			currIndex++;
		}
		if (currNode && currNode->data == x)
			return currIndex;
	}

	//function to update some element in list
	Node<T>* update(int index, T x)
	{
		if (index < 0) return NULL;
		int currIndex = 1;
		Node<T>* currNode = head;
		while (currNode && index > currIndex)
		{
			currNode = currNode->next;
			currIndex++;
		}
		if (index > 0 && currNode == NULL) return NULL;
		currNode->data = x;
		return currNode;
	}

	//function to remove some element in list
	int remove(T x)
	{
		Node<T>* prevNode = NULL;
		Node<T>* currNode = head;
		int currIndex = 1;
		while (currNode && currNode->data != x)
		{
			prevNode = currNode;
			currNode = currNode->next;
			currIndex++;
		}
		if (currNode)
		{
			if (prevNode)
			{
				prevNode->next = currNode->next;
				delete currNode;
			}
			else
			{
				head = currNode->next;
				delete currNode;
			}
			return currIndex;
		}
		return 0;
	}

	//function to display the list
	void print()
	{
		int num = 0;
		Node<T>* currNode = head;
		while (currNode != NULL)
		{
			cout << " -> " << currNode->name << "(" <<currNode->weight <<")";
			currNode = currNode->next;
			num++;
		}
	}
	//function to merge list
	void mergeLists(LinkedList<T> l2)
	{
		Node<T>* temp = l2.head;
		for (int i = 1; i <= 7; i += 2)
		{
			InsertAtIndex(temp->data, i);
			temp = temp->next;
		}
	}
};

//Min heap for dijkstra
//Min heap node structure
struct MinHeapNode
{
	int  v;
	int dist;
};

//Min heap structure
struct MinHeap
{
	//no. of nodes currently in minheap
	int size;

	//minheap capacity
	int capacity;

	int* pos;

	//minheap array
	MinHeapNode** array;
};

//creating a new min heap node
MinHeapNode* newMinHeapNode(int v, int dist)
{
	MinHeapNode* minHeapNode = new MinHeapNode;
	minHeapNode->v = v;
	minHeapNode->dist = dist;
	return minHeapNode;
}

//creaing a min heap
MinHeap* createMinHeap(int capacity)
{
	MinHeap* minHeap = new MinHeap;
	minHeap->pos = (int*)malloc(capacity * sizeof(int));
	minHeap->size = 0;
	minHeap->capacity = capacity;
	minHeap->array = (MinHeapNode**)malloc(capacity * sizeof(MinHeapNode*));
	return minHeap;
}

//function to swap two min heapnodes
void swapMinHeapNode(MinHeapNode** a, MinHeapNode** b)
{
	MinHeapNode* t = *a;
	*a = *b;
	*b = t;
}

//function to heapify at given index and update node positions
void minHeapify(MinHeap* minHeap, int idx)
{
	int smallest, left, right;
	smallest = idx;
	left = 2 * idx + 1;
	right = 2 * idx + 2;

	if (left < minHeap->size && minHeap->array[left]->dist < minHeap->array[smallest]->dist)
		smallest = left;

	if (right < minHeap->size && minHeap->array[right]->dist < minHeap->array[smallest]->dist)
		smallest = right;

	if (smallest != idx)
	{
		//the nodes to be swapped in min heap
		MinHeapNode* smallestNode = minHeap->array[smallest];
		MinHeapNode* idxNode = minHeap->array[idx];

		//swapping their positions
		minHeap->pos[smallestNode->v] = idx;
		minHeap->pos[idxNode->v] = smallest;

		//swapping the nodes
		swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);

		minHeapify(minHeap, smallest);
	}
}

//function to check if min heap is empty or not
int isEmpty(MinHeap* minHeap)
{
	return minHeap->size == 0;
}

//function to extract the minimum node from min heap
struct MinHeapNode* extractMin(MinHeap* minHeap)
{
	if (isEmpty(minHeap))
		return NULL;

	//storing the root node
	MinHeapNode* root = minHeap->array[0];

	//replacing it with the last node
	MinHeapNode* lastNode = minHeap->array[minHeap->size - 1];
	minHeap->array[0] = lastNode;

	//updating the position of last node
	minHeap->pos[root->v] = minHeap->size - 1;
	minHeap->pos[lastNode->v] = 0;

	//reducing the heap size and minheapifying
	--minHeap->size;
	minHeapify(minHeap, 0);

	return root;
}

//function to decreasy the distance value of given vertex
void decreaseKey(MinHeap* minHeap, int v, int dist)
{
	//finding the index of v in minheap
	int i = minHeap->pos[v];

	//updating node distance value
	minHeap->array[i]->dist = dist;

	//traveling up
	while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist)
	{
		//swapping the node with its parent
		minHeap->pos[minHeap->array[i]->v] = (i - 1) / 2;
		minHeap->pos[minHeap->array[(i - 1) / 2]->v] = i;
		swapMinHeapNode(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);

		//then moving to parent
		i = (i - 1) / 2;
	}
}

//function to check if given vertex is in min heap or not
bool isInMinHeap(MinHeap* minHeap, int v)
{
	if (minHeap->pos[v] < minHeap->size)
		return true;
	return false;
}

//max heap priority queue
//node structure
struct n
{
	int p;
	Message* info;
	n* l;
};

//priority queue class
class PriorityQueue
{
public:
	//front pointer
	n* f;

	//constructor
	PriorityQueue()
	{
		f = NULL;
	}

	//function to insert message in priority queue
	void insert(Message* i, int p)
	{
		n* t, * q;
		t = new n;
		t->info = i;
		t->p = t->info->priority;
		if (f == NULL || p > f->p)
		{
			t->l = f;
			f = t;
		}
		else
		{
			q = f;
			while (q->l != NULL && q->l->p >= p)
				q = q->l;
			t->l = q->l;
			q->l = t;
		}
	}

	//dequeue function for priority queue
	Message* deq()
	{
		n* t;

		//if empty
		if (f == NULL)
		{
			cout << "Priority Queue is empty\n";
			return NULL;
		}	
		//otherwise returning dequeued message
		else
		{
			t = f;
			f = f->l;
			return t->info;
		}
	}
};