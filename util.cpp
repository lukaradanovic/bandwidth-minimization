#include "util.hpp"

Graph* load_graph(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
        error("error opening file");
    
    
    Graph* graph = new Graph();
    int n;
    file >> n;
    graph->mat.resize(n);
    
    for (int i=0; i < n; i++)
    {
        int degree;
        file >> degree;
        graph->mat[i].resize(degree);
        
        for (int j = 0; j < degree; j++)
        {
            file >> graph->mat[i][j];
        }
    }
    
    file.close();
    
    return graph;
}

void save_graph(Graph& g, const::std::string& filename)
{
    std::ofstream output(filename);
    if (!output.is_open())
        error("Unable to open output file");


    int n = g.mat.size();
    output << n << std::endl;

    for (int i = 0; i < n; i++)
    {
        int degree = g.mat[i].size(); 
        output << degree;
        for (int j = 0; j < degree; j++)
        {
            output << " " << g.mat[i][j];
        }
        output << std::endl;
    }

    output.close();
}

Graph* generate_graph(int n, double density)
{
    Graph* g = new Graph();
    g->mat.resize(n);

    std::vector<std::vector<int>> mat(n);
    for (int i = 0; i < n; i++)
        mat[i].resize(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            mat[i][j] = 0;


    for (int i = 0; i < n; i++)
    {
        int upper_limit = floor(std::max(1.0, (n-i)*density));
        int num_edges = rand_int(1, upper_limit);

        for (int j = 0; j < num_edges; j++)
        {
            int k = rand_int(i, n-1);
            mat[i][k] = 1;
            mat[k][i] = 1;
        }
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (mat[i][j] == 1)
            {
                g->mat[i].push_back(j);
            }
        }
    }

    return g;
}

void save_solutions_generated(const std::string& input_file, const std::string& output_file)
{
    Graph* g = load_graph(input_file);
    Result res = simulated_annealing(*g, 30, false, false);
    
    int n = g->mat.size();
    std::vector<int> sol_brute(n);
    std::vector<int> sol_bfs(n);  

    brute_force(*g, sol_brute);
    bfs(*g, sol_bfs);

    std::ofstream output(output_file);
    output << "sa brute bfs" << std::endl;

    output << res.best_sol[0];
    for (int i = 1; i < res.best_sol.size(); i++)
        output << " " << res.best_sol[i];
    output << std::endl;

    output << sol_brute[0];
    for (int i = 1; i < sol_brute.size(); i++)
        output << " " << sol_brute[i];
    output << std::endl;

    output << sol_bfs[0];
    for (int i = 1; i < sol_bfs.size(); i++)
        output << " " << sol_bfs[i];
    output << std::endl;


    output.close();

    delete g;
}

void save_solutions_hb(const std::string& input_file, const std::string& output_file, int t_max)
{
    Graph* g = load_graph(input_file);
    Result res = simulated_annealing(*g, t_max, false, false);
    Result res_bfs = simulated_annealing(*g, t_max, true, false);

    int n = g->mat.size();
    std::vector<int> sol_bfs_only(n);  
    bfs(*g, sol_bfs_only);

    std::ofstream output(output_file);
    output << "sa sa_hist sa_bfs sa_bfs_hist bfs" << std::endl;

    output << res.best_sol[0];
    for (int i = 1; i < res.best_sol.size(); i++)
        output << " " << res.best_sol[i];
    output << std::endl;

    output << res.history[0].first << "," << res.history[0].second;
    for (int i = 1; i < res.history.size(); i++)
        output << " " << res.history[i].first << "," << res.history[i].second;
    output << std::endl;



    output << res_bfs.best_sol[0];
    for (int i = 1; i < res_bfs.best_sol.size(); i++)
        output << " " << res_bfs.best_sol[i];
    output << std::endl;

    output << res_bfs.history[0].first << "," << res_bfs.history[0].second;
    for (int i = 1; i < res_bfs.history.size(); i++)
        output << " " << res_bfs.history[i].first << "," << res_bfs.history[i].second;
    output << std::endl;




    output << sol_bfs_only[0];
    for (int i = 1; i < sol_bfs_only.size(); i++)
        output << " " << sol_bfs_only[i];
    output << std::endl;


    output.close();

    delete g;
}

void error(const std::string& msg)
{
    std::cerr << msg << std::endl;
    exit(EXIT_FAILURE);
}