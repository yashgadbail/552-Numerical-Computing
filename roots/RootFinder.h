#include <vector>
#include <functional>

using namespace std;

class RootFinder {
    public:
        RootFinder(const vector<double>& coefficients);
        
        double bisection(double a, double b, double tol, int maxIter);
        double newtonRaphson(double x0, double tol, int maxIter);
        double fixedPoint(double x0, function<double(double)> g, double tol, int maxIter);

    private:
        vector<double> coeffs;

        double evaluate(double x);
        double evaluateDerivative(double x);
};

