#ifndef _TESTS_HPP_
#define _TESTS_HPP_ 1

#include <filesystem>
#include "algorithms.hpp"
#include "util.hpp"

void hb_test(const std::string& data_folder, const std::string& results_folder, int t_max, int num_repetitions, bool use_bfs);
void gen_test(const std::string& data_folder, const std::string& results_file, bool use_bfs);

#endif