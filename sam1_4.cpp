#include "System.hpp"

int main(void) {
    Algebraic<2, 3> algebraic {
        Seed<2, 3> {{20.0, 10.0}},
        {
            Limit<2, 3> {{2000.0, 1000.0, 7500.0}, Equal},
            Limit<2, 3> {{12000.0, 7000.0, 60000.0}, Equal},
            Limit<2, 3> {{125.0, 100.0, 600.0}, Less},
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
