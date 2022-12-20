#include "graph.h"

// Find closest node to destination node from current node
int Graph::closest(std::vector<int>& edges, std::vector<double> distances, std::vector<int> visited)
{
    double min_dist = 1e6;
    int min_node = -1;
    
    for (auto& index : edges) 
    {
        double dist = distances[index];
        if (std::find(visited.begin(), visited.end(), index) == visited.end() && dist < min_dist) 
        {
            min_node = index;
            min_dist = dist;
        }
    }
    return min_node;
}


// Route node u to node v via greedy routing
void Graph::route(Node& u, Node& v, int* add_h, double* add_p, std::vector<int>* add_path) 
{
    double prop = 0.0;
    int hops = 0;
    int iv = v.index;
    int icurr = u.index;
    int iprev = u.index;
    std::vector<int> visited({icurr});
    std::vector<double>& eucl_distance = v.eucl_distance;

    while (icurr != iv)
    {
        iprev = icurr;
        icurr = closest(nodes[iprev].vedges, eucl_distance, visited);
        if (icurr == -1) {
            return;
        }

        visited.push_back(icurr);
        prop += nodes[iprev].eucl_distance[icurr];
        hops += 1;
    }

    *add_h = hops;
    *add_p = prop;
    *add_path  = visited;

    return;
}


// Summarize routing results for all possible routes
void Graph::greedy_route(int start, int end, int t, std::string outfile) 
{
    int cr = 0;
    int ir = 0;
    int max_hops = 0;
    double avg_hops = 0.0;
    double avg_prop = 0.0;
    double max_prop = 0.0;
    std::vector<int> worse_hops;
    std::vector<int> worse_prop;

    for (int i = start; i < end; i++) 
    {
        auto& u = nodes[i];
        for (auto& v : nodes) 
        {
            if (u.index != v.index) 
            {
                int hops = 0;
                double prop = 0.0;
                std::vector<int> path;
                route(u, v, &hops, &prop, &path);

                if (hops > 0) 
                {
                    cr += 1;
                    avg_hops += (double)hops;
                    if (hops > max_hops) 
                    {
                        max_hops = hops;
                        worse_hops = path;
                    }

                    avg_prop += prop;
                    if (prop > max_prop) 
                    {
                        max_prop = prop;
                        worse_prop = path;
                    }
                } 
                else
                {
                    ir += 1;
                }
            }
        }
    }

    std::ofstream ofs;
    ofs.open(outfile);
    ofs << "#Complete_routes: " + std::to_string(cr) + "\n";
    ofs << "#Incomplete_routes: " + std::to_string(ir) + "\n";
    if (t == 1) {
        ofs << "#Average_hops: " + std::to_string(avg_hops / (double)cr) + "\n";
        ofs << "#Maximum_hops: " + std::to_string(max_hops) + "\n";
        ofs << "#Average_propagation: " + std::to_string(avg_prop / (double)cr) + "\n";
        ofs << "#Maximum_propagation: " + std::to_string(max_prop) + "\n"; 
    }
    else
    {
        ofs << "#Average_hops: " + std::to_string(avg_hops) + "\n";
        ofs << "#Maximum_hops: " + std::to_string(max_hops) + "\n";
        ofs << "#Average_propagation: " + std::to_string(avg_prop) + "\n";
        ofs << "#Maximum_propagation: " + std::to_string(max_prop) + "\n"; 
    }
    for (auto& i : worse_hops) 
    {
        ofs << std::to_string(nodes[i].x) + "\t" + std::to_string(nodes[i].y) + "\n";
    } 
    ofs.close();
    
    return;
}



void Graph::greedy_routing(int t) 
{
    int cr = 0;
    int ir = 0;
    int max_hops = 0;
    double avg_hops = 0.0;
    double avg_prop = 0.0;
    double max_prop = 0.0;
    std::vector<int> worse_hops;
    std::vector<int> worse_prop;

    if (t > 1) 
    {
        // multithread here
        boost::thread_group thread_grp;
        for (int i = 0; i < t; i++)
        {
            int start = std::ceil((n / t) * i);
            int end   = (i != (t-1)) ? std::ceil((n / t) * (i + 1)) : n;
            std::string outfile = "greedy_routing_results_" + std::to_string(i) + ".txt";

            std::cout << "Thread: " << start << " - " << end << std::endl;
            thread_grp.create_thread(boost::bind(&Graph::greedy_route, this, start, end, t, outfile)); 
        }
        thread_grp.join_all();


        for (int i = 0; i < t; i++) 
        {
            std::string threadfile = "greedy_routing_results_" + std::to_string(i) + ".txt";

            std::ifstream file;
            file.open(threadfile);
            if (!file.is_open()) 
            {
                std::cout << "Error: Cannot open " << threadfile << " file.\n";
                exit(1);
            }

            std::string line = "";
            while(getline(file, line)) 
            {
                line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
                std::istringstream iss(line);

                std::string header;
                iss >> header;
                if (header == "#Complete_routes:") {
                    int iv;
                    iss >> iv;
                    cr += iv;
                }
                if (header == "#Incomplete_routes:") {
                    int iv;
                    iss >> iv;
                    ir += iv;
                }
                if (header == "#Average_hops:") {
                    int iv;
                    iss >> iv;
                    avg_hops += (double)iv;
                }
                if (header == "#Maximum_hops:") {
                    int iv;
                    iss >> iv;
                    if (iv > max_hops) {
                        max_hops = iv;
                    }
                }
                if (header == "#Average_propagation:") {
                    double dv;
                    iss >> dv;
                    avg_prop += dv;
                }
                if (header == "#Maximum_propagation:") {
                    double dv;
                    iss >> dv;
                    if (dv > max_prop) {
                        max_prop = dv;
                    }
                }
            }
            file.close();
        }


        std::ofstream ofs;
        ofs.open("greedy_routing_results.txt");
        ofs << "#Complete_routes: " + std::to_string(cr) + "\n";
        ofs << "#Incomplete_routes: " + std::to_string(ir) + "\n";
        ofs << "#Average_hops: " + std::to_string(avg_hops / (double)cr) + "\n";
        ofs << "#Maximum_hops: " + std::to_string(max_hops) + "\n";
        ofs << "#Average_propagation: " + std::to_string(avg_prop / (double)cr) + "\n";
        ofs << "#Maximum_propagation: " + std::to_string(max_prop) + "\n"; 

        ofs.close();
    } 
    else 
    {
        greedy_route(0, n, t, "greedy_routing_results.txt");
    }

    return;
}