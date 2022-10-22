#include "graph.h"

void Graph::construct_graph(double r) 
{
    std::ifstream file;
    file.open(filename);
    if (!file.is_open()) {
        std::cout << "Error: Cannot open input file.\n";
        exit(1);
    }

    std::string line;
    while (getline(file, line)) 
    {
        line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());

        Node u;
        u.add_node(n, line);
        nodes.push_back(u);
        indices.push_back(n);
        n++;
    }
    file.close();

    k = std::floor(std::log2(n));
    init_vectors();
    compute_euclidean_distances();
    compute_chebyshev_distances();
    construct_subgraphs(r);
}



void Graph::init_vectors()
{
    for (auto& u : nodes) 
    {
        u.resize_vecs(k, n);
    }
}



void Graph::compute_chebyshev_distances() 
{
    for (auto& u : nodes) 
    {
        for (auto& v : nodes) 
        {
            u.chebyshev_distance(v);
        }
    }
}



void Graph::compute_euclidean_distances() 
{
    for (auto& u : nodes) 
    {
        for (auto& v : nodes) 
        {
            u.euclidean_distance(v);
        }
    }
}



void Graph::construct_subgraphs(double r) 
{
    for (auto& u : nodes) 
    {
        u.subgraphs(nodes, indices, k, r);
    }
}
