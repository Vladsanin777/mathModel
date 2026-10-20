#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>

#define EPSILON 0.00001

template<const size_t _VARIABLES, const size_t _LIMITS>
class MathSystem {
public:
    struct Point {
        double x[_VARIABLES] = {0};
    };

    enum OperatorSeed {
        Min, Max
    };

    enum OperatorLimit {
        Less, More, Equal
    };

    struct Limit {
        double x[_VARIABLES + 1] = {0};
        OperatorLimit type;
    };

    struct Seed {
        double x[_VARIABLES] = {0};
        OperatorSeed type;
    };

    using Limits = Limit[_LIMITS];
    using Points = Point[_LIMITS - 1];

private:
    const Seed m_seed;
    const Limits m_limits;
    Points m_points;
    double m_targetRoot;
    Point m_targetPoint;
    
    MathSystem(Seed seed, Limits limits) : m_seed{seed}, m_limits{limits} {}

    void printTargetPoint() {
        std::cout << "The target point (\n\t"

        for (const double & x : point.x)
            std::cout << point.x[i] << "; ";

        std::cout << ")\nL = " << m_maxRoot << std::endl;
    }

    void printAll() {
        std::cout << "Count points: " << m_points.size() << std::endl;

        for (const auto& point : m_points) {
            double currentRoot = calculateRoot(point);
            std::cout << "Point (\n\t";

            for (const double & x : point.x)
                std::cout << point.x[i] << "; ";

            std::cout << ") -> L = " << currentRoot << std::endl;
        }
    }

private:
    bool checkConstraints(std::vector<double> x) {
        for (const Limit & limit : m_matrix) {
            double value{0.0};
            for (size_t i = 0; i < x.size(); i++)
                value += limit.x[i] * x[i];
            switch (limit.type) {
                case Less:
                    if (value > limit.x[limit.x.size() - 1] + EPSILON)
                        return false;
                    break;
                case More:
                    if (value < limit.x[limit.x.size() - 1] - EPSILON)
                        return false;
                    break;
                case Equal:
                    if (abs(value - limit.x[limit.x.size() - 1]) > EPSILON)
                        return false;
                    break;
            }
        }
        return true;
    }

    static bool findIntersection(const std::vector<Limit>& limits, Point& outPoint) {
        if (limits.empty()) return false;
        
        size_t n = _LIMITS;
        
        std::vector<std::vector<double>> mat(n, std::vector<double>(n + 1));
        for (size_t i = 0; i < n; ++i) {
            if (limits[i].coeff.size() < n + 1) return false; // Проверка корректности данных
            for (size_t j = 0; j <= n; ++j) {
                mat[i][j] = limits[i].coeff[j];
            }
        }

        for (size_t i = 0; i < n; ++i) {
            size_t maxRow = i;
            for (size_t k = i + 1; k < n; ++k) {
                if (std::abs(mat[k][i]) > std::abs(mat[maxRow][i])) {
                    maxRow = k;
                }
            }
            
            if (maxRow != i) {
                std::swap(mat[i], mat[maxRow]);
            }

            if (std::abs(mat[i][i]) < EPSILON) {
                return false; 
            }

            for (size_t k = i + 1; k < n; ++k) {
                double c = -mat[k][i] / mat[i][i];
                for (size_t j = i; j <= n; ++j) {
                    if (i == j) {
                        mat[k][j] = 0;
                    } else {
                        mat[k][j] += c * mat[i][j];
                    }
                }
            }
        }

        outPoint.coords.resize(n);
        for (int i = static_cast<int>(n) - 1; i >= 0; --i) {
            double sum = mat[i][n];
            for (size_t j = i + 1; j < n; ++j) {
                sum -= mat[i][j] * outPoint.coords[j];
            }
            outPoint.coords[i] = sum / mat[i][i];
        }

        return true;
    }

    double calculateRoot(Point p) {
        return m_root.A * p.x1 + m_root.B * p.x2;
    }

    void symplex() {
        for (size_t i = 0; i < m_matrix.size(); ++i) {
            for (size_t j = i + 1; j < m_matrix.size(); ++j) {
                Point intersection;
                if (findIntersection(m_matrix[i], m_matrix[j], intersection)) {
                    if (checkConstraints(intersection.x1, intersection.x2)) {
                        m_points.push_back(intersection);
                    }
                }
            }
        }

        switch (m_seed->type) {
            case Min:
                findMinPoint();
                break;
            case Max:
                findMaxPoint();
                break;
            default:
                break;
        }
    }

    void findMinPoint() {
        m_targetPoint = m_points[0];
        m_targetRoot = calculateRoot(m_points[0]);

        for (const auto& point : m_points) {
            double currentRoot = calculateRoot(point);
            
            if (currentRoot < m_minRoot) {
                m_targetRoot = currentRoot;
                m_targetPoint = point;
            }
        }
    }

    void findMaxPoint() {
        m_targetPoint = m_points[0];
        m_targetRoot = calculateRoot(m_points[0]);

        for (const auto& point : m_points) {
            double currentRoot = calculateRoot(point);
            
            if (currentRoot > m_maxRoot) {
                m_targetRoot = currentRoot;
                m_targetPoint = point;
            }
        }
    }
};

int main() {
    // 30*x1 + 40*x2
    Seed root {
        30.0, 40.0
    };

    // 1) 14*x1 + 4*x2 <= 252
    // 2) 4*x1 - 4*x2 <= 120
    // 3) 2*x1 + 12*x2 <= 240
    Matrix matrix {
        {14.0, 4.0, 252.0},
        {4.0, 4.0, 120.0},
        {2.0, 12.0, 240.0}
    };

    Simplex simplex(root, matrix);

    // Вывожу все точки пересечения
    simplex.printAll();
    // Вывожу самую отдалённую точку пересечения
    simplex.printMaxPoint();
    // Вывожу самую ближнюю точку пересечения
    // simpson.printMinPoint();

    return 0;
}
