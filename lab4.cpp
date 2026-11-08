#include "System.hpp"

int main() {
    // 2*x1 + 3*x2
    // 1) 1*x1 + 2*x2 <= 14
    // 2) 3*x1 - 5*x2 <= 5
    // 3) 5*x1 + 3*x2 >= 21
    // 4) 0*x1 + 1*x2 >= 2
    Algebraic<2, 4> algebraic {
        Seed<2, 4> {{2.0, 3.0}, Max},
        {
            Limit<2, 4> {{1.0, 2.0, 14.0}, Less},
            Limit<2, 4> {{3.0, -5.0, 5.0}, Less},
            Limit<2, 4> {{5.0, 3.0, 21.0}, More},
            Limit<2, 4> {{0.0, 1.0, 2.0}, More}
        }
    };

    algebraic.calculate();
    algebraic.printResult();
    algebraic.printInfo();

    Symplex<2, 4> symplex {
        algebraic
    };

    symplex.calculate();
    symplex.printResult();
    symplex.printInfo();

    return 0;
}
