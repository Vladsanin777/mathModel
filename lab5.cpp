#include "System.hpp"

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

    algebraic.calculate();
    algebraic.printResult();
    algebraic.printInfo();

    Symplex<2, 3> symplex {
        algebraic
    };

    symplex.calculate();
    symplex.printResult();
    symplex.printInfo();

    return 0;
}
