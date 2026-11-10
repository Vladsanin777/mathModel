#include <iostream>
#include <algorithm>
#include <vector>

int main(void) {
    constexpr size_t count{10};
    constexpr size_t totalMass{22};

    std::vector<size_t> efficiency {
        71, 28, 43, 89, 17, 59, 33, 98, 64, 92
    };

    std::vector<size_t> weight {
        2, 7, 3, 10, 3, 7, 6, 4, 9, 10
    };

    std::vector<std::vector<bool>> keep(
        count, std::vector<bool>(totalMass + 1, false)
    );

    std::vector<size_t> dp(totalMass + 1);

    for (size_t i{0}; i < count; i++) {
        size_t w_i{weight[i]}, e_i{efficiency[i]};
        for (size_t w{totalMass}; w >= w_i; w--) {
            if (dp[w-w_i] + e_i > dp[w]) {
                dp[w] = dp[w-w_i] + e_i;
                keep[i][w] = true;
            }
        }
    }

    std::vector<size_t> chosen_indices;
    size_t current_w{totalMass};

    for (size_t i{count - 1}; i --> 0; ) {
        if (keep[i][current_w]) {
            chosen_indices.push_back(i);
            current_w -= weight[i];
        }
    }

    std::reverse(chosen_indices.begin(), chosen_indices.end());

    std::cout << "Max efficiency: " << dp[totalMass] << std::endl;

    std::cout << "Chosen indices:" << std::endl;

    for (const size_t & chosen : chosen_indices) {
        std::cout << chosen << ' ';
    }

    std::cout << std::endl;

    return 0;
}
