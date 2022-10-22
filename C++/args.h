#ifndef COMMANDLINE_H
#define COMMANDLINE_H

#include "p2p.h"
#include <boost/program_options.hpp>

namespace po = boost::program_options;

class CommandLine
{
    public:
        int t;
        double r;
        std::string file = "";

        void args(int argc, char* argv[]);
};


#endif