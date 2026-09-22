#include <vector>
#include <iostream>
#include <algorithm>

int main(void) {
    std::vector<std::vector<int>> sitRight {
        {5, 11, 12, 5},
        {7, 5, 10, 7},
        {10, 5, 9, 9},
        {5, 6, 4, 14},
        {7, 4, 8, 6},
        {8, 11, 13, 10},
        {8, 16, 2, 7}
    }, sitBottom {
        {11, 3, 5, 3, 6},
        {11, 13, 11, 17, 7},
        {1, 3, 8, 8, 7},
        {2, 9, 11, 13, 8},
        {8, 4, 8, 11, 9},
        {12, 9, 17, 10, 6}
    };

    size_t rows = sitRight.size();
    size_t cols = sitBottom[0].size();

    std::vector<std::vector<int>> dp(rows, std::vector<int>(cols, 0));

    for (size_t i = 1; i < rows; i++)
        dp[i][0] = dp[i-1][0] + sitBottom[i - 1][0];

    for (size_t i = 1; i < cols; i++)
        dp[0][i] = dp[0][i-1] + sitRight[0][i - 1];

    for (size_t i = 1; i < rows; i++) {
        for (size_t j = 1; j < cols; j++) {
            dp[i][j] = std::min(
                dp[i - 1][j] + sitBottom[i - 1][j],
                dp[i][j - 1] + sitRight[i][j - 1]
            );
        }
    }

    std::cout << "Min way for dp: " << dp[rows - 1][cols - 1] << std::endl;

    return 0;
}
