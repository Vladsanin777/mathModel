#include <iostream>
#include <cmath>

#define EPSILON 0.001

double func(double x) {
    return x * x * std::log(x + 2);
}

double derivFunc(double x) {
    return 2 * x * std::log(x + 2) + (x * x) / (x + 2);
}

double secondDerivFunc(double x) {
    return (2 * std::log(x + 2)) + ((4 * x) / (x + 2))
            - ((x * x) / ((x + 2) * (x + 2)));
}

void svennSearchMin(double (*f) (double), double x, double h, double &min, double &max) {
    h /= 3;

    if (f(x-h) < f(x) && f(x) > f(x+h)) {
        min = x - h;
        max = x + h;
    }

    if (f(x) > f(x+h))
        for (; f(x) > f(x+h); x += h);
    else
        for (; f(x+h) > f(x); x -= h);

    min = x - h;
    max = x + 2 * h;
}

void svennSearchMax(double (*f) (double), double x, double h, double &min, double & max) {
    h /= 3;

    if (f(x-h) < f(x) && f(x) > f(x+h)) {
        min = x - h;
        max = x + h;
    }

    if (f(x) < f(x+h))
        for (; f(x) < f(x+h); x += h);
    else
        for (; f(x+h) < f(x); x -= h);

    min = x - h;
    max = x + 2 * h;
}

double newtonSearch(double (*f) (double), double (*d) (double), double x) {
    for (double h = f(x) / d(x);
            std::abs(h) >= EPSILON; x -= h,
            h = f(x) / d(x));
    return x;
}

int main(void) {
    double x0{5.5};
    double h{0.1};

    std::cout << "x0: ";
    std::cin >> x0;
    std::cout << "step: ";
    std::cin >> h;

    double xSvennMinA{0.0};
    double xSvennMinB{0.0};

    double xSvennMaxA{0.0};
    double xSvennMaxB{0.0};

    svennSearchMin(derivFunc, x0, h, xSvennMinA, xSvennMinB);

    std::cout << "Svenn min value: " << xSvennMinA << ", " << xSvennMinB << std::endl;

    svennSearchMax(func, (xSvennMinA + xSvennMinB) / 2, h, xSvennMaxA, xSvennMaxB);

    std::cout << "Svenn max value: " << xSvennMaxA << ", " << xSvennMaxB << std::endl;

    double result {
        newtonSearch(derivFunc, secondDerivFunc, (xSvennMaxA + xSvennMaxB) / 2)
    };

    std::cout << "Point extremum x = " << result << std::endl;
    std::cout << "Value function f(x) = " << func(result) << std::endl;

    if (secondDerivFunc(result) < 0) {
        std::cout << "This point is a MAXIMUM" << std::endl;
    }
    else {
        std::cout << "This point is a MINIMUM" << std::endl;
    }

    return 0;
}
