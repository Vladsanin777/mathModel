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
        double x[_VARIABLES]{};

        Point(double _x) : x{_x} {}
    };

    enum OperatorSeed {
        Min, Max
    };

    enum OperatorLimit {
        Less, More, Equal
    };

    struct Limit {
        double x[_VARIABLES + 1]{};
        OperatorLimit type;
        Limit(double _x[_VARIABLES], OperatorLimit _type)
                : x{_x}, type{_type} {}
        Limit(double _x[_VARIABLES]) : x{_x}, type{Less} {}
    };

    struct Seed {
        double x[_VARIABLES]{};
        OperatorSeed type;
        Seed(double _x[_VARIABLES], OperatorSeed _type)
                : x{_x}, type{_type} {}
        Seed(double _x[_VARIABLES]) : x{_x}, type{Max} {}
    };

    using Limits = Limit[_LIMITS];
    using Points = Point[_LIMITS - 1];

private:
    const Seed m_seed;
    const Limits m_limits;
    double m_targetRoot;
    Point m_targetPoint;
    
public:
    MathSystem(Seed seed, Limits limits)
            : m_seed{seed}, m_limits{limits} { init(); }

    MathSystem(const MathSystem& system)
            : m_seed{system.m_seed}, m_limits{system.m_limits} { init(); }

    void init(void) {
        calculate();
        findPoint();
    }

    void printTargetPoint() {
        std::cout << "The target point (\n\t";

        for (const double & x : m_targetPoint.x)
            std::cout << x << "; ";

        std::cout << ")\nL = " << m_targetRoot << std::endl;
    }

    virtual void printInfo(void) = 0;

protected:
    const Limits * getLimits(void) const {
        return m_limits;
    }

private:
    bool checkConstraints(const Point& point) const {
        for (const Limit & limit : m_limits) {
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

    double calculateRoot(const Point& point) const {
        double root{0.0};
        for (size_t i = 0; i < _VARIABLES; i++)
            root = m_seed.x[i] * point.x[i];
        return root;
    }

    virtual void findPoint(void) {
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

    virtual void calculate(void) = 0;

    virtual void findMinPoint(void) = 0;

    virtual void findMaxPoint(void) = 0;
};

template<const size_t _VARIABLES, const size_t _LIMITS>
class Graphical final : public MathSystem<_VARIABLES, _LIMITS> {
public:
    using MathSystem<_VARIABLES, _LIMITS>::MathSystem;
    using typename MathSystem<_VARIABLES, _LIMITS>::getLimits;
    using typename MathSystem<_VARIABLES, _LIMITS>::Points;
    using typename MathSystem<_VARIABLES, _LIMITS>::Point;
    using typename MathSystem<_VARIABLES, _LIMITS>::Limits;

private:
    Points m_points;

public:
    void printInfo(void) override {
        std::cout << "Count points: " << m_points.size() << std::endl;

        for (const Point& point : m_points) {
            double currentRoot = calculateRoot(point);
            std::cout << "Point (\n\t";

            for (const double & x : point.x)
                std::cout << x << "; ";

            std::cout << ") -> L = " << currentRoot << std::endl;
        }
    }

private:
    void calculate(void) override {
        for (size_t i{0}; i < _LIMITS; ++i)
            for (size_t j{i + 1}; j < _LIMITS; ++j) {
                Point intersection{};
                if (findIntersection(intersection))
                    if (checkConstraints(intersection))
                        m_points.push_back(intersection);
            }
    }

    bool findIntersection(Point& outPoint) {
        const Limits * const limits{getLimits()};
        
        double mat[_LIMITS][_LIMITS + 1] {};
        for (size_t i = 0; i < _LIMITS; i++) {
            if (_VARIABLES + 1 < _LIMITS + 1) return false;
            for (size_t j = 0; j < _LIMITS + 1; j++)
                mat[i][j] = limits[i].x[j];
        }

        for (size_t i = 0; i < _LIMITS; i++) {
            size_t maxRow = i;
            for (size_t k = i + 1; k < _LIMITS; k++)
                if (std::abs(mat[k][i]) > std::abs(mat[maxRow][i]))
                    maxRow = k;
            
            if (maxRow != i) std::swap(mat[i], mat[maxRow]);

            if (std::abs(mat[i][i]) < EPSILON) return false; 

            for (size_t k = i + 1; k < _LIMITS; ++k) {
                double c = -mat[k][i] / mat[i][i];
                for (size_t j = i; j <= _LIMITS; ++j) {
                    if (i == j) mat[k][j] = 0;
                    else mat[k][j] += c * mat[i][j];
                }
            }
        }

        for (size_t i {_LIMITS - 1}; i >= 0; i--) {
            double sum = mat[i][_LIMITS];
            for (size_t j {i + 1}; j < _LIMITS; ++j)
                sum -= mat[i][j] * outPoint.x[j];
            outPoint.x[i] = sum / mat[i][i];
        }

        return true;
    }

    void findMinPoint() override {
        double targetPoint{m_points[0].x};
        double targetRoot{calculateRoot(targetPoint)};

        for (const Point& point : m_points) {
            double currentRoot{calculateRoot(point)};
            
            if (currentRoot < m_minRoot) {
                targetRoot = currentRoot;
                targetPoint = point;
            }
        }

        setTargetPoint(targetPoint);
        setTargetRoot(targetRoot);
    }

    void findMaxPoint() override {
        double targetPoint{m_points[0]};
        double targetRoot{calculateRoot(targetRoot)};

        for (const Point& point : m_points) {
            double currentRoot{calculateRoot(point)};
            
            if (currentRoot > m_maxRoot) {
                m_targetRoot = currentRoot;
                m_targetPoint = point;
            }
        }

        setTargetPoint(targetPoint);
        setTargetRoot(targetRoot);
    }
};

int main() {
    // 30*x1 + 40*x2
    // 1) 14*x1 + 4*x2 <= 252
    // 2) 4*x1 - 4*x2 <= 120
    // 3) 2*x1 + 12*x2 <= 240
    Symplex symplex{
        {30, 40},
        {
            {14.0, 4.0, 252.0},
            {4.0, 4.0, 120.0},
            {2.0, 12.0, 240.0}
        }
    };

    simplex.print();

    simplex.printInfo();

    Graphical graphical{
        symplex
    };

    graphical.print();

    graphical.printInfo();

    return 0;
}
