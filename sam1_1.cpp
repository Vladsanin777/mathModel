#include "System.hpp"

int main(void) {
    Algebraic<2, 4> algebraic {
        Seed<2, 4> {{300.0, 200.0}},
        {
            Limit<2, 4> {{2.0, 3.0, 21.0}},
            Limit<2, 4> {{1.0, 0.0, 4.0}},
            Limit<2, 4> {{0.0, 1.0, 6.0}}
            Limit<2, 4> {{2.0, 1.0, 10.0}}
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
