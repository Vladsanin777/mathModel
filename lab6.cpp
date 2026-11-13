#include <iostream>
#include <iomanip>
#include <climits>
#include <string>

using namespace std;

const int n = 3; // количество поставщиков
const int m = 5; // количество потребителей

int a[n] = { 280, 200, 220 };
int b[m] = { 110,  100,  220, 80,  70 };
int c[n][m] = {
    {10, 4, 5, 8, 6},
    {7, 3, 6, 9, 10},
    {9, 8, 3, 7, 5}
};

int x[n][m];              // перевозки
bool zapol[n][m];         // базисные клетки
bool zapol1[n][m];        // копия базисных клеток для цикла
int alfa[n], beta[m];     // потенциалы
int delta[n][m];          // оценки
int znaki[n][m];          // знаки цикла
int kol_v_stroke[n];      // количество базисных клеток в строке
int kol_v_stolbce[m];     // количество базисных клеток в столбце

int i_tek, j_tek;         // текущая клетка цикла
int sv_i, sv_j;           // свободная клетка

// С-З угол
void SZU() {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++) { x[i][j] = 0; zapol[i][j] = false; }

    int aW[n], bW[m];
    for (int i = 0; i < n; i++) aW[i] = a[i];
    for (int j = 0; j < m; j++) bW[j] = b[j];

    int i = 0, j = 0;
    while (i < n && j < m) {
        int v = min(aW[i], bW[j]);
        x[i][j] = v;
        zapol[i][j] = true;
        aW[i] -= v;
        bW[j] -= v;
        if (aW[i] == 0 && bW[j] == 0) { i++; j++; }
        else if (aW[i] == 0) i++;
        else j++;
    }
}

// потенциали
void Potencial() {
    for (int i = 0; i < n; i++)
        alfa[i] = -INT_MAX;
    for (int j = 0; j < m; j++)
        beta[j] = -INT_MAX;
    alfa[0] = 0;
    bool f;
    do {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (zapol[i][j]) {
                    if ((alfa[i] == -INT_MAX) && (beta[j] != -INT_MAX))
                        alfa[i] = c[i][j] - beta[j];
                    else
                        if ((alfa[i] != -INT_MAX) && (beta[j] == -INT_MAX))
                            beta[j] = c[i][j] - alfa[i];
                }
            }
        }
        f = false;
        for (int i = 0; i < n; i++)
            if (alfa[i] == -INT_MAX)
                f = true;
        for (int j = 0; j < m; j++)
            if (beta[j] == -INT_MAX)
                f = true;
    } while (f);
}

// вычисляем оценки
void Ozenka() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (!(zapol[i][j]))
                delta[i][j] = beta[j] + alfa[i] - c[i][j];
            else
                delta[i][j] = 0;
        }
    }
}

// проверяем план
bool optim() {
    bool f = true;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++)
            if (!(zapol[i][j]) && (delta[i][j] > 0))
                f = false;
    }
    return f;
}

//нов опорн план
void Init_cicl() {
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j) znaki[i][j] = 0;
    for (int i = 0; i < n; ++i)
        kol_v_stroke[i] = 0;
    for (int j = 0; j < m; ++j)
        kol_v_stolbce[j] = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (zapol1[i][j]) {
                kol_v_stroke[i]++;
                kol_v_stolbce[j]++;
            }
        }
    }
    i_tek = sv_i;
    j_tek = sv_j;
    znaki[i_tek][j_tek] = 1;
}

void NewPlan() {
    int i_min, j_min, vmax, vmin;
    vmax = 0;
    // максимальную оценку
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (delta[i][j] > vmax && !(zapol[i][j])) {
                vmax = delta[i][j];
                sv_i = i;
                sv_j = j;
            }
        }
    }
    if (vmax == 0) return;

    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            zapol1[i][j] = zapol[i][j];

    Init_cicl();

    bool cycle_found = false;
    int safety_counter = 0;

    do {
        safety_counter++;

        bool found_col = false;
        for (int i = 0; i < n; ++i) {
            if (i != i_tek && zapol1[i][j_tek]) {
                i_tek = i;
                znaki[i_tek][j_tek] = -1;
                found_col = true;
                break;
            }
        }
        if (!found_col) break;
        if (i_tek == sv_i) { cycle_found = true; break; }

        bool found_row = false;
        for (int j = 0; j < m; ++j) {
            if ((j != j_tek) && (zapol1[i_tek][j])) {
                j_tek = j;
                znaki[i_tek][j_tek] = 1;
                found_row = true;
                break;
            }
        }
        if (!found_row) break;

        if (kol_v_stolbce[j_tek] < 2) {
            zapol1[i_tek][j_tek] = false;
            Init_cicl();
        }
        if (kol_v_stroke[i_tek] < 2) {
            zapol1[i_tek][j_tek] = false;
            Init_cicl();
        }
    } while (true);

    if (!cycle_found) {
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < m; ++j)
                znaki[i][j] = 0;
        for (int i = 0; i < n && !cycle_found; ++i) {
            if (i == sv_i) continue;
            for (int j = 0; j < m && !cycle_found; ++j) {
                if (j == sv_j) continue;
                if (zapol1[i][sv_j] && zapol1[sv_i][j] && zapol1[i][j]) {
                    znaki[sv_i][sv_j] = 1;
                    znaki[i][sv_j] = -1;
                    znaki[i][j] = 1;
                    znaki[sv_i][j] = -1;
                    cycle_found = true;
                }
            }
        }
    }

    // Находим минимум
    vmin = INT_MAX;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (znaki[i][j] == -1 && x[i][j] < vmin) {
                vmin = x[i][j];
                i_min = i;
                j_min = j;
            }
        }
    }
    if (vmin == INT_MAX) return;

    zapol[i_min][j_min] = false;
    zapol[sv_i][sv_j] = true;

    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            x[i][j] += znaki[i][j] * vmin;
}

void line() {
    cout << '+';
    for (int j = 0; j <= m; j++) cout << string(8, '-') << '+';
    cout << "\n";
}

void printPlan(const char* title) {
    cout << "\n" << title << "\n";
    line();
    cout << "|" << setw(8) << " " << "|";
    for (int j = 0; j < m; j++) cout << setw(7) << ("B" + to_string(j + 1)) << " |";
    cout << setw(7) << "Поставщик" << " |\n";
    line();
    for (int i = 0; i < n; i++) {
        cout << "|" << setw(7) << ("A" + to_string(i + 1)) << " |";
        for (int j = 0; j < m; j++)
            cout << setw(7) << (zapol[i][j] ? to_string(x[i][j]) : ".") << " |";
        cout << setw(7) << a[i] << " |\n";
        line();
    }
    cout << "|" << setw(7) << "Магазин" << " |";
    for (int j = 0; j < m; j++) cout << setw(7) << b[j] << " |";
    cout << setw(7) << " " << " |\n";
    line();
}

int totalCost() {
    int s = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            s += x[i][j] * c[i][j];
    return s;
}

int main() {
    SZU();
    printPlan("Опорный план:");
    cout << "Стоимость плана: " << totalCost() << "\n";

    int iter = 0;
    while (true) {
        Potencial();
        Ozenka();

        if (optim()) {
            cout << "\nПлан оптимален\n";
            break;
        }

        cout << "\nИтерация " << ++iter << "\n";
        NewPlan();
        printPlan("Новый опорный план:");
    }

    printPlan("ОПТИМАЛЬНЫЙ ПЛАН:");
    cout << "\nМинимальные суммарные затраты: " << totalCost();

    return 0;
}
