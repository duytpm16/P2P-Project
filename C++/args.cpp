#include "args.h"

void CommandLine::args(int argc, char* argv[]) 
{
    // General commands
    po::options_description general("General options");
    general.add_options()
        ("file", po::value<std::string>(), "")
        ("r", po::value<double>()->default_value(0.25), "")
        ("t", po::value<int>()->default_value(1), "");
    po::variables_map out;

    // QC
    try {
        po::store(po::command_line_parser(argc, argv)
            .options(general)
            .style(po::command_line_style::unix_style
                | po::command_line_style::allow_long_disguise)
            .run(),
            out);

    }
    catch (po::error const& e) {
        std::cout << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
    po::notify(out);



    if (out.count("file")) 
    {
        file = out["file"].as<std::string>();
    }
    else
    {
        std::cout << "--file parameter required.\n";
        exit(1);
    }

    if (out.count("r")) 
    {
        r = out["r"].as<double>();
        if (r <= 0 || r >=1) {
            std::cout << "--r must be a real number between (0, 1).\n";
        }
    }

    if (out.count("t")) 
    {
        t = out["t"].as<int>();
        if (t < 0) {
            std::cout << "--t must be an integer value greater than 0.\n";
            exit(1);
        }
    }
}