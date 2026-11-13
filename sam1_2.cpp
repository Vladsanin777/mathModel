#include "System.hpp"

int main(void) {
    Algebraic<2, 3> algebraic {
        Seed<2, 3> {{300.0, 200.0}},
        {
            Limit<2, 3> {{0.2, 0.1, 100.0}},
            Limit<2, 3> {{0.2, 0.5, 180.0}},
            Limit<2, 3> {{0.1, 0.2, 100.0}}
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
