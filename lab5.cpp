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

enum Operator {
    Less, More, Equal
};

struct Line {
    double A;
    double B;
    double C;
    Operator type;

    Line(double a, double b, double c, Operator t = Less)
        : A{a}, B{b}, C{c}, type{t} {}
};

struct LineRoot {
    double A;
    double B;
};

using Matrix = std::vector<Line>;
using Points = std::vector<Point>;

class Simplex {
private: LineRoot m_root;
    const Matrix m_matrix;
    Points m_points;
    double m_maxRoot;
    Point m_maxPoint;
    double m_minRoot;
    Point m_minPoint;
    
public:
    Simplex(LineRoot root, Matrix matrix) : m_root{root}, m_matrix{matrix} {
        calculate();
    }

    void printMaxPoint() {
        std::cout << "The maximum is reached at point (" << m_maxPoint.x1 << "; "
                << m_maxPoint.x2 << ")\nL_max = " << m_maxRoot << std::endl;
    }

    void printMinPoint() {
        std::cout << "The minimum is reached at point (" << m_minPoint.x1 << "; "
                << m_minPoint.x2 << ")\nL_max = " << m_minRoot << std::endl;
    }

    void printAll() {
        std::cout << "Count points: " << m_points.size() << std::endl;
        for (const auto& point : m_points) {
            double currentRoot = calculateRoot(point);
            std::cout << "Point (" << point.x1 << "; " << point.x2 << ") -> L = " << currentRoot << std::endl;
        }
    }

private:
    bool checkConstraints(double x1, double x2) {
        for (const Line& line : m_matrix) {
            double value = line.A * x1 + line.B * x2;
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

    void calculate() {
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
        findMinPoint();
        findMaxPoint();
    }

    void findMinPoint() {
        m_minPoint = m_points[0];
        m_minRoot = calculateRoot(m_points[0]);

        for (const auto& point : m_points) {
            double currentRoot = calculateRoot(point);
            
            if (currentRoot < m_minRoot) {
                m_minRoot = currentRoot;
                m_minPoint = point;
            }
        }
    }

    void findMaxPoint() {
        m_maxPoint = m_points[0];
        m_maxRoot = calculateRoot(m_points[0]);

        for (const auto& point : m_points) {
            double currentRoot = calculateRoot(point);
            
            if (currentRoot > m_maxRoot) {
                m_maxRoot = currentRoot;
                m_maxPoint = point;
            }
        }
    }
};

int main() {
    // 30*x1 + 40*x2
    LineRoot root {
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
