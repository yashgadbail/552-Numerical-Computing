#include "RootFinder.h"
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

double g(double x) {  
    return pow(x + 2, 1.0 / 3);
}

int main() {
    int degree= 3;
    const double precision = 1e-6;
    const int maxIter = 100;

    // cout << "Enter the degree of the polynomial: ";
    // cin >> degree;

    // vector<double> coefficients(degree + 1);
    // cout << "Enter the coefficients from highest to lowest degree:\n";
    // for (int i = 0; i <= degree; ++i) {
    //     cin >> coefficients[i];
    // }
    vector<double> coefficients = {1, 0, -1, -2}; 
    cout << "Polynomial: ";
    for (int i = 0; i <= degree; ++i) {
        if (coefficients[i] != 0) {
            if(degree-i == 0){
                cout << coefficients[i] << " ";
            }
            else if(coefficients[i] == 1){
                cout << " + x^" << (degree - i) << " ";
            }
            else if(coefficients[i] == -1){
                cout << " - x^" << (degree - i) << " ";
            }
            else {
                cout << coefficients[i] << "x^" << (degree - i) << " ";
            }
        }
    }
    cout << "= 0" << endl;

    RootFinder solver(coefficients);

    pair<double, double> interval = solver.findBisectionInterval(-100, 100, 0.1);
    if (isnan(interval.first)) {
        cout << "No interval found for Bisection Method.\n";
        return 1;
    }

    double a = interval.first;
    double b = interval.second;
    cout << "--> Bisection Interval: ["<< a <<", "<< b << "]\n";

    double x0 = solver.findInitialGuess(a, b);

    double rootBisection = solver.bisection(a, b,precision,maxIter);
    cout << " > Bisection Method Root: " << rootBisection << endl;

    double rootNewton = solver.newtonRaphson(x0,precision,maxIter);
    cout <<" > Newton-Raphson Root: "<< rootNewton << endl;

    double rootFixedPoint = solver.fixedPoint(x0, g,precision,maxIter);
    cout <<" > Fixed-Point Iteration Root: "<< rootFixedPoint<< endl;

    return 0;
}
