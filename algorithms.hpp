#ifndef _ALGORITHMS_HPP_

#define _ALGORITHMS_HPP_ 1

#include <iostream>
#include <cmath>
#include <string>
#include <chrono>
#include <queue>
#include <algorithm>
#include <numeric>

struct Graph
{
    std::vector<std::vector<int> > mat;
};

struct Result
{
    // vector of pairs (time, bandwidth)
    std::vector<std::pair<double, int>> history;
    std::vector<int> best_sol;
    double time;
    int bandwidth;
};

Result simulated_annealing(Graph& g, int t_max, bool use_bfs=false, bool verbose=true);

double calc_bandwidth_detailed(Graph& graph, std::vector<int>& sol, std::vector<int>& d_vec, int& true_band);
int calc_bandwidth(Graph& graph, std::vector<int>& sol);

void get_neighbor(Graph& g, std::vector<int>& current_sol, std::vector<int>& alt_sol, std::vector<int>& new_sol);
void nex(Graph& g, std::vector<int>& sol);
void rot (Graph& g, std::vector<int>& current_sol, std::vector<int>& alt_sol, std::vector<int>& new_sol);
void rex(std::vector<int>& sol);

int rand_int(int a, int b);

void bfs(const Graph& graph, std::vector<int>& labeling);
void bfs_(const Graph& graph, int start_node, std::vector<int>& labeling, int& next_label);

int brute_force(Graph& g, std::vector<int>& best_sol);

#endif