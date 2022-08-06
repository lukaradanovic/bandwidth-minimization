#include "algorithms.hpp"
#include "util.hpp"
#include "tests.hpp"

int main(int argc, char** argv)
{
    std::string fname;
    if (argc <= 1)
    {
        std::cout << "Enter filename: ";
        std::cin >> fname;    
    }
    else
    {
        fname = std::string(argv[1]);
    }

    Graph* g = load_graph(fname);
    int t_max = 20;
    Result res = simulated_annealing(*g, t_max, false, true);
    
    std::cout << "Bandwidth: " << res.bandwidth << std::endl;
    std::cout << "Time: " << res.time << "s" << std::endl;    
    
    delete g;

    return 0;
}