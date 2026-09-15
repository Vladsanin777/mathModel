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

struct Line {
    double A;
    double B;
    double C;
};

bool checkConstraints(double x1, double x2) {
    if (x1 + 2 * x2 > 14 + EPSILON) return false;
    if (3 * x1 - 5 * x2 > 5 + EPSILON) return false;
    if (5 * x1 + 3 * x2 < 21 - EPSILON) return false;
    if (x2 < 2 - EPSILON) return false;
    
    return true;
}

bool findIntersection(Line l1, Line l2, Point& outPoint) {
    double det = l1.A * l2.B - l1.B * l2.A;
    
    if (std::abs(det) < EPSILON) return false;
    
    outPoint.x1 = (l1.C * l2.B - l1.B * l2.C) / det;
    outPoint.x2 = (l1.A * l2.C - l1.C * l2.A) / det;
    return true;
}

double calculateL(Point p) {
    return 2 * p.x1 + 3 * p.x2;
}

int main() {
    std::cout << std::fixed << std::setprecision(4);

    // 1) 1*x1 + 2*x2 = 14
    // 2) 3*x1 - 5*x2 = 5
    // 3) 5*x1 + 3*x2 = 21
    // 4) 0*x1 + 1*x2 = 2
    std::vector<Line> lines = {
        {1.0, 2.0, 14.0},
        {3.0, -5.0, 5.0},
        {5.0, 3.0, 21.0},
        {0.0, 1.0, 2.0}
    };

    std::vector<Point> vertices;

    for (size_t i = 0; i < lines.size(); ++i) {
        for (size_t j = i + 1; j < lines.size(); ++j) {
            Point intersection;
            if (findIntersection(lines[i], lines[j], intersection)) {
                if (checkConstraints(intersection.x1, intersection.x2)) {
                    vertices.push_back(intersection);
                }
            }
        }
    }

    if (vertices.empty()) {
        std::cout << "The feasible region is empty!" << std::endl;
        return 0;
    }

    std::cout << "=== Found vertices of the feasible region and L(x) values ===" << std::endl;
    
    Point maxPoint = vertices[0];
    double maxL = calculateL(vertices[0]);

    for (const auto& v : vertices) {
        double currentL = calculateL(v);
        std::cout << "Point (" << v.x1 << "; " << v.x2 << ") -> L = " << currentL << std::endl;
        
        if (currentL > maxL) {
            maxL = currentL;
            maxPoint = v;
        }
    }

    std::cout << "The maximum is reached at point (" << maxPoint.x1 << "; " << maxPoint.x2 << ")\nL_max = " << maxL << std::endl;

    return 0;
}
