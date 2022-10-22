#include "bnode.h"

void BNode::add_node(int i, std::string line)
{
    index = i;

    std::istringstream iss(line);
    iss >> x >> y; 

    return;
}


void BNode::resize_vecs(int k, int n) 
{
    pcount.resize(k, 0);
    cheb_distance.resize(n, -1.0);
    vedges.resize(std::max(k-2, 0));

    return;
}


void BNode::chebyshev_distance(BNode& v) 
{
    if (cheb_distance[v.index] < 0) 
    {
        double dist = std::max(std::fabs(v.x - x), std::fabs(v.y - y));
        cheb_distance[v.index] = dist;
        v.cheb_distance[index] = dist;
    } 

    return;
}

void BNode::find_node_quadrant(std::vector<double>& lt, std::vector<double>& rt, std::vector<double>& bm, std::vector<double>& tp)
{  
    for (size_t i = 0; i < lt.size(); i++) 
    {
        if (x > lt[i] && x < rt[i] && y > bm[i] && y < tp[i])
        {
            quad_left   = lt[i];
            quad_right  = rt[i];
            quad_bottom = bm[i]; 
            quad_top    = tp[i];
        }  
    }
}


void BNode::phase1_links(std::vector<BNode>& nodes) 
{
    double xlen = (quad_left + quad_right) / 2.0;
    double ylen = (quad_bottom + quad_top) / 2.0;

    // Left nodes
    std::vector<BNode> temp1;
    std::vector<int> left_nodes;
    for (auto& v : nodes) 
    {
        int vi = v.index;
        if (vi != index && v.right_node == -1 && v.x > (quad_left - xlen) && v.x < quad_left && v.y > quad_bottom && v.y < quad_top)
        {
           left_nodes.push_back(vi);
        }
        else
        {
            temp1.push_back(v);
        }
    } 

    if (left_nodes.size() > 0) 
    {
        std::random_device rd1;
        std::mt19937 g1(rd1());
        std::shuffle(left_nodes.begin(), left_nodes.end(), g1);
        left_node = left_nodes[0];
        nodes[left_nodes[0]].right_node = index;
    }



    // Right nodes
    std::vector<BNode> temp2;
    std::vector<int> right_nodes;
    for (auto& v : temp1) 
    {
        int vi = v.index;
        if (vi != index && v.left_node == -1 && v.x > quad_right && v.x < (quad_right + xlen) && v.y > quad_bottom && v.y < quad_top)
        {
            right_nodes.push_back(v.index);
        }
        else
        {
            temp2.push_back(v);
        }
    }

    if (right_nodes.size() > 0) 
    {
        std::random_device rd2;
        std::mt19937 g2(rd2());
        std::shuffle(right_nodes.begin(), right_nodes.end(), g2);
        right_node = right_nodes[0];
        nodes[right_nodes[0]].left_node = index;
    }



    // Bottom nodes
    std::vector<BNode> temp3;
    std::vector<int> bottom_nodes;
    for (auto& v : temp2) 
    {
        int vi = v.index;
        if (vi != index && v.top_node == -1 && v.x > quad_left && v.x < quad_right && v.y > (quad_bottom - ylen) && v.y < quad_bottom)
        {
            bottom_nodes.push_back(v.index);
        }
        else
        {
            temp3.push_back(v);
        }
    }

    if (bottom_nodes.size() > 0) 
    {
        std::random_device rd3;
        std::mt19937 g3(rd3());
        std::shuffle(bottom_nodes.begin(), bottom_nodes.end(), g3);
        bottom_node = bottom_nodes[0];
        nodes[bottom_nodes[0]].top_node = index;
    }


    // Top nodes
    std::vector<int> top_nodes;
    for (auto& v : temp3) 
    {
        int vi = v.index;
        if (vi != index && v.bottom_node == -1 && v.x > quad_left && v.x < quad_right && v.y > quad_top && v.y < (quad_top + ylen))
        {
            top_nodes.push_back(v.index);
        }
    }

    if (top_nodes.size() > 0) 
    {
        std::random_device rd4;
        std::mt19937 g4(rd4());
        std::shuffle(top_nodes.begin(), top_nodes.end(), g4);
        top_node = top_nodes[0];
        nodes[top_nodes[0]].bottom_node = index;
    }
    
   return;
}


void BNode::phase2_links(std::vector<BNode>& nodes, int k) 
{
    double ql = quad_left;
    double qr = quad_right;
    double qb = quad_bottom;
    double qt = quad_top;

    for (int i = 0; i < (k - 2); i++) 
    {
        double xmid = (ql + qr) / 2.0;
        double ymid = (qb + qt) / 2.0;
        double xlen = std::fabs((qr - ql)) / 2.0;
        double ylen = std::fabs((qt - qb)) / 2.0;
    
        std::vector<double> lt = {xmid, xmid - xlen, xmid - xlen, xmid};
        std::vector<double> rt = {xmid + xlen, xmid, xmid, xmid  + xlen};
        std::vector<double> bm = {ymid, ymid, ymid - ylen, ymid - ylen};
        std::vector<double> tp = {ymid + ylen, ymid + ylen, ymid, ymid};

        for (int j = 0; j < 4; j++) 
        {
            if (x > lt[j] && x < rt[j] && y > bm[j] && y < tp[j])
            {
                ql = lt[j];
                qr = rt[j];
                qb = bm[j];
                qt = tp[j];
                continue;
            }
            else
            {
                std::vector<int> q_nodes; 
                for (auto& v : nodes) 
                {
                    int vi = v.index;
                    if (vi != index && edges.find(vi) == edges.end() && v.x > lt[j] && v.x < rt[j] && v.y > bm[j] && v.y < tp[j])
                    {
                        q_nodes.push_back(vi);
                    }
                }

                if (q_nodes.size() > 0) 
                {
                    std::random_device rd1;
                    std::mt19937 g1(rd1());
                    std::shuffle(q_nodes.begin(), q_nodes.end(), g1);
 
                    edges.insert(q_nodes[0]);
                    vedges[i].push_back(q_nodes[0]);
                    nodes[q_nodes[0]].edges.insert(index);
                    nodes[q_nodes[0]].vedges[i].push_back(index);
                }
            }
        }
    }

}


void BNode::phase3_links(std::vector<BNode>& nodes, int k, double r) 
{
    for (auto& v : nodes) 
    {
        int vi = v.index;
        if (vi != index && edges.find(vi) == edges.end() && cheb_distance[vi] < pow(r, k))
        {
            kappa_nodes.push_back(vi);
            edges.insert(vi);

            nodes[vi].kappa_nodes.push_back(index);
            nodes[vi].edges.insert(index);
        }
    }
}