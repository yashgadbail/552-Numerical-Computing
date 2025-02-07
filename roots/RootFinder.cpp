#include "RootFinder.h"
#include <iostream>
#include <cmath>

using namespace std;

RootFinder::RootFinder(const vector<double>& coefficients) : coeffs(coefficients) {}

pair<double, double> RootFinder::findBisectionInterval(double start, double end, double step) {
    double a = start, b = start + step;
    while (b <= end) {
        if (evaluate(a) * evaluate(b) < 0) {
            return {a, b};
        }
        a = b;
        b += step;
    }
    return {NAN, NAN};
}
double RootFinder::findInitialGuess(double a, double b) {
    return (a + b) / 2;
}
double RootFinder::evaluate(double x) {
    double result = 0;
    for (size_t i = 0; i < coeffs.size(); ++i) {
        result += coeffs[i] * pow(x, coeffs.size() - 1 - i);
    }
    return result;
}

double RootFinder::evaluateDerivative(double x) {
    double result = 0;
    for (size_t i = 0; i < coeffs.size() - 1; ++i) {
        result += (coeffs.size() - 1 - i) * coeffs[i] * pow(x, coeffs.size() - 2 - i);
    }
    return result;
}

double RootFinder::bisection(double a, double b, double tol, int maxIter) {
    if (evaluate(a) * evaluate(b) >= 0) {
        cerr << "Invalid interval: f(a) and f(b) must have opposite signs.\n";
        return NAN;
    }

    double c;
    for (int i = 0; i < maxIter; ++i) {
        c = (a + b) / 2;
        double fc = evaluate(c);

        if (fabs(fc) < tol) break;
        if (evaluate(a) * fc < 0) b = c;
        else a = c;
    }
    return c;
}

double RootFinder::newtonRaphson(double x0, double tol, int maxIter) {
    double x = x0;
    for (int i = 0; i < maxIter; ++i) {
        double fx = evaluate(x);
        double dfx = evaluateDerivative(x);

        if (fabs(dfx) < 1e-10) {
            cerr << "Derivative too small, stopping iteration.\n";
            return NAN;
        }

        double x_new = x - fx / dfx;
        if (fabs(x_new - x) < tol) return x_new;
        x = x_new;
    }
    return x;
}

double RootFinder::fixedPoint(double x0, function<double(double)> g, double tol, int maxIter) {
    double x = x0;
    for (int i = 0; i < maxIter; ++i) {
        double x_new = g(x);
        if (fabs(x_new - x) < tol) return x_new;
        x = x_new;
    }
    return x;
}
