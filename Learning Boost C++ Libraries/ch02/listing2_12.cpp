#include <iostream>
#include <boost/program_options.hpp>

namespace po = boost::program_options;
namespace postyle = boost::program_options::command_line_style;

int main(int argc, char *argv[])
{
    po::options_description desc("Options");
    desc.add_options()
    ("unified,U", po::value<unsigned int>()->default_value(3),
     "Print in unified form with specified number of "
     "lines from the surrounding context")
    (",p", "Print names of C functions "
     " containing the difference")
    (",N", "When comparing two directories, if a file exists in"
     " only one directory, assume it to be present but "
     " blank in the other directory")
    ("help,h", "Print this help message");
    int unix_style    = postyle::unix_style
                        | postyle::short_allow_next;
    int windows_style = postyle::allow_long
                        | postyle::allow_short
                        | postyle::allow_slash_for_short
                        | postyle::case_insensitive
                        | postyle::short_allow_next
                        | postyle::long_allow_next;
    po::variables_map vm;

    try {
        po::store(
            po::command_line_parser(argc, argv)
            .options(desc)
            .style(unix_style)  // or windows_style
            .run(), vm);
        po::notify(vm);

        if (argc == 1 || vm.count("help")) {
            std::cout << "USAGE: " << argv[0] << '\n'
                      << desc << '\n';
            return 0;
        }
    } catch (po::error& poe) {
        std::cerr << poe.what() << '\n'
                  << "USAGE: " << argv[0] << '\n' << desc << '\n';
        return EXIT_FAILURE;
    }

    unsigned int context = 0;

    if (vm.count("unified")) {
        context = vm["unified"].as<unsigned int>();
    }

    bool print_cfunc = (vm.count("p") > 0);
}
