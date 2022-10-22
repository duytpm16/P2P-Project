#include "bgraph.h"

void BGraph::construct_graph(double r, double xmin, double xmax, double ymin, double ymax, int l) 
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

        BNode u;
        u.add_node(n, line);
        nodes.push_back(u);
        indices.push_back(n);
        n++;
    }
    file.close();

    k = std::floor(std::log2(n));
 
    init_vectors();
    compute_chebyshev_distances();

    compute_quadrants(xmin, xmax, ymin, ymax, l);
    find_quadrant();

    std::cout << "Phase 1...";
    phase1();
    std::cout << "Complete.\n";

    std::cout << "Phase 2...";
    phase2();
    std::cout << "Complete.\n";

    std::cout << "Phase 2...";
    phase3(r);
    std::cout << "Complete.\n";
}



void BGraph::init_vectors()
{
    for (auto& u : nodes) 
    {
        u.resize_vecs(k, n);
    }
}


void BGraph::compute_chebyshev_distances() 
{
    for (auto& u : nodes) 
    {
        for (auto& v : nodes) 
        {
            u.chebyshev_distance(v);
        }
    }
}



void BGraph::compute_quadrants(double xmin, double xmax, double ymin, double ymax, int l)
{
    double xmid = (xmin + xmax) / 2.0;
    double ymid = (ymin + ymax) / 2.0;
    double xlen = std::fabs((xmax - xmin)) / 2.0;
    double ylen = std::fabs((ymax - ymin)) / 2.0;

    left   = {xmid, xmid - xlen, xmid - xlen, xmid};
    right  = {xmid + xlen, xmid, xmid, xmid  + xlen};
    bottom = {ymid, ymid, ymid - ylen, ymid - ylen};
    top    = {ymid + ylen, ymid + ylen, ymid, ymid};
    
    if (l > 1) 
    {
        int c = 0;
        int m = 0;
        for (int i = 1; i < l; i++) 
        {
            m += std::pow(4, i);
        }

        while (c < m) 
        {
            xmid = (left[0] + right[0]) / 2.0;
            ymid = (bottom[0] + top[0]) / 2.0;
            xlen = std::fabs((right[0] - left[0])) / 2.0;
            ylen = std::fabs((bottom[0] - top[0])) / 2.0;

            std::vector<double> lt = {xmid, xmid - xlen, xmid - xlen, xmid};
            std::vector<double> rt = {xmid + xlen, xmid, xmid, xmid  + xlen};
            std::vector<double> bm = {ymid, ymid, ymid - ylen, ymid - ylen};
            std::vector<double> tp = {ymid + ylen, ymid + ylen, ymid, ymid};

            left.insert(left.end(), lt.begin(), lt.end());
            right.insert(right.end(), rt.begin(), rt.end());
            bottom.insert(bottom.end(), bm.begin(), bm.end());
            top.insert(top.end(), tp.begin(), tp.end());

            left.erase(left.begin());
            right.erase(right.begin());
            bottom.erase(bottom.begin());
            top.erase(top.begin());

            c++;
        }
    }

    return;
}


void BGraph::find_quadrant()
{  
    for (auto& u : nodes) 
    {
        u.find_node_quadrant(left, right, bottom, top);
    }

    return;
}

void BGraph::phase1() 
{
    for (auto& u : nodes) 
    {
        u.phase1_links(nodes);
    }

    return;
}

void BGraph::phase2() 
{
    for (auto& u : nodes) 
    {
        u.phase2_links(nodes, k);
    }

    return;
}

void BGraph::phase3(double r) 
{
    for (auto& u : nodes) 
    {
        u.phase3_links(nodes, k, r);
    }

    return;
}
