#ifndef BNODE_H
#define BNODE_H

#include "p2p.h"

class BNode 
{
    public:
        int index;
        double x;
        double y;
        double quad_left;
        double quad_right;
        double quad_bottom;
        double quad_top;

        std::set<int> edges;
        std::vector<int> pcount;
        std::vector<double> cheb_distance;
        std::vector<std::vector<int>> vedges;
        std::vector<int> kappa_nodes;

        int top_node = -1;
        int left_node = -1;
        int bottom_node = -1;
        int right_node = -1;

        void add_node(int i, std::string line);
        void resize_vecs(int k, int n);
        void chebyshev_distance(BNode& v);
        void phase1_links(std::vector<BNode>& nodes);
        void phase2_links(std::vector<BNode>& nodes, int k);
        void phase3_links(std::vector<BNode>& nodes, int k, double r);
        void find_node_quadrant(std::vector<double>& lt, std::vector<double>& rt, std::vector<double>& bm, std::vector<double>& tp);
};



#endif