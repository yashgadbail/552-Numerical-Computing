#include "RootFinder.h"
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

double g(double x) {  
    return pow(x + 2, 1.0 / 3);
}

int main() {
    int degree;
    cout << "Enter the degree of the polynomial: ";
    cin >> degree;

    vector<double> coefficients(degree + 1);
    cout << "Enter the coefficients from highest to lowest degree:\n";
    for (int i = 0; i <= degree; ++i) {
        cin >> coefficients[i];
    }

    RootFinder solver(coefficients);

    double a, b, x0;
    cout << "Enter the interval [a, b] for Bisection Method: ";
    cin >> a >> b;

    cout << "Enter the initial guess for Newton-Raphson and Fixed-Point: ";
    cin >> x0;

    double rootBisection = solver.bisection(a, b, 1e-6, 100);
    cout << "Bisection Method Root: " << rootBisection << endl;

    double rootNewton = solver.newtonRaphson(x0, 1e-6, 100);
    cout << "Newton-Raphson Root: " << rootNewton << endl;

    double rootFixedPoint = solver.fixedPoint(x0, g, 1e-6, 100);
    cout << "Fixed-Point Iteration Root: " << rootFixedPoint << endl;

    return 0;
}
