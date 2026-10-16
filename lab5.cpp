#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>

#define EPSILON 0.00001

struct Point {
    double x1;
    double x2;
};

enum OperatorSeed {
    Min, Max
};

enum Operator {
    Less, More, Equal
};

struct Limit {
    std::vector<double> x;
    Operator type;

    Line(std::vector<double> & _x, Operator t = Less)
        : x{_x}, type{t} {}
};

struct Seed {
    std::vector<double> x;
    OperatorSeed type;

    Line(std::vector<double> & _x, OperatorSeed t = Max)
        : x{xs}, type{t} {}
};

using Matrix = std::vector<Line>;
using Points = std::vector<Point>;

class MathSystem {
private:
    LineSeed m_seed;
    const Matrix m_matrix;
    Points m_points;
    double m_targetRoot;
    Point m_targetPoint;
    
public:
    MathSystem(LineSeed root, Matrix matrix) : m_root{root}, m_matrix{matrix} {}

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

        for (const Line& line : m_matrix) {
            for (size_t i = 0; i < )
            double value = line.x[0] * x[0] + line.x[1] * x[1];
            switch (line.type) {
                case Less:
                    if (value > line.C + EPSILON) return false;
                    break;
                case More:
                    if (value < line.C - EPSILON) return false;
                    break;
                case Equal:
                    if (abs(value - line.C) > EPSILON) return false;
                    break;
            }
        }
        return true;
    }

    static bool findIntersection(Line line1, Line line2, Point& outPoint) {
        double det = line1.A * line2.B - line1.B * line2.A;
        
        if (std::abs(det) < EPSILON) return false;
        
        outPoint.x1 = (line1.C * line2.B - line1.B * line2.C) / det;
        outPoint.x2 = (line1.A * line2.C - line1.C * line2.A) / det;
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
    LineSeed root {
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
