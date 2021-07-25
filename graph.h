#ifndef GRAPH_H
#define GRAPH_H
#include <map>
#include <vector>
#include "node.h"
#include <list>

class Graph 
{
	public:
		// DO NOT MODIFY MEMBERS BELOW THIS LINE
        int get_num_nodes(); 						// get number of nodes
        int get_num_edges(); 						// get number of edges
        std::vector<Node> get_neighbors(Node u); 	// return neighbors of u 
        std::map<int, Node> get_id_to_node_map(); 	// allows lookup of nodes from ids
		// DO NOT MODIFY MEMBERS ABOVE THIS LINE
		
		// declare any constructors, members, and member functions you may want to use
		
		// implement any newly declared member functions in graph.cpp
		Graph();

		std::pair<int, int> BFS(int v);

		int n;
		int m;
		std::map<int, Node> mapping{};
		std::map<int, std::list<Node>> adj; 
};

#endif
