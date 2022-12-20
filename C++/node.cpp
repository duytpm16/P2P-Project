#include "node.h"

void Node::add_node(int i, std::string line)
{
    index = i;

    std::istringstream iss(line);
    iss >> x >> y;
}



void Node::resize_vecs(int k, int n) 
{
    pcount.resize(k, 0);
    eucl_distance.resize(n, -1.0);
    cheb_distance.resize(n, -1.0);
}



void Node::create_edge(Node& v, int i) 
{
    edges.insert(v.index);
    vedges.push_back(v.index);
    pcount[i]++;

    v.edges.insert(index);
    v.vedges.push_back(index);
    v.pcount[i]++;

    return;
}



void Node::euclidean_distance(Node& v) 
{
    if (eucl_distance[v.index] < 0) 
    {
        double dist = sqrt(pow(v.x - x, 2) + pow(v.y - y, 2));
        eucl_distance[v.index] = dist;
        v.eucl_distance[index] = dist;
    }

    return;
}



void Node::chebyshev_distance(Node& v) 
{
    if (cheb_distance[v.index] < 0) 
    {
        double dist = std::max(std::fabs(v.x - x), std::fabs(v.y - y));
        cheb_distance[v.index] = dist;
        v.cheb_distance[index] = dist;
    }

    return;
}


// Create edges for each k phases
void Node::subgraphs(std::vector<Node>& nodes, std::vector<int> indices, int k, double r) 
{
    for (int i = 0; i < k; i++) 
    {
        int n = 0;
        double ri = pow(r, i);
        std::vector<int> nearby;
        std::vector<int> sample;
        for (auto& v : indices) 
        {
            if (index != v && edges.find(v) == edges.end() && nodes[v].pcount[i] < k && cheb_distance[v] < ri) 
            {
                nearby.push_back(v);
            }
        }

        for (auto& v : nearby) 
        {
            if (nodes[v].pcount[i] < k) 
            {
                sample.push_back(v);
                n++;
            }
        }
        if (n == 0) break;

        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(sample.begin(), sample.end(), g);
        
        int s = std::min((k - pcount[i]), n);
        for (int j = 0; j < s; j++) 
        {
            create_edge(nodes[sample[j]], i);
        }
        indices = nearby;
    }

    return;
}