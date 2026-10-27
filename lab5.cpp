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
        Seed(double _x[_VARIABLES], OperatorLimit _type)
                : x{_x}, type{_type} {}
        Seed(double _x[_VARIABLES])
                : x{_x}, type{Less} {}
    };

    struct Seed {
        double x[_VARIABLES] = {0};
        OperatorSeed type;
        Seed(double _x[_VARIABLES], OperatorSeed _type)
                : x{_x}, type{_type} {}
        Seed(double _x[_VARIABLES])
                : x{_x}, type{MAX} {}
    };

    using Limits = Limit[_LIMITS];
    using Points = Point[_LIMITS - 1];

private:
    const Seed m_seed;
    const Limits m_limits;
    Points m_points;
    double m_targetRoot;
    Point m_targetPoint;
    
    MathSystem(Seed seed, Limits limits)
            : m_seed{seed}, m_limits{limits} {}

    MathSystem(const MathSystem& system)
            : m_seed{system.m_seed}, m_limits{system.m_limits} {}

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
    bool checkConstraints(const Point& point) {
        for (const Limit & limit : m_matrix) {
            double value{0.0};
            for (size_t i = 0; i < _VARIABLES; i++)
                value += limit.x[i] * point.x[i];
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

    static bool findIntersection(Point& outPoint) {
        if (limits.empty()) return false;
        
        double mat[_LIMITS][_LIMITS + 1] {};
        for (size_t i = 0; i < _LIMITS; i++) {
            if (limits[i].coeff.size() < _LIMITS + 1) return false;
            for (size_t j = 0; j < _LIMITS + 1; j++) {
                mat[i][j] = limits[i].coeff[j];
            }
        }

        for (size_t i = 0; i < _LIMITS; i++) {
            size_t maxRow = i;
            for (size_t k = i + 1; k < _LIMITS; k++) {
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

        for (size_t i {_LIMITS - 1}; i >= 0; --i) {
            double sum = mat[i][_LIMITS];
            for (size_t j {i + 1}; j < n; ++j) {
                sum -= mat[i][j] * outPoint.x[j];
            }
            outPoint.x[i] = sum / mat[i][i];
        }

        return true;
    }

    double calculateRoot(const Point& point) {
        double root{0.0};
        for (size_t i = 0; i < _VARIABLES; i++)
            seed = m_seed.x[i] * point.x[i];
        return root;
    }

    void symplex() {
        for (size_t i{0}; i < _LIMITS; ++i) {
            for (size_t j{i + 1}; j < _LIMITS; ++j) {
                Point intersection{};
                if (findIntersection(intersection)) {
                    if (checkConstraints(intersection)) {
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
    // 1) 14*x1 + 4*x2 <= 252
    // 2) 4*x1 - 4*x2 <= 120
    // 3) 2*x1 + 12*x2 <= 240
    Matrix matrix ;

    MathSystem symplex{
        {30, 40},
        {
            {14.0, 4.0, 252.0},
            {4.0, 4.0, 120.0},
            {2.0, 12.0, 240.0}
        }
    };

    simplex.symplex();

    // Вывожу все точки пересечения
    simplex.printAll();
    // Вывожу самую отдалённую точку пересечения
    simplex.printMaxPoint();
    // Вывожу самую ближнюю точку пересечения
    // simpson.printMinPoint();

    MathSystem graf{
        symplex
    };

    graf.graf();

    // Вывожу все точки пересечения
    graf.printAll();
    // Вывожу самую отдалённую точку пересечения
    graf.printMaxPoint();
    // Вывожу самую ближнюю точку пересечения
    // simpson.printMinPoint();

    return 0;
}
