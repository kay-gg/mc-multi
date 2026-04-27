#include "timing.hpp"
#include "world.hpp"
#include "gen.hpp"

#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

namespace {

constexpr int CHUNK_GRID_WIDTH = 19;
constexpr int RUNS_PER_TEST = 5;
const std::string OUTPUT_FILE = "timing_results.csv";

struct TimingCase {
    std::string algorithmName;
    int threadCount;
    bool useFastGenerator;
};

std::unique_ptr<ChunkGenerator> makeGenerator(bool useFastGenerator) {
    if (useFastGenerator) {
        return std::make_unique<FlatWorldGen>();
    }

    return std::make_unique<FlatWorldGenSequential>();
}

double timeOneWorldGeneration(const TimingCase& testCase) {
    World world(testCase.threadCount, makeGenerator(testCase.useFastGenerator));

    auto start = std::chrono::high_resolution_clock::now();

    for (int x = 0; x < CHUNK_GRID_WIDTH; x++) {
        for (int z = 0; z < CHUNK_GRID_WIDTH; z++) {
            world.genChunk(ChunkCord{x, z});
        }
    }

    world.waitForAllChunks();

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> elapsed = end - start;
    return elapsed.count();
}

}

void runTimingReport() {
    int maxThreads = static_cast<int>(std::thread::hardware_concurrency());

    if (maxThreads <= 0) {
        maxThreads = 4;
    }

    std::vector<TimingCase> tests = {
        {"slow", 1, false},
        {"slow", maxThreads, false},
        {"fast", 1, true},
        {"fast", maxThreads, true},
    };

    std::ofstream out(OUTPUT_FILE);

    if (!out) {
        std::cerr << "Could not open " << OUTPUT_FILE << " for writing.\n";
        return;
    }

    out << "algorithm,threads,run,chunks,time_ms\n";

    std::cout << "Writing timing data to " << OUTPUT_FILE << "\n";
    std::cout << "Chunks per run: " << CHUNK_GRID_WIDTH * CHUNK_GRID_WIDTH << "\n";
    std::cout << "Max hardware threads: " << maxThreads << "\n";

    for (const TimingCase& testCase : tests) {
        for (int run = 1; run <= RUNS_PER_TEST; run++) {
            double ms = timeOneWorldGeneration(testCase);

            out << testCase.algorithmName << ","
                << testCase.threadCount << ","
                << run << ","
                << CHUNK_GRID_WIDTH * CHUNK_GRID_WIDTH << ","
                << std::fixed << std::setprecision(3) << ms << "\n";

            std::cout << testCase.algorithmName
                      << " threads=" << testCase.threadCount
                      << " run=" << run
                      << " time_ms=" << std::fixed << std::setprecision(3) << ms
                      << "\n";
        }
    }

    std::cout << "Done. Now run: python3 graph_timing.py\n";
}