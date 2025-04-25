#ifndef COMPARISON_H
#define COMPARISON_H

#include "interpolation.h"
#include "tchebyshev.h"
#include <string>

class Comparison {
private:
    LagrangeInterpolation lagrange;
    SplineInterpolation spline;
    TchebyshevPolynomial tchebyshev;
    
public:
    Comparison();
    ~Comparison();
    
    // Load data into all methods
    void loadDataFromConsole();
    void loadDataFromFile(const std::string& filename);
    
    // Prepare the methods
    void prepareAll();
    
    // Compare methods and generate results
    void compareAndGenerate(double start, double end, int points);
    
    // Generate comparison plot with gnuplot
    void generateComparisonPlot() const;
};

#endif // COMPARISON_H