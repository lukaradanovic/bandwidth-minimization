#ifndef _UTIL_HPP_
#define _UTIL_HPP_ 1

#include <fstream>
#include "algorithms.hpp"

Graph* load_graph(const std::string& filename);
void save_graph(Graph& g, const::std::string& filename);
Graph* generate_graph(int n, double density=0.2);

void save_solutions_generated(const std::string& input_file, const std::string& output_file);
void save_solutions_hb(const std::string& input_file, const std::string& output_file, int t_max);

void error(const std::string& msg);

#endif