#include "interpolation.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>

// Base Interpolation class
Interpolation::Interpolation() : n(0) {}

Interpolation::~Interpolation() {}

void Interpolation::loadDataFromConsole() {
    std::cout << "Enter number of data points: ";
    std::cin >> n;
    
    x_points.resize(n);
    y_points.resize(n);
    
    std::cout << "Enter data points (x y):" << std::endl;
    for (int i = 0; i < n; i++) {
        std::cout << "Point " << i + 1 << ": ";
        std::cin >> x_points[i] >> y_points[i];
    }
}

void Interpolation::loadDataFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Failed to open file: " << filename << std::endl;
        return;
    }
    
    x_points.clear();
    y_points.clear();
    
    double x, y;
    while (file >> x >> y) {
        x_points.push_back(x);
        y_points.push_back(y);
    }
    
    n = x_points.size();
    std::cout << "Loaded " << n << " data points from file." << std::endl;
    
    file.close();
}

void Interpolation::saveResultsToFile(const std::string& filename, const std::vector<double>& x_eval, 
                                     const std::vector<double>& y_eval) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Failed to open file for writing: " << filename << std::endl;
        return;
    }
    
    for (size_t i = 0; i < x_eval.size(); i++) {
        file << x_eval[i] << " " << y_eval[i] << std::endl;
    }
    
    file.close();
    std::cout << "Results saved to file: " << filename << std::endl;
}

void Interpolation::displayData() const {
    std::cout << "Data points:" << std::endl;
    for (int i = 0; i < n; i++) {
        std::cout << "(" << x_points[i] << ", " << y_points[i] << ")" << std::endl;
    }
}

std::vector<double> Interpolation::evaluateRange(double start, double end, int points) const {
    std::vector<double> result(points);
    double step = (end - start) / (points - 1);
    
    for (int i = 0; i < points; i++) {
        double x = start + i * step;
        result[i] = evaluate(x);
    }
    
    return result;
}

void Interpolation::generateGnuplotScript(const std::string& dataFile, const std::string& scriptFile,
                                         const std::string& plotTitle) const {
    std::ofstream script(scriptFile);
    if (!script.is_open()) {
        std::cout << "Failed to create gnuplot script file." << std::endl;
        return;
    }
    
    script << "set terminal wxt size 800,600\n";
    script << "set title '" << plotTitle << "'\n";
    script << "set xlabel 'X'\n";
    script << "set ylabel 'Y'\n";
    script << "set grid\n";
    script << "set key outside\n";
    
    // Plot original data points and interpolation
    script << "plot '" << dataFile << "' using 1:2 with points pt 7 title 'Data Points', \\\n";
    script << "     '" << dataFile << ".result' using 1:2 with lines lw 2 title '" << plotTitle << "'\n";
    
    script.close();
    std::cout << "Gnuplot script created: " << scriptFile << std::endl;
    std::cout << "Run with: gnuplot -p " << scriptFile << std::endl;
}

// LagrangeInterpolation class
LagrangeInterpolation::LagrangeInterpolation() : Interpolation() {}

LagrangeInterpolation::~LagrangeInterpolation() {}

double LagrangeInterpolation::evaluate(double x) const {
    if (n == 0) return 0;
    
    double result = 0.0;
    
    for (int i = 0; i < n; i++) {
        double term = y_points[i];
        for (int j = 0; j < n; j++) {
            if (j != i) {
                term *= (x - x_points[j]) / (x_points[i] - x_points[j]);
            }
        }
        result += term;
    }
    
    return result;
}

// SplineInterpolation class
SplineInterpolation::SplineInterpolation() : Interpolation() {}

SplineInterpolation::~SplineInterpolation() {}

void SplineInterpolation::prepare() {
    if (n < 3) {
        std::cout << "Spline interpolation requires at least 3 points." << std::endl;
        return;
    }
    
    // Sort the data points by x-values
    std::vector<std::pair<double, double>> points;
    for (int i = 0; i < n; i++) {
        points.push_back(std::make_pair(x_points[i], y_points[i]));
    }
    std::sort(points.begin(), points.end());
    
    // Update sorted points
    for (int i = 0; i < n; i++) {
        x_points[i] = points[i].first;
        y_points[i] = points[i].second;
    }
    
    calculateCoefficients();
}

void SplineInterpolation::calculateCoefficients() {
    // Initialize arrays for coefficients
    a.resize(n);
    b.resize(n);
    c.resize(n);
    d.resize(n);
    
    // Set a to the y values
    for (int i = 0; i < n; i++) {
        a[i] = y_points[i];
    }
    
    // Compute h_i = x_{i+1} - x_i
    std::vector<double> h(n - 1);
    for (int i = 0; i < n - 1; i++) {
        h[i] = x_points[i + 1] - x_points[i];
    }
    
    // Set up the tridiagonal system
    std::vector<double> alpha(n - 1, 0);
    for (int i = 1; i < n - 1; i++) {
        alpha[i] = 3.0 * (a[i + 1] - a[i]) / h[i] - 3.0 * (a[i] - a[i - 1]) / h[i - 1];
    }
    
    // Solve the tridiagonal system for c
    std::vector<double> l(n, 0);
    std::vector<double> mu(n, 0);
    std::vector<double> z(n, 0);
    
    l[0] = 1.0;
    for (int i = 1; i < n - 1; i++) {
        l[i] = 2.0 * (x_points[i + 1] - x_points[i - 1]) - h[i - 1] * mu[i - 1];
        mu[i] = h[i] / l[i];
        z[i] = (alpha[i] - h[i - 1] * z[i - 1]) / l[i];
    }
    
    l[n - 1] = 1.0;
    
    // Back substitution
    c[n - 1] = 0.0;
    for (int j = n - 2; j >= 0; j--) {
        c[j] = z[j] - mu[j] * c[j + 1];
        b[j] = (a[j + 1] - a[j]) / h[j] - h[j] * (c[j + 1] + 2.0 * c[j]) / 3.0;
        d[j] = (c[j + 1] - c[j]) / (3.0 * h[j]);
    }
}

double SplineInterpolation::evaluate(double x) const {
    if (n == 0) return 0;
    
    // Find the appropriate segment
    int i = 0;
    while (i < n - 1 && x > x_points[i + 1]) {
        i++;
    }
    
    if (i >= n - 1) {
        i = n - 2;
    }
    
    // Calculate the local variable
    double dx = x - x_points[i];
    
    // Evaluate the cubic polynomial
    return a[i] + b[i] * dx + c[i] * dx * dx + d[i] * dx * dx * dx;
}