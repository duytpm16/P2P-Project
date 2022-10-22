#include "p2p.h"
#include "graph.h"
#include "bgraph.h"
#include "args.h"

int main(int argc, char* argv[]) 
{
    CommandLine c;
    c.args(argc, argv);

    //Graph G(c.file);

    std::cout << "Constructing graph..\n";
    //G.construct_graph(c.r);

    std::cout << "Performing greedy routing..." << std::endl; 
    //G.greedy_routing(c.t);

    BGraph B(c.file);
    B.construct_graph(0.25, 0, 1, 0, 1, 2);

    return 0;
}


