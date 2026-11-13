#include "System.hpp"

int main(void) {
    Algebraic<2, 3> algebraic {
        Seed<2, 3> {{250.0, 300.0}},
        {
            Limit<2, 3> {{0.05, 0.1, 20.0}},
            Limit<2, 3> {{0.1, 0.2, 60.0}},
            Limit<2, 3> {{0.3, 0.1, 50.0}}
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
