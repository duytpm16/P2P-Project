#ifndef NODE_H
#define NODE_H

#include "p2p.h"

class Node 
{
    public:
        int index;
        double x;
        double y;
        std::set<int> edges;
        std::vector<int> vedges;
        std::vector<int> pcount;
        std::vector<double> eucl_distance;
        std::vector<double> cheb_distance;

        void add_node(int i, std::string line);
        void resize_vecs(int k, int n);
        void create_edge(Node& v, int i);
        void chebyshev_distance(Node& v);
        void euclidean_distance(Node& v);
        void subgraphs(std::vector<Node>& nodes, std::vector<int> indices, int k, double r);
};

#endif