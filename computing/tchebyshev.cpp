#include "tchebyshev.h"
#include <iostream>
#include <fstream>
#include <cmath>

TchebyshevPolynomial::TchebyshevPolynomial() : n(0), degree(0) {}

TchebyshevPolynomial::~TchebyshevPolynomial() {}

void TchebyshevPolynomial::loadDataFromConsole() {
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

void TchebyshevPolynomial::loadDataFromFile(const std::string& filename) {
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

void TchebyshevPolynomial::setDegree(int deg) {
    degree = deg;
    std::cout << "Set Tchebyshev polynomial degree to " << degree << std::endl;
}

double TchebyshevPolynomial::tchebyshevValue(int n, double x) const {
    if (n == 0) return 1.0;
    if (n == 1) return x;
    
    return 2.0 * x * tchebyshevValue(n - 1, x) - tchebyshevValue(n - 2, x);
}

void TchebyshevPolynomial::prepare() {
    if (n == 0) {
        std::cout << "No data points available." << std::endl;
        return;
    }
    
    if (degree <= 0) {
        std::cout << "Please set a positive degree for the polynomial." << std::endl;
        return;
    }
    
    // Find the domain [a, b] of the data
    double min_x = x_points[0];
    double max_x = x_points[0];
    for (int i = 1; i < n; i++) {
        if (x_points[i] < min_x) min_x = x_points[i];
        if (x_points[i] > max_x) max_x = x_points[i];
    }
    
    // Scale the degree based on available data points
    if (degree >= n) {
        std::cout << "Warning: Degree is greater than or equal to the number of data points." << std::endl;
        std::cout << "Setting degree to " << n - 1 << std::endl;
        degree = n - 1;
    }
    
    // Initialize coefficients
    coefficients.resize(degree + 1, 0.0);
    
    // For each coefficient c_j
    for (int j = 0; j <= degree; j++) {
        double sum = 0.0;
        
        // For each data point
        for (int i = 0; i < n; i++) {
            // Map x from [min_x, max_x] to [-1, 1]
            double scaled_x = 2.0 * (x_points[i] - min_x) / (max_x - min_x) - 1.0;
            sum += y_points[i] * tchebyshevValue(j, scaled_x);
        }
        
        // c_j = (2/n) * sum(f(x_i) * T_j(x_i))
        coefficients[j] = (2.0 / n) * sum;
    }
    
    // Adjust c_0
    coefficients[0] *= 0.5;
    
    std::cout << "Tchebyshev polynomial coefficients calculated." << std::endl;
}

double TchebyshevPolynomial::evaluate(double x) const {
    if (n == 0 || degree == 0) return 0;
    
    // Find the domain [a, b] of the data
    double min_x = x_points[0];
    double max_x = x_points[0];
    for (int i = 1; i < n; i++) {
        if (x_points[i] < min_x) min_x = x_points[i];
        if (x_points[i] > max_x) max_x = x_points[i];
    }
    
    // Map x from [min_x, max_x] to [-1, 1]
    double scaled_x = 2.0 * (x - min_x) / (max_x - min_x) - 1.0;
    
    // Check bounds
    if (scaled_x < -1.0 || scaled_x > 1.0) {
        std::cout << "Warning: Evaluating outside the original data range." << std::endl;
    }
    
    double result = 0.0;
    for (int j = 0; j <= degree; j++) {
        result += coefficients[j] * tchebyshevValue(j, scaled_x);
    }
    
    return result;
}

std::vector<double> TchebyshevPolynomial::evaluateRange(double start, double end, int points) const {
    std::vector<double> result(points);
    double step = (end - start) / (points - 1);
    
    for (int i = 0; i < points; i++) {
        double x = start + i * step;
        result[i] = evaluate(x);
    }
    
    return result;
}

void TchebyshevPolynomial::displayData() const {
    std::cout << "Data points:" << std::endl;
    for (int i = 0; i < n; i++) {
        std::cout << "(" << x_points[i] << ", " << y_points[i] << ")" << std::endl;
    }
}

void TchebyshevPolynomial::saveResultsToFile(const std::string& filename, const std::vector<double>& x_eval, 
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

void TchebyshevPolynomial::generateGnuplotScript(const std::string& dataFile, const std::string& scriptFile,
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
    
    // Plot original data points and polynomial
    script << "plot '" << dataFile << "' using 1:2 with points pt 7 title 'Data Points', \\\n";
    script << "     '" << dataFile << ".result' using 1:2 with lines lw 2 title '" << plotTitle << "'\n";
    
    script.close();
    std::cout << "Gnuplot script created: " << scriptFile << std::endl;
    std::cout << "Run with: gnuplot -p " << scriptFile << std::endl;
}