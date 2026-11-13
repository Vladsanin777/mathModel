#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Edge {
    int u, v, weight;
};

int main() {
    const int n = 11;

    vector<Edge> edges = {
        {1, 2, 6},
        {2, 4, 10},
        {2, 3, 12},
        {2, 5, 4},
        {3, 6, 18},
        {4, 5, 8},
        {4, 8, 11},
        {4, 8, 16},
        {5, 9, 20},
        {6, 7, 15},
        {6, 10, 2},
        {7, 10, 8},
        {8, 10, 5},
        {9, 11, 1},
        {10, 11, 1}
    };

    vector<int> tp(n, 0), tn(n, 0);

    for (int s = 0; s < n - 1; s++) {
        for (size_t k = 0; k < edges.size(); k++) {
            int u = edges[k].u - 1;
            int v = edges[k].v - 1;
            if (tp[u] + edges[k].weight > tp[v]) {
                tp[v] = tp[u] + edges[k].weight;
            }
        }
    }

    int critical_time = tp[n - 1]; // максимум

    for (int i = 0; i < n; i++) {
        tn[i] = critical_time;
    }

    for (int s = 0; s < n - 1; s++) {
        for (size_t k = 0; k < edges.size(); k++) {
            int u = edges[k].u - 1;
            int v = edges[k].v - 1;
            if (tn[v] - edges[k].weight < tn[u]) {
                tn[u] = tn[v] - edges[k].weight;
            }
        }
    }

    //  СРОКИ
    cout << "ПАРАМЕТРЫ СОБЫТИЙ6 \n";
    cout << setw(10) << "Событие"
        << setw(15) << "Ранний срок"
        << setw(15) << "Поздний срок"
        << setw(15) << "Резерв" << "\n";

    for (int i = 0; i < n; i++) {
        int reserve = tn[i] - tp[i];
        cout << setw(10) << (i + 1)
            << setw(15) << tp[i]
            << setw(15) << tn[i]
            << setw(15) << reserve << "\n";
    }

    cout << "\nПАРАМЕТРЫ РАБОТ: \n";
    cout << setw(10) << "Работа"
        << setw(10) << "Длительность"
        << setw(15) << "Полный резерв"
        << setw(15) << "Свободный резерв" << "\n";

    // РЕЗЕРВ
    for (size_t k = 0; k < edges.size(); k++) {
        int u = edges[k].u - 1;
        int v = edges[k].v - 1;

        // Полный р
        int Rn = tn[v] - tp[u] - edges[k].weight;

        // Свободный р
        int Rc = tp[v] - tp[u] - edges[k].weight;

        cout << setw(7) << edges[k].u << " -> " << setw(2) << edges[k].v
            << setw(10) << edges[k].weight
            << setw(15) << Rn
            << setw(15) << Rc << "\n";
    }

    // КРИТ ПУТ
    cout << "\nКРИТИЧЕСКИЙ ПУТЬ: \n";
    cout << "Макс время выполнения всех работ: " << critical_time << "\n";
    cout << "Критический путь: 1";

    int current = 0;
    while (current != n - 1) {
        for (size_t k = 0; k < edges.size(); k++) {
            int u = edges[k].u - 1;
            int v = edges[k].v - 1;
            if (u == current) {
                int Rn = tn[v] - tp[u] - edges[k].weight;
                if (Rn == 0) {
                    cout << " -> " << edges[k].v;
                    current = v;
                    break;
                }
            }
        }
    }
    cout << "\n";

    return 0;
}
