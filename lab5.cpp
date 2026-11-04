#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <array>
#include <iomanip>

#define EPSILON 0.00001

template <const size_t _VARIABLES, const size_t _LIMITS>
class MathSystem {
public:
    enum OperatorSeed {
        Min, Max
    };

    enum OperatorLimit {
        Less, More, Equal
    };

    struct Limit {
        std::array<double, _VARIABLES + 1> x;
        OperatorLimit type;
        Limit(std::array<double, _VARIABLES + 1> _x,
                OperatorLimit _type) : x{_x}, type{_type} {}
        Limit(std::array<double, _VARIABLES + 1> _x)
            : x{_x}, type{Less} {}

        Limit() = default;
    };

    struct Seed {
        std::array<double, _VARIABLES> x;
        OperatorSeed type;
        Seed(std::array<double, _VARIABLES> _x,
                OperatorSeed _type) : x{_x}, type{_type} {}
        Seed(std::array<double, _VARIABLES> _x) : x{_x}, type{Max} {}

        Seed() = default;
    };

    using Limits = std::array<Limit, _LIMITS>;

private:
    const MathSystem<_VARIABLES, _LIMITS>::Seed m_seed;
    const MathSystem<_VARIABLES, _LIMITS>::Limits m_limits;
    double m_result;
    
public:
    MathSystem(Seed seed, Limits limits)
            : m_seed{seed}, m_limits{limits} {}

    MathSystem(const MathSystem<_VARIABLES, _LIMITS>& system)
            : m_seed{system.m_seed}, m_limits{system.m_limits} {}

    void printResult(void) const {
        std::cout << getResult() << std::endl;
    }

    virtual void printInfo(void) const = 0;

    OperatorSeed getType(void) const {
        return m_seed.type;
    }

    double getResult(void) const {
        return m_result;
    }

    const Seed * getSeed(void) const {
        return &m_seed;
    }

    const Limits getLimits(void) const {
        return m_limits;
    }

    virtual void calculate(void) = 0;

protected:
    void setResult(double result) {
        m_result = result;
    }
};

template<const size_t _VARIABLES, const size_t _LIMITS>
class Symplex final : public MathSystem<_VARIABLES, _LIMITS> {
public:
    using MathSystem<_VARIABLES, _LIMITS>::MathSystem;
    using MathSystem<_VARIABLES, _LIMITS>::printResult;

    Symplex(const MathSystem<_VARIABLES, _LIMITS> & system) :
        MathSystem<_VARIABLES, _LIMITS>{system} {}

    void printInfo(void) const override {

    }

    void calculate(void) override {

    }
};

template<const size_t _VARIABLES, const size_t _LIMITS>
class Graphical final : public MathSystem<_VARIABLES, _LIMITS> {
public:
    struct Point {
        std::array<double, _VARIABLES> x;

        Point(std::array<double, _VARIABLES> _x) : x{_x} {}

        Point() = default;
    };

    using Points = std::vector<Point>;
    using MathSystem<_VARIABLES, _LIMITS>::MathSystem;
    using MathSystem<_VARIABLES, _LIMITS>::printResult;
    using MathSystem<_VARIABLES, _LIMITS>::getResult;
    using MathSystem<_VARIABLES, _LIMITS>::setResult;
    using MathSystem<_VARIABLES, _LIMITS>::getLimits;
    using MathSystem<_VARIABLES, _LIMITS>::getType;
    using MathSystem<_VARIABLES, _LIMITS>::getSeed;
    using typename MathSystem<_VARIABLES, _LIMITS>::OperatorSeed;
    using typename MathSystem<_VARIABLES, _LIMITS>::OperatorLimit;
    using typename MathSystem<_VARIABLES, _LIMITS>::Seed;
    using typename MathSystem<_VARIABLES, _LIMITS>::Limit;
    using typename MathSystem<_VARIABLES, _LIMITS>::Limits;

private:
    Point m_targetPoint;
    Points m_points;

public:
    Graphical(const MathSystem<_VARIABLES, _LIMITS> & system) :
        MathSystem<_VARIABLES, _LIMITS>{system} {}

    void printInfo(void) const override {
        std::cout << "Count points: " << m_points.size() << std::endl;

        for (const Point& point : m_points) {
            double currentRoot{calculateRoot(point)};
            std::cout << "Point (\n\t";

            for (const double & x : point.x)
                std::cout << x << "; ";

            std::cout << ") -> L = " << currentRoot << std::endl;
        }
    }

    Point getTargetPoint(void) {
        return m_targetPoint;
    }

    void calculate(void) override {
        for (size_t i{0}; i < _LIMITS; ++i)
            for (size_t j{i + 1}; j < _LIMITS; ++j) {
                Point intersection{};
                if (findIntersection(intersection))
                    if (checkConstraints(intersection))
                        m_points.push_back(intersection);
            }

        findPoint();
    }

private:
    void setTargetPoint(Point point) {
        m_targetPoint = point;
    }

    double calculateRoot(const Point & point) const {
        double root{0.0};

        const Seed * const seed {getSeed()};
        for (size_t i = 0; i < _VARIABLES; i++)
            root += getSeed()->x[i] * point.x[i];
        return root;
    }

    bool findIntersection(Point & outPoint) {
        const Limits limits{getLimits()};
        
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

    bool checkConstraints(const Point & point) const {
        for (const Limit & limit : getLimits()) {
            double value{0.0};

            for (size_t i = 0; i < _VARIABLES; i++)
                value += limit.x[i] * point.x[i];

            switch (limit.type) {
                case OperatorLimit::Less:
                    if (value > limit.x[limit.x.size() - 1] + EPSILON)
                        return false;
                    break;
                case OperatorLimit::More:
                    if (value < limit.x[limit.x.size() - 1] - EPSILON)
                        return false;
                    break;
                case OperatorLimit::Equal:
                    if (abs(value - limit.x[limit.x.size() - 1]) > EPSILON)
                        return false;
                    break;
            }
        }
        return true;
    }

    void findPoint(void) {
        switch (getType()) {
            case OperatorSeed::Min:
                findMinPoint();
                break;
            case OperatorSeed::Max:
                findMaxPoint();
                break;
        }
    }

    void findMinPoint() {
        Point targetPoint{m_points[0]};
        double targetRoot{calculateRoot(targetPoint)};

        for (const Point& point : m_points) {
            double currentRoot{calculateRoot(point)};
            
            if (currentRoot < targetRoot) {
                targetRoot = currentRoot;
                targetPoint = point;
            }
        }

        setTargetPoint(targetPoint);
        setResult(targetRoot);
    }

    void findMaxPoint() {
        Point targetPoint{m_points[0]};
        double targetRoot{calculateRoot(targetPoint)};

        for (const Point& point : m_points) {
            double currentRoot{calculateRoot(point)};
            
            if (currentRoot > targetRoot) {
                targetRoot = currentRoot;
                targetPoint = point;
            }
        }

        setTargetPoint(targetPoint);
        setResult(targetRoot);
    }
};

int main() {
    // 30*x1 + 40*x2
    // 1) 14*x1 + 4*x2 <= 252
    // 2) 4*x1 - 4*x2 <= 120
    // 3) 2*x1 + 12*x2 <= 240
    Symplex<2, 3> symplex {
        Symplex<2, 3>::Seed {{30, 40}},
        {
            Symplex<2, 3>::Limit {{14.0, 4.0, 252.0}},
            Symplex<2, 3>::Limit {{4.0, 4.0, 120.0}},
            Symplex<2, 3>::Limit {{2.0, 12.0, 240.0}}
        }
    };

    symplex.calculate();

    symplex.printResult();

    symplex.printInfo();

    Graphical<2, 3> graphical {
        symplex
    };

    graphical.calculate();

    graphical.printResult();

    graphical.printInfo();

    return 0;
}
