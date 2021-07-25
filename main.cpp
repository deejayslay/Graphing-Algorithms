#include "project3.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <locale.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <cstdio>
#include <random>
#include <numeric>
#include <algorithm>
#include <math.h>
#include "graph_algorithms.cpp"
#include "node.cpp"
#include "graph.cpp"


Graph erdosReyni(int numVertices, double p)
{
    int v = 1;
    int w=  -1;

    double r;
    int f;

    std::default_random_engine generator;

    std::vector<int> edges1;
    std::vector<int> edges2;

    while (v < numVertices)
    {
        std::uniform_real_distribution<double> distr(0.0, 1.0);
        r = distr(generator);
        f = floor( (log2(1 - r)) / (log(1 - p)) );
        w = ( w + 1 + f );
        while ( (w >= v) && (v < numVertices) )
        {
            w = ( w - v );
            ++v;
        }
        if (v < numVertices)
        {
            edges1.push_back(v);
            edges2.push_back(w);
        }
    }

    Graph g;
    g = make_graph(numVertices, edges1, edges2);
    return g;
}


// 
// d = number of edges per new node, minimum degree
Graph barabasiAlbert(int numVertices, int minDegree)
{
    std::default_random_engine generator;

    std::vector<int> bigM(2*numVertices*minDegree);
    int r;

    for (int v=0; v < numVertices; ++v)
    {
        for (int i=0; i < minDegree; ++i)
        {
            bigM[2*(v*minDegree + i)] = v;

            std::uniform_int_distribution<int> distr(0, 2*(v*minDegree + i));
            r = distr(generator);

            bigM[(2*(v*minDegree + i)) + 1] = bigM[r];
        }
    }

    std::vector<int> u;
    std::vector<int> v;

    int edge1, edge2;
    std::vector<int> adjacency1;

    std::map<int, std::vector<int>> bigEHashMap;

    for (int i=0; i < (numVertices*minDegree); ++i)
    {
        edge1 = bigM[2*i];
        edge2 = bigM[(2*i) + 1];

        if ( bigEHashMap.find(edge1) != bigEHashMap.end() )
        {
            adjacency1 = bigEHashMap[edge1];
            if (std::find(adjacency1.begin(), adjacency1.end(), edge2) == adjacency1.end())
            {
                u.push_back(edge1);
                v.push_back(edge2);

                bigEHashMap[edge1].push_back(edge2);
            }
        }
        else 
        {
            u.push_back(edge1);
            v.push_back(edge2);
            bigEHashMap[edge1].push_back(edge2);
        }
    }
    Graph g;
    g = make_graph(numVertices, u, v);
    return g;
}

void save_data(const std::string& dir_name, const std::string& file_name, unsigned input_size, double yValue)
{
	std::ofstream out;
	out.open("/Users/deejayslay/Desktop/CS165/Assignment3/data/" +dir_name + "/"+ file_name +".csv", std::ofstream::out | std::ofstream::app);
	out << input_size << ",";
	out <<  yValue << "\n";
	out.close();
}


// for Barabasi
void run_experimentBarabasi(unsigned min_input_size, unsigned input_size, unsigned num_runs)
{
	std::string dir_name = "barabasi";     // directory name of random graph

    std::string file_name1 = "diameter";
    std::string file_name2 = "coefficient";

	if(min_input_size == input_size)
    { 
		std::string file_temp1 = "/Users/deejayslay/Desktop/CS165/Assignment3/data/" + dir_name + "/"+ file_name1 + ".csv";
        std::string file_temp2 = "/Users/deejayslay/Desktop/CS165/Assignment3/data/" + dir_name + "/"+ file_name2 + ".csv";
		std::remove(file_temp1.c_str());
        std::remove(file_temp2.c_str());
	}


    Graph g;
    int diameter;
    float coefficient;

    double totalDiameter, totalCoefficient;

	for (unsigned i = 0; i < num_runs; ++i)
	{
        g = barabasiAlbert(input_size, 5);
		// call to functions
        diameter = get_diameter(g);
        coefficient = get_clustering_coefficient(g);
        totalDiameter += diameter;
        totalCoefficient += coefficient;
	}
	save_data(dir_name, file_name1, input_size, totalDiameter/(double) num_runs);
    save_data(dir_name, file_name2, input_size, totalCoefficient/(double) num_runs);
}


// for Erdos
void run_experimentErdos(unsigned min_input_size, unsigned input_size, unsigned num_runs)
{
	std::string dir_name = "erdos";     // directory name of random graph

    std::string file_name1 = "diameter";
    std::string file_name2 = "coefficient";

	if(min_input_size == input_size)
    { 
		std::string file_temp1 = "/Users/deejayslay/Desktop/CS165/Assignment3/data/" + dir_name + "/"+ file_name1 + ".csv";
        std::string file_temp2 = "/Users/deejayslay/Desktop/CS165/Assignment3/data/" + dir_name + "/"+ file_name2 + ".csv";
		std::remove(file_temp1.c_str());
        std::remove(file_temp2.c_str());
	}

    Graph g;
    int diameter;
    float coefficient;

    double p = ( (2 * (log(input_size))) / input_size );

    double totalDiameter, totalCoefficient;

	for (unsigned i = 0; i < num_runs; ++i)
	{
        g = erdosReyni(input_size, p);
		// call to functions
        diameter = get_diameter(g);
        coefficient = get_clustering_coefficient(g);
        totalDiameter += diameter;
        totalCoefficient += coefficient;
	}
	save_data(dir_name, file_name1, input_size, totalDiameter/(double) num_runs);
    save_data(dir_name, file_name2, input_size, totalCoefficient/(double) num_runs);
}



// for Degree Distribution
void run_experimentDegree(unsigned input_size, std::string sizeString)
{
	std::string dir_name1 = "barabasi";     // directory name of random graph
    std::string dir_name2 = "erdos";     // directory name of random graph

    std::string file_name1 = "degree";

    Graph g1;
    Graph g2;
    std::map<int, int> distr;

    double p = ( (2 * (log(input_size))) / input_size );

    g1 =  barabasiAlbert(input_size, 5);
    distr = get_degree_distribution(g1);

    for (auto const& v : distr)
    {
        save_data(dir_name1, file_name1 + sizeString, v.first, v.second);   
    }

    // erdos
    g2 = erdosReyni(input_size, p);
    distr = get_degree_distribution(g2);

    for (auto const& v : distr)
    {
        save_data(dir_name2, file_name1 + sizeString, v.first, v.second);   
    }
    
}


void run_experiments(unsigned min_input_size, unsigned max_input_size, double factor, unsigned num_runs)
{
	// for (unsigned input_size = min_input_size; input_size <= max_input_size; input_size *= factor)
	// {
	// 	run_experimentBarabasi(min_input_size, input_size, num_runs);
    //     run_experimentErdos(min_input_size, input_size, num_runs);
	// 	std::cout << "Experiments with input size "<< input_size <<" for "<< num_runs <<" runs"<<" are done" << std::endl;
	// }

    std::vector<int> nValues{1000, 10000, 100000};
    for (unsigned int i=0; i < nValues.size(); ++i)
    {
        run_experimentDegree(nValues[i], std::to_string(nValues[i]));
    }
}


int main()
{ 
	run_experiments(4, pow(2, 17), 2, 10);
	
	return 0;

}