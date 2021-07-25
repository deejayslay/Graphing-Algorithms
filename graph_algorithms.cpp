#include "graph.h"
#include <iostream>
#include <map>
#include <list>
#include <algorithm>
#include <deque>
#include <vector>
#include <random>

int get_diameter(Graph graph)
{
	int dMax = 0;
	std::pair<int, int> furthest;

	auto it = graph.adj.begin();
	std::advance(it, rand() % graph.adj.size());
	int r = it->first;
	while (true)
	{
		furthest = graph.BFS(r);
		if (furthest.first > dMax)
		{
			dMax = furthest.first;
			r = furthest.second;
		}
		else 
		{
			break;
		}
	}
	return dMax;
}

float get_clustering_coefficient(Graph graph) 
{
	// linear algorithm
	// std::cout << r;
	// std::cout << ", ";
	// std::cout << furthest.first << std::endl;
	// degeneration
	std::deque<int> l;
	std::map<int, bool> hashL;
	std::map<int, int> d;
	std::map<int, std::list<Node>> bigD;
	std::map<int, std::list<int>> bigN;
	for (auto const& x : graph.adj)
	{
		d[x.first] = x.second.size();
		bigD[x.second.size()].push_back(Node(x.first));
		hashL[x.first] = false;
	}

	int k = 0;
	int smallestIndex;
	Node v;
	
	for (unsigned int i=0; i < graph.n; ++i)
	{
		// find smallest index in bigD
		for (unsigned int j=0; j < bigD.size(); ++j)
		{
			if (bigD[j].size() != 0)    // if nonempty
			{
				smallestIndex = j;
				break;
			}
		}

		// set k
		if (smallestIndex > k)
		{
			k = smallestIndex;
		}

		// select vertex v
		v = bigD[smallestIndex].front();


		// mark v as being in L
		hashL[v.id] = true;

			// if v not in L
		l.push_front(v.id);
		bigD[smallestIndex].remove(v);
		// for each neighbor w of v
		for (auto const& w : graph.adj[v.id])
		{
			// if w not in L
			if (hashL[w.id] == false)
			{
				d[w.id] -= 1;
				bigD[d[w.id]].push_back(Node(w));
				bigN[v.id].push_back(w.id);
			}
		}
	}

	// triangle counting 
	int triangleCt, vertex;
	std::list<Node> adjacency;
	triangleCt = 0;

	std::list<Node>::iterator u;
	std::list<Node>::iterator w;

	for (int i=l.size()-1; i >= 0; --i)
	{
		vertex = l[i];
		adjacency = graph.adj[vertex];
		// for each pair of vertices, u and w
		for (auto u = adjacency.begin(); u != adjacency.end(); ++u)
		{
			auto w = u;
			++w;
			for (; w != adjacency.end(); ++w)
			{
				if (std::find(bigN[vertex].begin(), bigN[vertex].end(), u->id) != bigN[vertex].end())
				{
					if (std::find(bigN[vertex].begin(), bigN[vertex].end(), w->id) != bigN[vertex].end())
					{
						// if (u, w) an edge in the graph
						if (std::find(graph.adj[u->id].begin(), graph.adj[u->id].end(), Node(w->id)) != graph.adj[u->id].end())
						{
							++triangleCt;
						}
					}
				}
			}
		}
	}

	// compute denominator
	int denom, lengthTwoPaths;
	denom = 0;
	for (auto const& v : graph.adj)
	{
		lengthTwoPaths = (v.second.size() * (v.second.size() - 1))/2;
		denom += lengthTwoPaths;
	}

	int numerator = (3 * triangleCt);

	float bigC = ((float)numerator/(float)denom);
	return bigC;

	// O(n^3) algorithm

	// int triangleCt, maxEdges, hashIndex, hashIndex1, hashIndex2;

	// maxEdges = (graph.n*(graph.n-1))/2;

	// std::vector<int> hashTable[maxEdges];

	// for (auto const& v : graph.adj)
	// {
	// 	for (auto const& w : v.second)
	// 	{
	// 		hashIndex = (v.first % maxEdges);
	// 		hashTable[hashIndex].push_back(w.id);
	// 	}
	// }

	// std::vector<int> mappings;

	// triangleCt = 0;

	// for (auto u = graph.adj.begin(); u != graph.adj.end(); ++u)
	// {
	// 	auto v = u;
	// 	++v;
	// 	for (; v != graph.adj.end(); ++v)
	// 	{
	// 		auto w = v;
	// 		++w;
	// 		for (; w != graph.adj.end(); ++w)
	// 		{
	// 			// check (u, v)
	// 			hashIndex = (u->first % maxEdges);
	// 	        mappings = hashTable[hashIndex];
	// 			if( std::find(mappings.begin(), mappings.end(), v->first) != mappings.end())
	// 			{
	// 				// check (u, w)
	// 	        	mappings = hashTable[hashIndex];
	// 				if( std::find(mappings.begin(), mappings.end(), w->first) != mappings.end())
	// 				{
	// 					// check (u, v)
	// 					hashIndex = (v->first % maxEdges);
	// 					mappings = hashTable[hashIndex];
	// 					if( std::find(mappings.begin(), mappings.end(), w->first) != mappings.end())
	// 					{
	// 						++triangleCt;
	// 					}
	// 				}

	// 			}
	// 		}
	// 	}
	// }

	// int denom, lengthTwoPaths;
	// denom = 0;
	// for (auto const& v : graph.adj)
	// {
	// 	lengthTwoPaths = (v.second.size() * (v.second.size() - 1))/2;
	// 	denom += lengthTwoPaths;
	// }

	// int numerator = (3 * triangleCt);

	// float bigC = ((float)numerator/(float)denom);

	// return bigC;	
}


std::map<int, int> get_degree_distribution(Graph graph) 
{
	std::map<int, int> answer;
	for (auto const& v : graph.adj)
	{
		answer[v.second.size()] = 0;
	}

	for (auto const& v : graph.adj)
	{
		answer[v.second.size()] += 1;
	}
	
	return answer;
}
