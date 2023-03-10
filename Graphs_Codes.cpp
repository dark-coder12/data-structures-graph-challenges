#include <iostream>
#include <stack>
#include <queue>
#include <list>

using namespace std;

class MatrixGraph
{

};

class ListGraphUndirected
{
private:
	list<int>* adjList;
	int dimension;
public:
	ListGraphUndirected(int size)
	{
		dimension = size;
		adjList = new list<int>[dimension];
	}

	void AddEdge(int src, int dest)
	{
		adjList[src].push_back(dest);
		adjList[dest].push_back(src);
	}

	void PrintGraph()
	{
		for (int i = 0; i < dimension; i++)
		{
			cout << i;

			for (auto x : adjList[i])
			{
				cout << " -> " << x;
			}
			cout << endl;
		}
		cout << endl;
	}


	void CheckIfCycleExists()
	{
		bool cycleExists = false;

		vector<bool> visited;
		visited.resize(dimension, false);

		for (int i = 0; i < dimension; i++)
		{
			if (visited[i] == false)
			{
				//if (CycleCheckHelperDFS(visited, i, -1))

				if (CycleCheckHelperBFS(visited, i))
					cycleExists = true;
			}
		}

		if (cycleExists)
			cout << "Cycle exists in the graph." << endl;
		else
			cout << "Cycle does not exist in the graph." << endl;
	}

	bool CycleCheckHelperDFS(vector<bool>& visited, int current, int parent)
	{
		visited[current] = true;

		for (list<int>::iterator i = adjList[current].begin(); i != adjList[current].end(); i++)
		{
			if (visited[*i] == false)
			{
				return CycleCheckHelperDFS(visited, *i, current);
			}
			else
			{
				if (*i != parent)
					return true;
			}
		}
	}

	bool CycleCheckHelperBFS(vector<bool>& visited, int current)
	{
		vector<int> parents;
		parents.resize(dimension, -1);

		queue<int> nodeQueue;

		nodeQueue.push(current);
		visited[current] = true;

		while (!nodeQueue.empty())
		{
			int src = nodeQueue.front();
			nodeQueue.pop();

			for (auto x : adjList[src])
			{
				if (!visited[x])
				{
					visited[x] = true;
					parents[x] = src;
					nodeQueue.push(x);
				}
				else
					if (parents[src] != x)
						return true;

			}
		}
		return false;
	}

	bool CheckIfConnected(int src, int dest)
	{
		if (src > dimension || dest > dimension)
			return false;

		for (list<int>::iterator i = adjList[src].begin(); i != adjList[src].end(); i++)
		{
			if (*i == dest)
				return true;
		}
		return false;
	}
	void CountCycle(vector<bool>& visited, int& count, int current, int start, int k)
	{
		visited[current] = true;

		if (k == 0)
		{
			visited[current] = false;

			if (CheckIfConnected(current, start))
				count++;

			return;
		}

		for (int i = 0; i < dimension; i++)
		{
			if (!visited[i])
			{
				CountCycle(visited, count, i, start, k - 1);

			}
		}
		visited[current] = false;
	}


	void CountKLengthCycles(int k)
	{
		vector<bool> visited;
		visited.resize(dimension, false);

		int count = 0;

		for (int i = 0; i < dimension - (k - 1); i++)
		{
			CountCycle(visited, count, i, i, k - 1);

			visited[i] = true;
		}

		cout << "K length cycles are : " << count / 2 << endl;
	}
};


class ListGraphDirected
{
private:
	list<int>* adjList;
	int dimension;
public:
	ListGraphDirected(int size)
	{
		dimension = size;
		adjList = new list<int>[dimension];
	}

	void AddEdge(int src, int dest)
	{
		adjList[src].push_back(dest);
	}

	void PrintGraph()
	{
		for (int i = 0; i < dimension; i++)
		{
			cout << i;

			for (auto x : adjList[i])
			{
				cout << " -> " << x;
			}
			cout << endl;
		}
		cout << endl;
	}

	void DFS(vector<bool>& visited, int current)
	{
		visited[current] = true;

		for (list<int>::iterator x = adjList[current].begin(); x != adjList[current].end(); x++)
		{
			if (visited[*x] == false)
				DFS(visited, *x);
		}
	}

	void CheckIfCycleExists()
	{
		bool cycleExists = false;

		vector<bool> visited;
		visited.resize(dimension, false);

		vector<bool> ancestorStack;
		ancestorStack.resize(dimension, false);

		for (int i = 0; i < dimension; i++)
		{
			if (visited[i] == false)
			{
				/*if (CheckCycleHelperDFS(visited, ancestorStack, i))
					cycleExists = true;*/

				if (CheckCycleHelperBFS())
					cycleExists = true;
			}
		}

		if (cycleExists)
			cout << "Cycle exists in the graph." << endl << endl;
		else
			cout << "Cycle does not exist in the graph." << endl << endl;
	}


	bool CheckCycleHelperDFS(vector<bool>& visited, vector<bool>& ancestorStack, int current)
	{
		visited[current] = true;
		ancestorStack[current] = true;

		for (list<int>::iterator x = adjList[current].begin(); x != adjList[current].end(); x++)
		{
			if (!visited[*x])
			{
				if (CheckCycleHelperDFS(visited, ancestorStack, (*x)))
					return true;
			}
			else
				if (ancestorStack[*x])
					return true;
		}

		ancestorStack[current] = false;
		return false;
	}

	bool CheckCycleHelperBFS()
	{
		vector<int> inDegrees;
		inDegrees.resize(dimension, 0);

		for (int i = 0; i < dimension; i++)
		{
			for (list<int>::iterator x = adjList[i].begin(); x != adjList[i].end(); x++)
			{
				inDegrees[*x]++;
			}
		}

		queue<int> zeroInDegrees;

		for (int i = 0; i < dimension; i++)
		{
			if (inDegrees[i] == 0)
				zeroInDegrees.push(i);
		}

		int countNodes = 1;

		while (!zeroInDegrees.empty())
		{
			int node = zeroInDegrees.front();
			zeroInDegrees.pop();

			for (list<int>::iterator x = adjList[node].begin(); x != adjList[node].end(); x++)
			{
				if (--inDegrees[*x] == 0)
				{
					zeroInDegrees.push(*x);
					countNodes++;
				}
			}
		}
		if (countNodes != dimension)
			return true;
		return false;
	}

	ListGraphDirected Transpose()
	{
		ListGraphDirected graph(dimension);

		for (int i = 0; i < dimension; i++)
		{
			for (list<int>::iterator x = adjList[i].begin(); x != adjList[i].end(); x++)
			{
				graph.AddEdge(*x, i);
			}
		}

		return graph;
	}

	void IsGraphStronglyConnected()
	{
		vector<bool> visited;
		visited.resize(dimension, false);

		int start = 0;

		DFS(visited, start);

		for (int i = 0; i < dimension; i++)
		{
			if (!visited[i])
			{
				cout << "Graph is not strongly connected." << endl << endl;
				return;
			}
		}

		for (int i = 0; i < dimension; i++)
		{
			visited[i] = false;
		}

		ListGraphDirected T = Transpose();

		T.DFS(visited, start);

		for (int i = 0; i < dimension; i++)
		{
			if (!visited[i])
			{
				cout << "Graph is not strongly connected." << endl << endl;
				return;
			}
		}
		cout << "Graph is strongly connected." << endl << endl;
	}

	void FillOrderOfTraversal(vector<bool>& visited, int current, stack<int>& saveOrder)
	{
		visited[current] = true;

		for (list<int>::iterator x = adjList[current].begin(); x != adjList[current].end(); x++)
		{
			if (visited[*x] == false)
				FillOrderOfTraversal(visited, *x, saveOrder);
		}

		saveOrder.push(current);
	}

	void PrintThroughDFS(vector<bool>& visited, int current)
	{
		visited[current] = true;

		cout << current << " ";

		for (list<int>::iterator x = adjList[current].begin(); x != adjList[current].end(); x++)
		{
			if (visited[*x] == false)
				PrintThroughDFS(visited, *x);
		}
	}
	void KosarajuAlgo_PrintStronglyConnectedComponents()
	{
		cout << endl << endl << "Following are the strongly connected components : " << endl << endl;

		vector<bool> visited;
		visited.resize(dimension, false);

		stack<int> saveOrder;

		for (int i = 0; i < dimension; i++)
		{
			if (!visited[i])
				FillOrderOfTraversal(visited, i, saveOrder);
		}

		for (int i = 0; i < dimension; i++)
		{
			visited[i] = false;
		}

		ListGraphDirected T = Transpose();

		while (!saveOrder.empty())
		{
			int node = saveOrder.top();
			saveOrder.pop();

			for (list<int>::iterator x = adjList[node].begin(); x != adjList[node].end(); x++)
			{
				if (visited[*x] == false)
				{
					T.PrintThroughDFS(visited, *x);

					cout << endl;
				}
			}
		}
		cout << endl;
	}

	void CountNodesOnALevel(int start, int level)
	{
		int lev[100];

		vector<bool> visited;
		visited.resize(dimension, false);

		for (int i = 0; i < dimension; i++)
			lev[i] = 0;

		list<int> nodes;

		nodes.push_back(start);
		visited[start] = true;
		lev[start] = 0;

		while (!nodes.empty())
		{
			start = nodes.front();
			nodes.pop_front();

			for (list<int>::iterator x = adjList[start].begin(); x != adjList[start].end(); x++)
			{
				if (!visited[*x])
				{
					lev[*x] = lev[start] + 1;

					visited[*x] = true;

					nodes.push_back(*x);
				}
			}
		}

		int count = 0;

		for (int i = 0; i < dimension; i++)
		{
			if (lev[i] == level)
				count++;
		}

		cout << "Nodes on Level " << level << " are : " << count << endl << endl;
	}

	void CountAllPathsToDest(int src, int dest)
	{
		int count = 0;

		vector<bool> visited;

		visited.resize(dimension, false);

		CountAllPathsToDest(visited, src, dest, count);

		cout << "All paths from " << src << " to " << dest << " are " << count << endl;
	}

	void CountAllPathsToDest(vector<bool>& visited, int src, int dest, int& count)
	{
		visited[src] = true;

		if (src == dest)
		{
			count++;
		}
		else
		{
			for (list<int>::iterator i = adjList[src].begin(); i != adjList[src].end(); i++)
			{
				if (visited[*i] == false)
				{
					CountAllPathsToDest(visited, *i, dest, count);
				}
			}
		}
		visited[src] = false;
	}
};

class Node
{
private:
	int dest;
	int weight;
public:
	Node(int d, int w) : dest(d), weight(w) {}

	friend class WeightedGraph;

};

class WeightedGraph
{
private:
	list<Node>* adjList;
	int dimension;
public:

	WeightedGraph(int size)
	{
		dimension = size;
		adjList = new list<Node>[dimension];
	}

	void AddEdge(int src, int dest, int weight)
	{
		Node n(dest, weight);
		adjList[src].push_back(n);
	}

	void PrintGraph()
	{
		for (int i = 0; i < dimension; i++)
		{
			cout << i;

			for (auto x : adjList[i])
			{
				cout << " -> " << x.dest;
			}
			cout << endl;
		}
		cout << endl;
	}

	void ShortestPath(int src)
	{
		vector<int> distance;
		distance.resize(dimension);

		for (int i = 0; i < dimension; i++)
			distance[i] = 9999;

		distance[src] = 0;

		int index = 0;

		while (index < dimension)
		{
			for (list<Node>::iterator i = adjList[index].begin(); i != adjList[index].end(); i++)
			{
				if ((distance[index] + i->weight) < distance[i->dest])
				{
					distance[i->dest] = (distance[index] + i->weight);
				}
			}
			index++;
		}
		cout << "Shortest Path from 0 to 3 : " << endl << endl;

		int d = 3;

		for (int i = 0; i < dimension; i++)
		{
			if (i != src && i <= d)
				cout << src << " to " << i << ", Cost: " << distance[i] << endl << endl;
		}
	}

	int Find_MinDistance(vector<int>& dis, vector<bool>& vis)
	{
		int min = 9999;

		int minI = 0;

		for (int i = 0; i < dimension; i++)
		{
			if (dis[i] <= min && vis[i] == false)
			{
				min = dis[i], minI = i;
			}
		}

		return minI;
	}
	void DjikstraAlgorithm(int src)
	{
		vector<int> distance;
		distance.resize(dimension);

		for (int i = 0; i < dimension; i++)
			distance[i] = 9999;

		distance[src] = 0;

		int index = 0;

		vector<bool> visited;
		visited.resize(dimension, false);

		for (int i = 0; i < dimension; i++)
		{
			int u = Find_MinDistance(distance, visited);

			visited[u] = true;

			for (list<Node>::iterator x = adjList[u].begin(); x != adjList[u].end(); x++)
			{
				if (visited[x->dest] == false && distance[u] != 9999 && distance[x->dest] > (distance[u] + x->weight))
					distance[x->dest] = distance[u] + x->weight;
			}
		}

		for (int i = 0; i < dimension; i++)
		{
			cout << "Distance of " << src << " to " << i << " = " << distance[i] << endl << endl;
		}
	}
};

void main()
{
	cout << endl << "____________________________ Undirected Graphs ___________________________" << endl << endl;

	ListGraphUndirected listGraph(6);

	listGraph.AddEdge(0, 1);
	listGraph.AddEdge(0, 3);
	listGraph.AddEdge(1, 2);
	listGraph.AddEdge(2, 3);
	listGraph.AddEdge(3, 4);
	listGraph.AddEdge(4, 5);

	listGraph.PrintGraph();
	listGraph.CheckIfCycleExists();
	listGraph.CountKLengthCycles(3);

	cout << endl << "____________________________ Directed Graphs ___________________________" << endl << endl;

	ListGraphDirected listGraphD(8);

	listGraphD.AddEdge(0, 1);
	listGraphD.AddEdge(1, 2);
	listGraphD.AddEdge(0, 3);
	listGraphD.AddEdge(2, 3);
	listGraphD.AddEdge(3, 4);
	listGraphD.AddEdge(4, 7);
	listGraphD.AddEdge(4, 5);
	listGraphD.AddEdge(5, 6);

	// comment it to remove cycle
	listGraphD.AddEdge(7, 2);
	listGraphD.AddEdge(6, 5);

	listGraphD.PrintGraph();
	listGraphD.CheckIfCycleExists();
	listGraphD.CountNodesOnALevel(0, 2);
	listGraphD.CountAllPathsToDest(0, 4);

	listGraphD.KosarajuAlgo_PrintStronglyConnectedComponents();

	cout << endl << "__________________________ Weighted Directed Graphs _________________________" << endl << endl;

	WeightedGraph wGraph(5);

	wGraph.AddEdge(0, 1, 2);
	wGraph.AddEdge(0, 2, 5);
	wGraph.AddEdge(1, 3, 3);
	wGraph.AddEdge(2, 4, 6);
	wGraph.AddEdge(3, 4, 1);

	wGraph.PrintGraph();

	cout << endl << endl << "Normal yet efficient way to find Shortest Path" << endl << endl;

	wGraph.ShortestPath(0);

	cout << endl << endl << "Djikstra's Algorithm - Nothing Beats It (Nor Do I undestand it) " << endl << endl;

	wGraph.DjikstraAlgorithm(0);

	return;
}