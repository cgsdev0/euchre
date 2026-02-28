#include <random>
#include <vector>

struct RngCore {
    std::uniform_int_distribution<int> dis{1, 6};
    std::random_device rd;
    std::mt19937 gen;

    RngCore() {
        // seed the RNG (once per boot)
        gen.seed(rd());
    }

    std::vector<int> roll() {
        return std::vector{dis(gen), dis(gen)};
    }
};
