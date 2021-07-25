#include "graph.h"
#include <list>
#include <iostream>

Graph::Graph() 
	: n{0}, m{0}
{
}

Graph make_graph(int num_nodes, std::vector<int> u, std::vector<int> v){
	Graph g;
	g.n = num_nodes;
	g.m = u.size();
	for (unsigned int i=0; i < u.size(); ++i)
	{
		g.mapping[u[i]] = Node(u[i]);
		g.mapping[v[i]] = Node(v[i]);
	}
	if (g.m != 0)
	{
		for (unsigned int i=0; i < u.size(); ++i)
		{
			g.adj[u[i]].push_back(Node(v[i]));
			g.adj[v[i]].push_back(Node(u[i]));
		}
	}
	return g; // remove this line if you implement the function
}


std::pair<int, int> Graph::BFS(int v)
{
	std::pair<int, int> answer;
	
    int dis[n+1];
	for (int i=0; i < (n + 1); ++i)
	{
    	dis[i] = -1;
	}
 
    std::list<int> queue;
	queue.push_back(v);

	dis[v] = 0;
    std::list<Node>::iterator i;
	int current;
 
    while(!queue.empty())
    {
		current = queue.front();
        queue.pop_front();

		for (i = adj[current].begin(); i != adj[current].end(); ++i)
		{
			if (dis[i->id] == -1)
            {

                dis[i->id] = dis[current] + 1;
				queue.push_back(i->id);
			}
		}
	}

	answer.first = 0;
    for (int i = 1; i <= n; i++)
    {
        if (dis[i] > answer.first)
        {
            answer.first = dis[i];
            answer.second = i;
        }
    }
	return answer;

}

int Graph::get_num_nodes() {
	return n;
}

int Graph::get_num_edges() {
	return m;
}

std::vector<Node> Graph::get_neighbors(Node u) {
	std::vector<Node> answer{};
	if (m != 0)
	{
		for (auto const& i : adj[u.id])
		{
			answer.push_back(i);		
		}
	}
	
	return answer;
	
}

std::map<int, Node>Graph::get_id_to_node_map(){
	return mapping;
}

