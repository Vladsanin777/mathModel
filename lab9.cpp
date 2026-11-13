#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

struct Edge {
    int from;
    int to;
    int weight;
};

int main() {
    const int n = 10;
    vector<Edge> edges = {
        {1, 2, 9},  // d1
        {1, 4, 3},  // d2
        {2, 3, 2},  // d3
        {2, 1, 9},  // d4
        {3, 2, 2},  // d5
        {3, 6, 8},  // d6
        {4, 1, 3},  // d7
        {4, 8, 4},  // d8
        {5, 7, 1},  // d9
        {5, 9, 5},  // d10
        {6, 3, 8},  // d11
        {6, 9, 3},  // d12
        {7, 8, 6},  // d13
        {7, 5, 1},  // d14
        {8, 4, 4},  // d15
        {8, 7, 6},  // d16
        {9, 5, 5},  // d17
        {9, 6, 3}   // d18
    };
    cout << " Список ребер\n";
    cout << setw(10) << "Откуда"
        << setw(10) << "Куда"
        << setw(10) << "Вес" << "\n";


    for (size_t i = 0; i < edges.size(); i++) {
        cout << setw(10) << edges[i].from
            << setw(10) << edges[i].to
            << setw(10) << edges[i].weight << "\n";
    }

    vector<vector<int>> adjMatrix(n, vector<int>(n, 0));
    for (size_t i = 0; i < edges.size(); i++) {
        int u = edges[i].from - 1;
        int v = edges[i].to - 1;
        int w = edges[i].weight;

        adjMatrix[u][v] = w;
    }
    cout << "\n\n Матрица Смежности\n";

    cout << "    ";
    for (int j = 0; j < n; j++) {
        cout << setw(4) << (j + 1);
    }
    cout << "\n";

    for (int i = 0; i < n; i++) {
        cout << "V" << setw(2) << (i + 1) << " ";

        for (int j = 0; j < n; j++) {

            cout << setw(4) << adjMatrix[i][j];
        }
        cout << "\n";
    }

    return 0;
}
