/*
20i-0436
Shayan Faisal
CS-A
Data Structures Project
*/

#include "Header.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <windows.h>
using namespace std;

//function to convert uppercase characters in string to lowercase
void convertToLowercase(string& text)
{
    int i = 0;
    while (i < text.length())
    {
        if (isupper(text[i]))
        {
            text[i] = tolower(text[i]);
        }
        i++;
    }
}

//function to convert lowercase characters in string to uppercase
void convertToUppercase(string& text)
{
    int i = 0;
    while (i < text.length())
    {
        if (islower(text[i]))
        {
            text[i] = toupper(text[i]);
        }
        i++;
    }
}

//function that separates string into an array of words
string* separateIntoWords(string str, char separator)
{
    string* words = new string[50];
    for (int i = 0; i < 50; i++)
    {
        words[i] = "";
    }
    int x = 0;
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] == separator)
            x++;
        else
            words[x] += str[i];
    }
    return words;
}

//Graph class
class Graph
{
public:
    int nodes;
    string* nodeNames;
    LinkedList<int>* adjlist;

    //constructors
    Graph()
    {
        nodes = 0;
        nodeNames = NULL;
    }
    Graph(int nodes)
    {
        adjlist = new LinkedList<int>[nodes];
        this->nodes = nodes;
        nodeNames = NULL;
    }

    //destructor
    ~Graph()
    {
        delete [] adjlist;
    }

    //function to read CSV file and make graph
    bool ReadCSV(string filename)
    {
        ifstream inputFile;
        inputFile.open(filename);
        if (inputFile.is_open())
        {
            string temp;
            int lineCount = 0;
            while(!inputFile.eof())
            {
                getline(inputFile, temp);
                //reading first line
                if (lineCount == 0)
                {
                    if (temp[0] != ',')
                    {
                        cout << "File format not correct!" << endl;
                        return false;
                    }
                        
                    nodeNames = separateIntoWords(temp, ',');
                    //finding number of nodes
                    int i = 1;
                    while (nodeNames[i] != "")
                        i++;
                    nodes = i-1;
                    adjlist = new LinkedList<int>[nodes];
                    this->nodes = nodes;
                }
                else
                {
                    //basically adding the edges to adjacency list
                    string* words = separateIntoWords(temp, ',');
                    int src = lineCount - 1;
                    for (int i = 0; words[i] != ""; i++)
                    {
                        if (i > 0 && words[i] != "?")
                        {
                            AddEdge(src, i - 1, nodeNames[i], stoi(words[i]));
                        }  
                    }
                }
                lineCount++;
            }
            return true;
        }
        else
        {
            cout << "File not found!" << endl;
            return false;
        }
    }

    //function to send message from file
    bool SendMessage(string filename)
    {
        //if no graph has been made yet
        if (adjlist == NULL)
            return false;

        //opening file
        ifstream inputFile;
        inputFile.open(filename);
        if (inputFile.is_open())
        {
            string temp;
            PriorityQueue p;
            while (!inputFile.eof())
            {
                //reading file line by line
                getline(inputFile, temp);

                //separating contents of line based on : (colon)
                string* words = separateIntoWords(temp, ':');

                int i = 0;
                //finding number of words
                while (words[i] != "")
                {
                    i++;
                }

                //if there are less than 5 entries in this line then it's not a valid message
                if (i < 5)
                {
                    cout << "Invalid message!" << endl << endl;
                    continue;
                }
                //otherwsie create a message object and insert into max heap priority queue
                else
                {

                    Message* m = new Message(stoi(words[0]), stoi(words[1]), words[2], words[3], words[4], "");
                    p.insert(m, m->priority);
                }
            }

            //while priority queue is not empty
            while(p.f != NULL)
            {
                //dequeue from priority queue to get message
                Message* m = p.deq();
                cout << "--Sending message FROM " << m->src_address << " TO " << m->dest_address << endl;
                cout << "Message ID: " << m->id << endl;
                cout << "Message Priority: " << m->priority << endl;
                cout << "Message Payload: " << m->payload << endl;
                cout << endl;

                //sending the message
                string src = m->src_address;
                Node<int>* currNode = locateNode(src);
                Message* message = m;
                do
                {
                    string destQueue = currNode->routingTable->findQueueByDest(message->dest_address, nodes);
                    cout << "*FORWARDING TO  " << destQueue << endl;
                    src = destQueue;

                    //enqueuing it to outgoing queue of current node
                    currNode->outgoingQueue.enQueue(message);
                    Node<int>* nextNode = locateNode(destQueue);

                    //dequeuing from outgoing queue of current node
                    message = currNode->outgoingQueue.deQueue();

                    //network delay
                    Sleep(1000);

                    //enqueueing to incoming queue of next router
                    nextNode->incomingQueue.enQueue(message);

                    currNode = nextNode;

                }
                while (src != m->dest_address);

                //once message reaches it's destination then repeat for next message in priority queue
                cout << "Message has successfully reached it's destination!" << endl << endl;
                Sleep(1000);
            }

            return true;
        }
        //if file wasn't found
        else
        {
            cout << "File not found!" << endl;
            return false;
        }
    }

    //function to add an edge between nodes in adjacency list
    void AddEdge (int src, int dst, string n, int w)
    {
        adjlist[src].insert(dst, n, w);
    }

    //function to display the adjacency list representation of created graph
    void Display()
    {
        cout << "\n--Adjacency List: " << endl;
        for (int i = 0; i < nodes; i++)
        {
            cout << setw(3) << nodeNames[i+1] << ":";
            adjlist[i].print();
            cout << endl;
        }
    }

    //function to find shortest paths using the Dijkstra algorithm
    int* dijkstra(int src, int* &parents)
    {
        //number of nodes in graph
        int numNodes = nodes;

        //parent array to store shortest path tree
        parents = new int[numNodes];

        //distance values array for minimum weighted edge
        int* distances = new int[numNodes];

        //creating minheap
        MinHeap* minHeap = createMinHeap(numNodes);

        //minheap initialization with all node distance values
        for (int i = 0; i < numNodes; i++)
        {
            parents[i] = -1;
            distances[i] = INT_MAX;
            minHeap->array[i] = newMinHeapNode(i, distances[i]);
            minHeap->pos[i] = i;
        }

        //since we'll start from source, so make its distance as zero to extract it first
        minHeap->array[src] = newMinHeapNode(src, distances[src]);
        minHeap->pos[src] = src;
        distances[src] = 0;
        decreaseKey(minHeap, src, distances[src]);

        //minheap size is equal to total number of nodes
        minHeap->size = numNodes;

        //minheap contains all nodes whose shortest distance is not yet found
        while (!isEmpty(minHeap))
        {
            //extracting minimum distance node
            MinHeapNode* minHeapNode = extractMin(minHeap);

            //storing this extracted node number
            int u = minHeapNode->v;

            //traversing through the adjacent nodes of extracted vertex and updating distance values
            Node<int>* currNode = adjlist[u].head;
            while (currNode != NULL)
            {
                int v = currNode->data;

                //if distance from one node to other is less than previously calculated one
                if (isInMinHeap(minHeap, v) && distances[u] != INT_MAX && currNode->weight + distances[u] < distances[v])
                {
                    //updating the distance and parent array (for path)
                    parents[v] = u;
                    distances[v] = distances[u] + currNode->weight;

                    //also updating distance in minheap
                    decreaseKey(minHeap, v, distances[v]);
                }
                currNode = currNode->next;
            }
        }

        //returning the distances array
        return distances;
    }

    //function to locate and return node with given name in the graph
    Node<int>* locateNode(string v)
    {
        Node<int>* currNode;
        for (int i = 0; i < nodes; i++)
        {
            currNode = adjlist[i].head;
            while (currNode)
            {
                if (currNode->name == v)
                    return currNode;
                else
                    currNode = currNode->next;
            }
        }
        cout << "Node not found!" << endl;
        return NULL;
    }

    //function to locate and return node with given data in the graph
    Node<int>* locateNodeByData(int v)
    {
        Node<int>* currNode;
        for (int i = 0; i < nodes; i++)
        {
            currNode = adjlist[i].head;
            while (currNode)
            {
                if (currNode->data == v)
                    return currNode;
                else
                    currNode = currNode->next;
            }
        }
        cout << "Node not found!" << endl;
        return NULL;
    }

    //recursive function to print the shortest path
    void printPath(int parent[], int j)
    {
        if (parent[j] == -1)
            return;

        printPath(parent, parent[j]);

        cout << " -> " << nodeNames[j + 1];
    }

    //recursive function to get the first parent (for use in routing tables)
    void getFirstParent(int parent[], int j, int& p)
    {
        if (parent[j] == -1)
            return;

        p = j;

        getFirstParent(parent, parent[j], p);
    }

    //function to display routing tables of each router
    void printRoutingTables()
    {
        for (int i = 0; i < nodes; i++)
        {
            Node<int>* node = locateNodeByData(i);

            //if it's a machine then skip it
            if (node->name[0] == 'M')
                continue;

            cout << node->name << "'s Routing table: " << endl;
            cout << "Destination\tOutgoing Queue" << endl;
            for (int j = 0; j < nodes; j++)
                if (node->routingTable->rPairs[j].dest != "")
                    cout << setw(3) << node->routingTable->rPairs[j].dest << "\t\t" << node->routingTable->rPairs[j].outgoingQueue << endl;
            cout << endl;
        }
    }

    //function to print routing table of a specific router
    void printRoutingTable(string rt)
    {
        Node<int>* node = locateNode(rt);
        cout << node->name << "'s Routing table: " << endl;
        cout << "Destination\tOutgoing Queue" << endl;
        for (int j = 0; j < nodes; j++)
            if (node->routingTable->rPairs[j].dest != "")
                cout << setw(3) << node->routingTable->rPairs[j].dest << "\t\t" << node->routingTable->rPairs[j].outgoingQueue << endl;
        cout << endl;
    }
};

//function to display help commands for the command line interface
void displayHelp()
{
	cout<<endl;
	cout<<"--Help commands: "<<endl;
    cout<<"* Create graph from CSV file: create graph 'filename.csv' (e.g create graph network.csv)" << endl;
    cout<<"* Display graph: display graph"<<endl;
	cout<<"* Send message: send msg 'filename.txt' (e.g send msg hello.txt)"<<endl;
    cout<<"* Print routing table: print rt 'rX' (e.g print rt r6)" << endl;
	cout<<"* Print path: print path 'src' to 'dest' (e.g print path M6 to *)"<<endl;
	cout<<"* Display helping commands: help"<<endl;
	cout<<"* Exit program: exit"<<endl;
	cout<<endl;
}

//command line interface function for controlling the program
void commandLine()
{
    Graph g;;
	string command;
	displayHelp();
	do
	{
		cout<<"TheNetworkEmulator\\>";
		getline(cin, command);
		convertToLowercase(command);
		string* words = separateIntoWords(command, ' ');

		//checking commands
        //exit
		if (words[0] == "exit")
			break;

        //help
		else if (words[0] == "help")
			displayHelp();

        //create graph
        else if (words[0] == "create" && words[1] == "graph")
        {
            if (words[2] != "")
            {
                //if successfully graph made by csv
                if (g.ReadCSV(words[2]))
                {
                    g.Display();

                    //routing table updating by applying digkstra to each node
                    for (int i = 0; i < g.nodes; i++)
                    {
                        int* parents = new int[g.nodes];
                        int* distances = g.dijkstra(i, parents);
                        Node<int>* node = g.locateNodeByData(i);
                        node->routingTable->rPairs = new RoutingPairs[g.nodes];
                        for (int j = 0; j < g.nodes-1; j++)
                        {
                            if (g.nodeNames[j+1][0] != 'R')
                            {
                                int p = 0;
                                g.getFirstParent(parents, j, p);
                                node->routingTable->rPairs[j].dest = g.nodeNames[j + 1];
                                node->routingTable->rPairs[j].outgoingQueue = g.nodeNames[p + 1];    
                            }
                        }
                        delete parents;
                    }    

                    cout << "\nGraph created from "<< words[2] << " successfully!" << endl;
                }
            }
            else
                cout << "Filename not specified!" << endl;
        }

        //display graph
        else if (words[0] == "display" && words[1] == "graph")
        {
            if (g.adjlist != NULL)
            {
                g.Display();
                cout << endl;
            }
            else
                cout << "Graph doesn't exist!" << endl;
        }

        //print path
        else if (words[0] == "print" && words[1] == "path" && words[3] == "to")
        {
            //print path from all nodes to a specific node
            if (words[2] == "*" && words[4] != "")
            {
                convertToUppercase(words[4]);
                
                Node<int>* destNode = g.locateNode(words[4]);
                
                if (destNode != NULL)
                {
                    for (int i = 0; i < g.nodes; i++)
                    {
                        Node<int>* srcNode = g.locateNodeByData(i);
                        int* parents = new int[g.nodes];
                        int* distances = g.dijkstra(srcNode->data, parents);
                        cout << "\n-" << srcNode->name << " to " << words[4] << endl;
                        cout << "Distance: "<< distances[destNode->data] << endl;
                        cout << "Path: ";
                        cout << g.nodeNames[srcNode->data + 1];
                        g.printPath(parents, destNode->data);
                        cout << endl;
                    }
                    cout << endl;
                }
                else
                    cout << "Nodes not specified!" << endl;
            }

            //print path from a specific node to all nodes
            else if (words[2] != "" && words[4] == "*")
            {
                convertToUppercase(words[2]);
                Node<int>* node = g.locateNode(words[2]);

                if (node != NULL)
                {
                    int* parents = new int[g.nodes];
                    int* distances = g.dijkstra(node->data, parents);

                    //displaying these shortest distances
                    cout << "\nVertex\tDistance from " << g.nodeNames[node->data + 1] << "\tPath" << endl;
                    for (int i = 0; i < g.nodes; ++i)
                    {
                        cout << setw(3) << g.nodeNames[i + 1] << "\t\t" << distances[i] << "\t\t";
                        cout << g.nodeNames[node->data + 1];
                        g.printPath(parents, i);
                        cout << endl;
                    }
                    cout << endl;
                }
                else
                    cout << "Nodes not specified!" << endl;
            }

            //print path from one node to another node
            else if (words[2] != "" && words[4] != "")
            {
                convertToUppercase(words[2]);
                convertToUppercase(words[4]);
                Node<int>* srcNode = g.locateNode(words[2]);
                Node<int>* destNode = g.locateNode(words[4]);
                if (srcNode != NULL && destNode != NULL)
                {
                    int* parents = new int[g.nodes];
                    int* distances = g.dijkstra(srcNode->data, parents);
                    cout << "\n-" << srcNode->name << " to " << words[4] << endl;
                    cout << "Distance: " << distances[destNode->data] << endl;
                    cout << "Path: ";
                    cout << g.nodeNames[srcNode->data + 1];
                    g.printPath(parents, destNode->data);
                    cout << endl << endl;
                }
                else
                    cout << "Nodes not specified!" << endl;
            }      
        }

        //send message
        else if (words[0] == "send" && words[1] == "msg")
        {
            if (words[2] != "")
            {
                g.SendMessage(words[2]);
            }
            else
                cout << "Messsage not specified!" << endl;
        }

        //print rt
        else if (words[0] == "print" && words[1] == "rt")
        {
            if (words[2] != "")
            {
                convertToUppercase(words[2]);
                cout << endl;
                if (words[2] == "*")
                    g.printRoutingTables();
                else
                    g.printRoutingTable(words[2]);
            }
            else
                cout << "Router not specified!" << endl;
        }

        //unrecognizable command
		else
		{
			cout<<"Command not recognized!"<<endl;
			cout<<"Use 'help' to display a list of commands."<<endl;
		}
	}
	while (command != "exit");

	cout<<"Program exited."<<endl;
}

int main()
{
	system("CLS");
	cout<<"\n*---THE NETWORK EMULATOR---*\n";

	//starting command line
	commandLine();

	return 0;
}