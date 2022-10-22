#ifndef GRAPH_H
#define GRAPH_H

#include "p2p.h"
#include "node.h"

class Graph
{
    public:
        Graph(std::string fn) : filename(fn) {}

        int n = 0;
        int k = 0;
        std::string filename;
        std::vector<int> indices;
        std::vector<Node> nodes;

        void construct_graph(double r);

        void route(Node& u, Node& v, int* add_h, double* add_p, std::vector<int>* add_path);
        void greedy_route(int start, int end, int t, std::string outfile);
        void greedy_routing(int t);

    private:
        void init_vectors();
        void construct_subgraphs(double r);
        void compute_chebyshev_distances();
        void compute_euclidean_distances();
        int closest(std::vector<int>& edges, std::vector<double> distances, std::vector<int> visited); 
};

#endif