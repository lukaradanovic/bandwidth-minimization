#include "algorithms.hpp"

Result simulated_annealing(Graph& g, int t_max, bool use_bfs, bool verbose)
{
    int dim = g.mat.size();
    int m = 0;
    for (int i = 0; i < dim; i++)
        m += g.mat[i].size();
    
    double t0 = 1000;
    double tf = 1e-7;
    double alpha = 0.99;
    double t = t0;
    double l = 40;
    double lf = 10 * dim * m;
    double r = (log(tf) - log(t0)) / log(alpha);
    double gamma = exp((log(lf) - log(l)) / r);
    
    std::vector<int> current_sol(dim);
    std::vector<int> alt_sol(dim);
    std::vector<int> new_sol(dim);
    std::vector<int> best_sol(dim);
    std::vector<int> d_vec(dim);
    
    
    std::vector<int> default_sol(dim);
    std::iota(default_sol.begin(), default_sol.end(), 0);


    Result result = Result();
    result.history.emplace_back(0.0, calc_bandwidth(g, default_sol));
    
    if (verbose)
        std::cout << "Working..." << std::endl;
    
    auto start_time = std::chrono::steady_clock::now();
    
    
    if (use_bfs)
    {
        bfs(g, current_sol);
    }
    else
    {
        std::iota(current_sol.begin(), current_sol.end(), 0);
        std::random_shuffle(current_sol.begin(), current_sol.end());
    }
    
    std::copy(current_sol.begin(), current_sol.end(), best_sol.begin());
    
    int true_band;
    double band = calc_bandwidth_detailed(g, current_sol, d_vec, true_band);
    double best_band = band;
    
    
    std::chrono::duration<double> new_time = std::chrono::steady_clock::now() - start_time;
    result.history.emplace_back(new_time.count(), true_band);
    
    
    while (t > tf)
    {
        bool improvement = false;
        
        int i;
        for (i = 0; i < l; i++)
        {
            get_neighbor(g, current_sol, alt_sol, new_sol);
            int new_true_band;
            double new_band = calc_bandwidth_detailed(g, new_sol, d_vec, new_true_band);
            
            if (new_band < band)
            {
                std::copy(new_sol.begin(), new_sol.end(), current_sol.begin());
                band = new_band;
            }
            else if ((double)std::rand() / RAND_MAX < exp(-(new_band-band) / t))
            {
                std::copy(new_sol.begin(), new_sol.end(), current_sol.begin());
                band = new_band;    
            }
            
            if (new_band < best_band)
            {   
                if (new_true_band < true_band)
                {
                    new_time = std::chrono::steady_clock::now() - start_time;
                    result.history.emplace_back(new_time.count(), new_true_band);
                    true_band = new_true_band;

                    std::copy(new_sol.begin(), new_sol.end(), best_sol.begin());
                }
            
                best_band = new_band;
                improvement = true;
            }
        }
        
        if (!improvement)
        {
            t *= alpha;
            l *= gamma;
        }
        
        new_time = std::chrono::steady_clock::now() - start_time;
        if (new_time.count() > t_max)
        {
            if (verbose)
                std::cout << "Timeout" << std::endl;
            break;
        }
    }
    
    new_time = std::chrono::steady_clock::now() - start_time;
    if (verbose)
        std::cout << "Total running time: " <<  new_time.count() << "s" << std::endl;
    
    
    result.best_sol.resize(dim);
    std::copy(best_sol.begin(), best_sol.end(), result.best_sol.begin());
    result.time = result.history[result.history.size()-1].first;
    result.bandwidth = result.history[result.history.size()-1].second;
    
    return result;
}

double calc_bandwidth_detailed(Graph& graph, std::vector<int>& sol, std::vector<int>& d_vec, int& true_band)
{
    int dim = graph.mat.size();
    double max_band = 0;
    
    for (int i = 0; i < dim; i++)
    {
        d_vec[i] = 0;
    }
    
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < graph.mat[i].size(); j++)
        {
            int diff = abs(sol[i] - sol[graph.mat[i][j]]);
            max_band = std::max(max_band, (double)diff);
            d_vec[diff]++;
        }
    }
    
    double delta = 0;
    for (int i = 0; i <= max_band; i++)
    {
        delta = (delta + d_vec[i]) / (double)(dim - i + 1);
    }
    /* correct way
    delta = (delta + d_vec[0]) / (double)(dim + 1);
    for (int i = 1; i <= max_band; i++)
    {
        delta = (delta + d_vec[i]) / (double)(2*(dim - i + 1));
    }
    */
    true_band = max_band;
    
    return max_band + delta;
}

int calc_bandwidth(Graph& graph, std::vector<int>& sol)
{
    int dim = graph.mat.size();
    int max_band = 0;
    
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < graph.mat[i].size(); j++)
        {
            int diff = abs(sol[i] - sol[graph.mat[i][j]]);
            max_band = std::max(max_band, diff);
        }
    }
    
    return max_band;
}

void get_neighbor(Graph& g, std::vector<int>& current_sol, std::vector<int>& alt_sol, std::vector<int>& new_sol)
{
    double a = 0.6;
    double b = 0.2;
    int dim = g.mat.size();
    
    double p = (double)rand() / (double)RAND_MAX;
    
    if (p <= a)
    {
        std::copy(current_sol.begin(), current_sol.end(), new_sol.begin());
        rex(new_sol);
    }
    else if (p <= a + b)
    {
        std::copy(current_sol.begin(), current_sol.end(), new_sol.begin());
        nex(g, new_sol);
    }
    else 
    {
        rot(g, current_sol, alt_sol, new_sol);
    }
}

void rot (Graph& g, std::vector<int>& current_sol, std::vector<int>& alt_sol, std::vector<int>& new_sol)
{
    int dim = g.mat.size();
    for (int i = 0; i < dim; i++)
    {
        alt_sol[current_sol[i]] = i;
    }
    
    int index1;
    int index2; 
    if (dim >= 6)
    {
        index1 = rand_int(0, dim-6);
        index2 = index1 + rand_int(1, 5);
    }
    else
    {
        index1 = 0;
        index2 = rand_int(0, dim-1);
    }
    
    std::rotate(alt_sol.begin()+index1, alt_sol.begin()+index1+1, alt_sol.begin()+index2+1);
    
    for (int i = 0; i < dim; i++)
    {
        new_sol[alt_sol[i]] = i;
    }
    
}

void nex(Graph& g, std::vector<int>& sol)
{
    int index1 = rand_int(0, g.mat.size()-1);
    int index2 = g.mat[index1][rand_int(0, g.mat[index1].size()-1)];
    
    std::swap(sol[index1], sol[index2]);
}

void rex(std::vector<int>& sol)
{
    int dim = sol.size();
    int index1 = rand_int(0, dim-1);
    int index2 = rand_int(0, dim-2);
    if (index2 >= index1)
        index2++;


    std::swap(sol[index1], sol[index2]);
}

int rand_int(int a, int b)
{
    double r = std::rand() / (double)RAND_MAX;
    r = (r * (b + 1 - a)) + a;
    return std::min((int)floor(r), b);
}

void bfs(const Graph& graph, std::vector<int>& labeling)
{
    int n = graph.mat.size();
    std::vector<int> vertices(n);
    
    for (int i=0; i < n; i++)
    {
        vertices[i] = i;
        labeling[i] = -1;
    }
        
    std::sort(vertices.begin(), vertices.end(), [graph](int x, int y){
        return graph.mat[x].size() < graph.mat[y].size(); 
    });
    
    int next_label = 0; int l = 0;
    for (int i = 0; i < n; i++)
    {   
        if (labeling[vertices[i]] == -1)
        {
            bfs_(graph, vertices[i], labeling, next_label);
        } 
    }
}

void bfs_(const Graph& graph, int start_node, std::vector<int>& labeling, int& next_label)
{
    int n = graph.mat.size();
    std::queue<int> q = std::queue<int>();
    labeling[start_node] = next_label;
    next_label++;
    
    q.push(start_node);
    while(!q.empty())
    {
        int node = q.front();
        q.pop();
        
        for (int i=0; i < graph.mat[node].size(); i++)
        {
            if (labeling[graph.mat[node][i]] == -1)
            {
                q.push(graph.mat[node][i]);
                labeling[graph.mat[node][i]] = next_label;
                next_label++;
            }
        }
    }
    
}

int brute_force(Graph& g, std::vector<int>& best_sol)
{
    int n = best_sol.size();
    std::vector<int> current_sol(n);

    std::iota(current_sol.begin(), current_sol.end(), 0);
    std::copy(current_sol.begin(), current_sol.end(), best_sol.begin());
    
    int bandwidth = calc_bandwidth(g, current_sol);

    while (std::next_permutation(current_sol.begin(), current_sol.end()))
    {
        int new_band = calc_bandwidth(g, current_sol);
        if (new_band < bandwidth)
        {
            bandwidth = new_band;
            std::copy(current_sol.begin(), current_sol.end(), best_sol.begin());
        }
    }


    return bandwidth;
}