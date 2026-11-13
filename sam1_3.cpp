#include "System.hpp"

int main(void) {
    Algebraic<2, 2> algebraic {
        Seed<2, 2> {{120.0, 160.0}, Min},
        {
            Limit<2, 2> {{0.2, 0.4, 2.0}, More},
            Limit<2, 2> {{0.2, 0.2, 4.0}, More},
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
