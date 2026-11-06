#include <iostream>
#include <cmath>
#include <algorithm>
#include <array>
#include <vector>
#include <iomanip>
#include <bitset>

#define EPSILON 0.00001


template <typename _T, const size_t Rows>
using Matrix = std::array<std::array<_T, Rows>, Rows>;

template <typename _T, const size_t Rows>
using MatrixExtended = std::array<std::array<_T, Rows + 1>, Rows>;

template <typename _T, const size_t Rows>
using VectorSolution = std::array<_T, Rows>;

template <typename _T, const size_t Rows>
bool findAndSwapPivot(
        MatrixExtended<_T, Rows> & matrix,
        const size_t currentRow) {
    size_t pivotRow = currentRow;
    _T maxElement = std::abs(matrix[currentRow][currentRow]);

    for (size_t i = currentRow + 1; i < Rows; i++)
        if (std::abs(matrix[i][currentRow]) > maxElement) {
            maxElement = std::abs(matrix[i][currentRow]);
            pivotRow = i;
        }

    if (maxElement < EPSILON) return false;

    if (pivotRow != currentRow)
        std::swap(matrix[currentRow], matrix[pivotRow]);

    return true;
}

template <typename _T, const size_t Rows>
void forwardGaussianStroke(
        MatrixExtended<_T, Rows> & matrix,
        const size_t currentRow, const _T pivot) {
    for (size_t i{currentRow + 1}; i < Rows; i++) {
        const _T factor = matrix[i][currentRow] / pivot;
        for (size_t j{currentRow}; j <= Rows; j++)
            matrix[i][j] -= factor * matrix[currentRow][j];
    }
}

template <typename _T, const size_t Rows>
void forwardGaussianStroke(
        MatrixExtended<_T, Rows> & matrix) {
    for (size_t i{0}; i < Rows; i++) {
        if (!findAndSwapPivot(matrix, i))
            continue;

        _T pivot = matrix[i][i];

        forwardGaussianStroke(matrix, i, pivot);
    }
}

template <typename _T, const size_t Rows>
bool forwardGaussianStrokeStrict(
        MatrixExtended<_T, Rows> & matrix) {
    for (size_t i{0}; i < Rows; i++) {
        if (!findAndSwapPivot(matrix, i))
            return false;

        _T pivot = matrix[i][i];

        forwardGaussianStroke(matrix, i, pivot);
    }

    return true;
}

template <typename _T, const size_t Rows>
void backwardGaussianStroke(
        const MatrixExtended<_T, Rows> & matrix,
        VectorSolution<_T, Rows> & vector) {
    for (size_t i{Rows}; i-- > 0; ) {
        if (abs(matrix[i][i]) < EPSILON) continue; 

        vector[i] = matrix[i][Rows];

        for (size_t j{i + 1}; j < Rows; j++)
            vector[i] -= matrix[i][j] * vector[j];

        vector[i] /= matrix[i][i];
    }
}

template <typename _T, const size_t Rows>
VectorSolution<_T, Rows> backwardGaussianStroke(
        const MatrixExtended<_T, Rows> & matrix) {
    VectorSolution<_T, Rows> x;

    backwardGaussianStroke(matrix, x);

    return x;
}

template <const size_t _VARIABLES, const size_t _LIMITS,
        typename = std::enable_if_t<(_VARIABLES < _LIMITS)>>
class System {
protected:
    using Point = std::array<double, _VARIABLES>;

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
        Point x;
        OperatorSeed type;
        Seed(Point _x, OperatorSeed _type) : x{_x}, type{_type} {}
        Seed(Point _x) : x{_x}, type{Max} {}

        Seed() = default;
    };

    using Limits = std::array<Limit, _LIMITS>;

private:
    const System<_VARIABLES, _LIMITS>::Seed m_seed;
    const System<_VARIABLES, _LIMITS>::Limits m_limits;
    double m_result;
    
public:
    System(Seed seed, Limits limits)
            : m_seed{seed}, m_limits{limits} {}

    System(const System<_VARIABLES, _LIMITS>& system)
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

    Point  getSeed(void) const {
        return m_seed.x;
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
class Algebraic final : public System<_VARIABLES, _LIMITS> {
protected:
    using typename System<_VARIABLES, _LIMITS>::Point;

public:
    using Points = std::vector<Point>;
    using System<_VARIABLES, _LIMITS>::System;
    using System<_VARIABLES, _LIMITS>::printResult;
    using System<_VARIABLES, _LIMITS>::getResult;
    using System<_VARIABLES, _LIMITS>::setResult;
    using System<_VARIABLES, _LIMITS>::getLimits;
    using System<_VARIABLES, _LIMITS>::getType;
    using System<_VARIABLES, _LIMITS>::getSeed;
    using typename System<_VARIABLES, _LIMITS>::OperatorSeed;
    using typename System<_VARIABLES, _LIMITS>::OperatorLimit;
    using typename System<_VARIABLES, _LIMITS>::Seed;
    using typename System<_VARIABLES, _LIMITS>::Limit;
    using typename System<_VARIABLES, _LIMITS>::Limits;

private:
    using Table = std::array<std::array<double, _VARIABLES + 1>, _VARIABLES>;
    using ChosenIndices = std::array<size_t, _VARIABLES>;

    Point m_targetPoint;
    Points m_points;

public:
    Algebraic(const System<_VARIABLES, _LIMITS> & system) :
        System<_VARIABLES, _LIMITS>{system} {}

    void printInfo(void) const override {
        std::cout << "Algebraic method:" << std::endl;
        std::cout << "Count points: " << m_points.size() << std::endl;

        for (const Point& point : m_points) {
            double currentRoot{calculateRoot(point)};
            std::cout << "Point (\n\t";

            for (const double & x : point)
                std::cout << x << "; ";

            std::cout << ") -> L = " << currentRoot << std::endl;
        }
    }

    Point getTargetPoint(void) {
        return m_targetPoint;
    }

    void calculate(void) override {
        m_points.clear();

        if (_LIMITS < _VARIABLES) {
            findPoint();
            return;
        }

        std::vector bitmask(_LIMITS, false);
        std::fill(bitmask.begin(), bitmask.begin() + _VARIABLES, true);

        do {
            ChosenIndices chosenIndices;
            for (size_t i{0}, j{0}; i < _LIMITS; ++i)
                if (bitmask[i]) chosenIndices[j++] = i;

            Point intersection;
            if (findIntersection(chosenIndices, intersection))
                if (checkConstraints(intersection))
                    m_points.push_back(intersection);

        } while (std::prev_permutation(bitmask.begin(), bitmask.end())); 

        findPoint();
    }

private:
    void setTargetPoint(Point point) {
        m_targetPoint = point;
    }

    double calculateRoot(Point point) const {
        double root{0.0};

        const Point seed {getSeed()};
        for (size_t i{0}; i < _VARIABLES; i++)
            root += seed[i] * point[i];
        return root;
    }

    MatrixExtended<double, _VARIABLES> getMatrixExtended(
            const ChosenIndices & chosenIndices) {
        const Limits limits{getLimits()};
        
        MatrixExtended<double, _VARIABLES> matrix;

        for (size_t i = 0; i < _VARIABLES; i++) {
            size_t limitIdx = chosenIndices[i];
            for (size_t j = 0; j <= _VARIABLES; j++) {
                matrix[i][j] = limits[limitIdx].x[j];
            }
        }

        return matrix;
    }

    bool findIntersection(const ChosenIndices & chosenIndices, Point & outPoint) {
        MatrixExtended<double, _VARIABLES> matrix{
            getMatrixExtended(chosenIndices)
        };

        if (!forwardGaussianStrokeStrict(matrix)) return false;

        backwardGaussianStroke(matrix, outPoint);

        return true;
    }

    bool checkConstraints(const Point & point) const {
        for (const Limit & limit : getLimits()) {
            double value{0.0};

            for (size_t i = 0; i < _VARIABLES; i++)
                value += limit.x[i] * point[i];

            double rhs{limit.x[_VARIABLES]}; 

            switch (limit.type) {
                case OperatorLimit::Less:
                    if (value > rhs + EPSILON) return false;
                    break;
                case OperatorLimit::More:
                    if (value < rhs - EPSILON) return false;
                    break;
                case OperatorLimit::Equal:
                    if (abs(value - rhs) > EPSILON) return false;
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

    void findMinPoint(void) {
        if (m_points.size() == 0) return;
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

    void findMaxPoint(void) {
        if (m_points.size() == 0) return;
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

template<const size_t _VARIABLES, const size_t _LIMITS>
class Symplex final : public System<_VARIABLES, _LIMITS> {
protected:
    using typename System<_VARIABLES, _LIMITS>::Point;

public:
    using System<_VARIABLES, _LIMITS>::System;
    using System<_VARIABLES, _LIMITS>::printResult;
    using System<_VARIABLES, _LIMITS>::getResult;
    using System<_VARIABLES, _LIMITS>::setResult;
    using System<_VARIABLES, _LIMITS>::getLimits;
    using System<_VARIABLES, _LIMITS>::getType;
    using System<_VARIABLES, _LIMITS>::getSeed;
    using typename System<_VARIABLES, _LIMITS>::OperatorSeed;
    using typename System<_VARIABLES, _LIMITS>::OperatorLimit;
    using typename System<_VARIABLES, _LIMITS>::Point;
    using typename System<_VARIABLES, _LIMITS>::Seed;
    using typename System<_VARIABLES, _LIMITS>::Limit;
    using typename System<_VARIABLES, _LIMITS>::Limits;

private:
    using Table = std::array<std::vector<double>, _LIMITS + 1>;
    using Basis = std::array<size_t, _LIMITS>;
    using BasisNames = std::vector<std::string>;
    struct Iteration {
        Table table;
        Basis basis;
        BasisNames basisName;
    }
    using Iterations = std::vector<Iteration>;

    Iterations m_iterations;
    size_t m_totalCols{0};
    size_t m_totalVars{0};
    Point m_optimalPoint{};
    bool m_isOptimalFound{false};
    
    Table m_table;
    Basis m_basis;

public:
    Symplex(const System<_VARIABLES, _LIMITS> & system) :
        System<_VARIABLES, _LIMITS>{system} {}

    void printInfo(void) const override {
        if (!m_isOptimalFound) {
            std::cout << "Optimal solution not found (infeasible or unbounded)." << std::endl;
            return;
        }

        std::cout << "Symplex method:" << std::endl;

        std::cout << "=== Final Simplex Table ===" << std::endl;
        size_t totalCols = m_table.empty() ? 0 : _LIMITS + _VARIABLES + 1;

        std::cout << "Basis\t";
        for (size_t j = 0; j < totalCols - 1; ++j) {
            std::cout << "x" << j + 1 << "\t";
        }
        std::cout << "| b" << std::endl;
        std::cout << "--------------------------------------------------" << std::endl;

        for (size_t i = 0; i < _LIMITS; ++i) {
            std::cout << "x" << m_basis[i] + 1 << "\t";
            for (size_t j = 0; j < totalCols; ++j) {
                std::cout << m_table[i][j] << "\t";
            }
            std::cout << std::endl;
        }

        std::cout << "Delta\t";
        for (size_t j = 0; j < totalCols; ++j) {
            std::cout << m_table[_LIMITS][j] << "\t";
        }
        std::cout << "\n==================================================" << std::endl;

        std::cout << "Optimal Point (\n\t";
        for (size_t i{0}; i < _VARIABLES; ++i) {
            std::cout << m_optimalPoint[i] << "; ";
        }
        std::cout << "\n) -> L = " << getResult() << std::endl;
    }

    void calculate(void) override {
        const Limits limits{getLimits()};
        Point seed{getSeed()};
        bool isMin{false};

        switch (getType()) {
            case OperatorSeed::Min:
                isMin = true;
                for (size_t i{0}; i < _VARIABLES; ++i)
                    seed[i] = -seed[i];
                break;
        }

        for (size_t i{0}; i < _LIMITS; ++i) {
            const Limit& lim = limits[i];
            for (size_t j = 0; j < _VARIABLES; ++j) {
                m_table[i][j] = lim.x[j];
            }
            m_table[i][_VARIABLES + _LIMITS] = lim.x[_VARIABLES];

            m_table[i][_VARIABLES + i] = 1.0;
            m_basis[i] = _VARIABLES + i;
        }

        for (size_t j = 0; j < _VARIABLES; ++j) {
            m_table[_LIMITS][j] = -seed[j];
        }

        while (true) {
            int pivotCol = -1;
            double minEval = -EPSILON;
            for (size_t j = 0; j < _VARIABLES + _LIMITS; ++j) {
                if (m_table[_LIMITS][j] < minEval) {
                    minEval = m_table[_LIMITS][j];
                    pivotCol = j;
                }
            }

            if (pivotCol == -1) {
                m_isOptimalFound = true;
                break;
            }

            int pivotRow = -1;
            double minRatio = 1e30;
            for (size_t i = 0; i < _LIMITS; ++i) {
                if (m_table[i][pivotCol] > EPSILON) {
                    double ratio = m_table[i][_VARIABLES + _LIMITS] / m_table[i][pivotCol];
                    if (ratio < minRatio) {
                        minRatio = ratio;
                        pivotRow = i;
                    }
                }
            }

            if (pivotRow == -1) {
                m_isOptimalFound = false;
                return;
            }

            m_basis[pivotRow] = pivotCol;
            double pivotVal = m_table[pivotRow][pivotCol];

            for (size_t j = 0; j <= _VARIABLES + _LIMITS; ++j) {
                m_table[pivotRow][j] /= pivotVal;
            }

            for (size_t i = 0; i <= _LIMITS; ++i) {
                if ((int)i != pivotRow) {
                    double factor = m_table[i][pivotCol];
                    for (size_t j = 0; j <= _VARIABLES + _LIMITS; ++j) {
                        m_table[i][j] -= factor * m_table[pivotRow][j];
                    }
                }
            }
        }

        for (size_t i = 0; i < _VARIABLES; ++i) {
            m_optimalPoint[i] = 0.0;
        }

        for (size_t i = 0; i < _LIMITS; ++i) {
            if (m_basis[i] < _VARIABLES) {
                m_optimalPoint[m_basis[i]] = m_table[i][_VARIABLES + _LIMITS];
            }
        }

        double finalL = m_table[_LIMITS][_VARIABLES + _LIMITS];
        if (isMin) {
            finalL = -finalL;
        }

        setResult(finalL);
    }
};

template<const size_t _VARIABLES, const size_t _LIMITS>
class Graphical final : public System<_VARIABLES, _LIMITS> {
public:
    using System<_VARIABLES, _LIMITS>::System;
    using System<_VARIABLES, _LIMITS>::printResult;

    Graphical(const System<_VARIABLES, _LIMITS> & system) :
        System<_VARIABLES, _LIMITS>{system} {}

    void printInfo(void) const override {

    }

    void calculate(void) override {

    }
}; 

template <const size_t _VARIABLES, const size_t _LIMITS>
using Seed = System<_VARIABLES, _LIMITS>::Seed;

template <const size_t _VARIABLES, const size_t _LIMITS>
using Limit = System<_VARIABLES, _LIMITS>::Limit;

template <const size_t _VARIABLES, const size_t _LIMITS>
void calculate(System<_VARIABLES, _LIMITS> & system) {
    system.calculate();
    system.printResult();
    system.printInfo();
}

int main(void) {
    // 30*x1 + 40*x2
    // 1) 14*x1 + 4*x2 <= 252
    // 2) 4*x1 - 4*x2 <= 120
    // 3) 2*x1 + 12*x2 <= 240
    Algebraic<2, 3> algebraic {
        Seed<2, 3> {{30, 40}},
        {
            Limit<2, 3> {{14.0, 4.0, 252.0}},
            Limit<2, 3> {{4.0, 4.0, 120.0}},
            Limit<2, 3> {{2.0, 12.0, 240.0}}
        }
    };

    calculate(algebraic);

    Symplex<2, 3> symplex{algebraic};

    calculate(symplex);

    Graphical<2, 3> graphical{symplex};

    calculate(graphical);

    return 0;
}
