#ifndef BGRAPH_H
#define BGRAPH_H

#include "p2p.h"
#include "bnode.h"

class BGraph
{
    public:
        BGraph(std::string fn) : filename(fn) {}

        int n = 0;
        int k = 0;
        std::string filename;
        std::vector<int> indices;
        std::vector<BNode> nodes;

        std::vector<double> left;
        std::vector<double> right;
        std::vector<double> bottom;
        std::vector<double> top;

        void construct_graph(double r, double xmin, double xmax, double ymin, double ymax, int l);

    private:
        void init_vectors();
        void phase1();
        void phase2();
        void phase3(double r);
        void find_quadrant();
        void compute_chebyshev_distances();
        void compute_quadrants(double xmin, double xmax, double ymin, double ymax, int l);
};

#endif