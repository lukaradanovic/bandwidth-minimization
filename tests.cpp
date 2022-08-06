#include "tests.hpp"

void hb_test(const std::string& data_folder, const std::string& results_file, int t_max, int num_repetitions, bool use_bfs)
{
    std::ofstream output(results_file);
    if (!output.is_open())
        error("Unable to open output file");

    output << "name bandwidth time" << std::endl;

    for (const auto & entry : std::filesystem::directory_iterator(data_folder))
    {
        Graph* g = load_graph(entry.path());

        std::vector<double> times(num_repetitions);
        std::vector<int> bands(num_repetitions);

        for (int i = 0; i < num_repetitions; i++)
        {
            Result res = simulated_annealing(*g, t_max, use_bfs, false);
            times[i] = res.time;
            bands[i] = res.bandwidth;
        }

        double avg_time = std::accumulate(times.begin(), times.end(), 0.0) / (double)num_repetitions;
        double avg_band = std::accumulate(bands.begin(), bands.end(), 0.0) / (double)num_repetitions;

        std::string fname = std::string(entry.path().stem());
        
        std::cout << fname << " b: " << avg_band << " t: " << avg_time << std::endl;
        output << fname << " " << avg_band << " " << avg_time << std::endl;

        delete g;
    }

    output.close();

}

void gen_test(const std::string& data_folder, const std::string& results_file, bool use_bfs)
{
    std::ofstream output(results_file);
    if (!output.is_open())
        error("Unable to open output file");

    output << "name bandwidth time" << std::endl;

    for (const auto & entry : std::filesystem::directory_iterator(data_folder))
    {
        Graph* g = load_graph(entry.path());
        
        std::vector<int> labeling(g->mat.size());

        auto start_time = std::chrono::steady_clock::now();

        if (use_bfs)
            bfs(*g, labeling);
        else
            brute_force(*g, labeling);

        std::chrono::duration<double> new_time = std::chrono::steady_clock::now() - start_time;

        double time = new_time.count();
        int bandwidth = calc_bandwidth(*g, labeling);

        std::string fname = std::string(entry.path().stem());

        std::cout << fname << " b: " << bandwidth << " t: " << time << std::endl;
        output << fname << " " << bandwidth << " " << time << std::endl;

        delete g;
    }

    output.close();
}