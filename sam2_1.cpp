#include <iostream>
#include <vector>

using namespace std;

int main() {
    const int types = 5;
    const int M = 108;

    int m[types + 1] = { 0, 5, 5, 5, 4, 4 };
    int u[types + 1] = { 0, 68, 50, 41, 51, 66 };
    int v[types + 1] = { 0, 16, 15, 15, 20, 11 };

    vector<int> weights(1, 0);
    vector<int> values(1, 0);
    vector<int> originalType(1, 0);

    int k = 0;
    for (int i = 1; i <= types; i++) {
        for (int j = 1; j <= m[i]; j++) {
            weights.push_back(v[i]);
            values.push_back(u[i]);
            originalType.push_back(i);
            k++;
        }
    }
    vector<vector<int>> A(k + 1, vector<int>(M + 1, 0));

    // Прямой проход
    for (int s = 1; s <= k; s++) {
        for (int n = 0; n <= M; n++) {
            A[s][n] = A[s - 1][n];
            if (n >= weights[s]) {
                int take = A[s - 1][n - weights[s]] + values[s];
                if (take > A[s][n])
                    A[s][n] = take;
            }
        }
    }
    cout << "Матрица A[s][n] (строки - предметы, столбцы - вместимость):\n";
    cout << "\t";
    // Вывод
    for (int n = 0; n <= M; n++) {
        if (n % 10 == 0 || n == M) {
            cout << n << "\t";
        }
    }
    cout << "\n";
    for (int s = 0; s <= k; s++) {
        cout << s << "\t";

        for (int n = 0; n <= M; n++) {
            if (n % 10 == 0 || n == M) {
                cout << A[s][n] << "\t";
            }
        }
        cout << "\n";
    }
    // Обратный проход
    int n = M;
    int totalW = 0, totalP = 0;
    vector<int> chosenTypes(types + 1, 0);
    for (int s = k; s >= 1; s--) {
        if (A[s][n] != A[s - 1][n]) {
            int type = originalType[s];
            chosenTypes[type]++;
            totalW += weights[s];
            totalP += values[s];
            n -= weights[s];
        }
    }

    cout << "Оптимальный план:\n";
    for (int i = 1; i <= types; i++) {
        if (chosenTypes[i] > 0) {
            cout << "Груз вида " << i << ": взять " << chosenTypes[i]
                << " шт., вес " << v[i] << ", прибыль " << u[i] << "\n";
        }
    }
    cout << "\nИтого: \n";
    cout << "Общий вес = " << totalW << " максимум " << M << "\n";
    cout << "Максимальная прибыль = " << totalP << "\n";

    return 0;
}
