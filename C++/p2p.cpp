#include "p2p.h"
#include "graph.h"
#include "bgraph.h"
#include "args.h"

int main(int argc, char* argv[]) 
{
    CommandLine c;
    c.args(argc, argv);

    std::cout << "Constructing routing graph..\n";
    Graph G(c.file);
    G.construct_graph(c.r);

    std::cout << "Performing greedy routing...\n"
    G.greedy_routing(c.t);

    std::cout << "Constructing broadcast graph..\n";
    BGraph B(c.file);
    B.construct_graph(0.25, 0, 1, 0, 1, 2);

    return 0;
}


