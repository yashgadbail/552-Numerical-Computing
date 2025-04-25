#include "curvefitting.h"
#include <iostream>
#include <fstream>
#include <cmath>

// Base CurveFitting class
CurveFitting::CurveFitting() : n(0) {}

CurveFitting::~CurveFitting() {}

void CurveFitting::loadDataFromConsole() {
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

void CurveFitting::loadDataFromFile(const std::string& filename) {
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

void CurveFitting::saveResultsToFile(const std::string& filename, const std::vector<double>& x_eval, 
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

void CurveFitting::displayData() const {
    std::cout << "Data points:" << std::endl;
    for (int i = 0; i < n; i++) {
        std::cout << "(" << x_points[i] << ", " << y_points[i] << ")" << std::endl;
    }
}

std::vector<double> CurveFitting::evaluateRange(double start, double end, int points) const {
    std::vector<double> result(points);
    double step = (end - start) / (points - 1);
    
    for (int i = 0; i < points; i++) {
        double x = start + i * step;
        result[i] = evaluate(x);
    }
    
    return result;
}

void CurveFitting::generateGnuplotScript(const std::string& dataFile, const std::string& scriptFile,
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
    
    // Plot original data points and fit curve
    script << "plot '" << dataFile << "' using 1:2 with points pt 7 title 'Data Points', \\\n";
    script << "     '" << dataFile << ".result' using 1:2 with lines lw 2 title '" << plotTitle << "'\n";
    
    script.close();
    std::cout << "Gnuplot script created: " << scriptFile << std::endl;
    std::cout << "Run with: gnuplot -p " << scriptFile << std::endl;
}

// LeastSquareFit class
LeastSquareFit::LeastSquareFit() : CurveFitting(), a(0), b(0) {}

LeastSquareFit::~LeastSquareFit() {}

void LeastSquareFit::fitCurve() {
    if (n < 2) {
        std::cout << "At least 2 data points are needed for linear fit." << std::endl;
        return;
    }
    
    double sum_x = 0.0;
    double sum_y = 0.0;
    double sum_xy = 0.0;
    double sum_x2 = 0.0;
    
    for (int i = 0; i < n; i++) {
        sum_x += x_points[i];
        sum_y += y_points[i];
        sum_xy += x_points[i] * y_points[i];
        sum_x2 += x_points[i] * x_points[i];
    }
    
    double denom = n * sum_x2 - sum_x * sum_x;
    
    if (std::abs(denom) < 1e-10) {
        std::cout << "Cannot fit a line (denominator close to zero)." << std::endl;
        a = 0;
        b = 0;
        return;
    }
    
    a = (sum_y * sum_x2 - sum_x * sum_xy) / denom;
    b = (n * sum_xy - sum_x * sum_y) / denom;
    
    displayEquation();
}

double LeastSquareFit::evaluate(double x) const {
    return a + b * x;
}

void LeastSquareFit::displayEquation() const {
    std::cout << "Fitted equation: y = " << a << " + " << b << "x" << std::endl;
}

// SecondDegreeFit class
SecondDegreeFit::SecondDegreeFit() : CurveFitting(), a(0), b(0), c(0) {}

SecondDegreeFit::~SecondDegreeFit() {}

void SecondDegreeFit::fitCurve() {
    if (n < 3) {
        std::cout << "At least 3 data points are needed for quadratic fit." << std::endl;
        return;
    }
    
    double sum_x = 0.0, sum_x2 = 0.0, sum_x3 = 0.0, sum_x4 = 0.0;
    double sum_y = 0.0, sum_xy = 0.0, sum_x2y = 0.0;
    
    for (int i = 0; i < n; i++) {
        double x = x_points[i];
        double y = y_points[i];
        
        sum_x += x;
        sum_x2 += x * x;
        sum_x3 += x * x * x;
        sum_x4 += x * x * x * x;
        
        sum_y += y;
        sum_xy += x * y;
        sum_x2y += x * x * y;
    }
    
    // Set up the system of equations
    double A[3][4] = {
        {static_cast<double>(n), sum_x, sum_x2, sum_y},
        {sum_x, sum_x2, sum_x3, sum_xy},
        {sum_x2, sum_x3, sum_x4, sum_x2y}
    };
    
    // Gaussian elimination
    for (int i = 0; i < 3; i++) {
        // Find pivot
        int max_row = i;
        double max_val = std::abs(A[i][i]);
        
        for (int j = i + 1; j < 3; j++) {
            if (std::abs(A[j][i]) > max_val) {
                max_val = std::abs(A[j][i]);
                max_row = j;
            }
        }
        
        // Swap rows if needed
        if (max_row != i) {
            for (int j = 0; j < 4; j++) {
                std::swap(A[i][j], A[max_row][j]);
            }
        }
        
        // Eliminate below
        for (int j = i + 1; j < 3; j++) {
            double factor = A[j][i] / A[i][i];
            for (int k = i; k < 4; k++) {
                A[j][k] -= factor * A[i][k];
            }
        }
    }
    
    // Back substitution
    c = A[2][3] / A[2][2];
    b = (A[1][3] - A[1][2] * c) / A[1][1];
    a = (A[0][3] - A[0][1] * b - A[0][2] * c) / A[0][0];
    
    displayEquation();
}

double SecondDegreeFit::evaluate(double x) const {
    return a + b * x + c * x * x;
}

void SecondDegreeFit::displayEquation() const {
    std::cout << "Fitted equation: y = " << a << " + " << b << "x + " << c << "x^2" << std::endl;
}

// ExponentialFit class
ExponentialFit::ExponentialFit() : CurveFitting(), a(0), b(0) {}

ExponentialFit::~ExponentialFit() {}

void ExponentialFit::fitCurve() {
    if (n < 2) {
        std::cout << "At least 2 data points are needed for exponential fit." << std::endl;
        return;
    }
    
    // Check for non-positive y values
    for (int i = 0; i < n; i++) {
        if (y_points[i] <= 0) {
            std::cout << "Warning: Exponential fit requires positive y values. Point (" 
                      << x_points[i] << ", " << y_points[i] << ") will be ignored." << std::endl;
        }
    }
    
    double sum_x = 0.0;
    double sum_lny = 0.0;
    double sum_x2 = 0.0;
    double sum_xlny = 0.0;
    int valid_points = 0;
    
    for (int i = 0; i < n; i++) {
        if (y_points[i] > 0) {
            double lny = std::log(y_points[i]);
            sum_x += x_points[i];
            sum_lny += lny;
            sum_x2 += x_points[i] * x_points[i];
            sum_xlny += x_points[i] * lny;
            valid_points++;
        }
    }
    
    if (valid_points < 2) {
        std::cout << "Not enough valid data points for exponential fit." << std::endl;
        a = 1.0;
        b = 0.0;
        return;
    }
    
    double denom = valid_points * sum_x2 - sum_x * sum_x;
    
    if (std::abs(denom) < 1e-10) {
        std::cout << "Cannot fit an exponential curve (denominator close to zero)." << std::endl;
        a = 1.0;
        b = 0.0;
        return;
    }
    
    // For y = a * e^(bx), we compute ln(y) = ln(a) + bx
    double ln_a = (sum_lny * sum_x2 - sum_x * sum_xlny) / denom;
    b = (valid_points * sum_xlny - sum_x * sum_lny) / denom;
    a = std::exp(ln_a);
    
    displayEquation();
}

double ExponentialFit::evaluate(double x) const {
    return a * std::exp(b * x);
}

void ExponentialFit::displayEquation() const {
    std::cout << "Fitted equation: y = " << a << " * e^(" << b << "x)" << std::endl;
}

// PowerFit class
PowerFit::PowerFit() : CurveFitting(), a(0), b(0) {}

PowerFit::~PowerFit() {}

void PowerFit::fitCurve() {
    if (n < 2) {
        std::cout << "At least 2 data points are needed for power fit." << std::endl;
        return;
    }
    
    // Check for non-positive values
    for (int i = 0; i < n; i++) {
        if (x_points[i] <= 0 || y_points[i] <= 0) {
            std::cout << "Warning: Power fit requires positive x and y values. Point (" 
                      << x_points[i] << ", " << y_points[i] << ") will be ignored." << std::endl;
        }
    }
    
    double sum_lnx = 0.0;
    double sum_lny = 0.0;
    double sum_lnx2 = 0.0;
    double sum_lnxlny = 0.0;
    int valid_points = 0;
    
    for (int i = 0; i < n; i++) {
        if (x_points[i] > 0 && y_points[i] > 0) {
            double lnx = std::log(x_points[i]);
            double lny = std::log(y_points[i]);
            sum_lnx += lnx;
            sum_lny += lny;
            sum_lnx2 += lnx * lnx;
            sum_lnxlny += lnx * lny;
            valid_points++;
        }
    }
    
    if (valid_points < 2) {
        std::cout << "Not enough valid data points for power fit." << std::endl;
        a = 1.0;
        b = 0.0;
        return;
    }
    
    double denom = valid_points * sum_lnx2 - sum_lnx * sum_lnx;
    
    if (std::abs(denom) < 1e-10) {
        std::cout << "Cannot fit a power curve (denominator close to zero)." << std::endl;
        a = 1.0;
        b = 0.0;
        return;
    }
    
    // For y = a * x^b, we compute ln(y) = ln(a) + b*ln(x)
    double ln_a = (sum_lny * sum_lnx2 - sum_lnx * sum_lnxlny) / denom;
    b = (valid_points * sum_lnxlny - sum_lnx * sum_lny) / denom;
    a = std::exp(ln_a);
    
    displayEquation();
}

double PowerFit::evaluate(double x) const {
    if (x <= 0) {
        std::cout << "Warning: Power function not defined for x <= 0" << std::endl;
        return 0;
    }
    return a * std::pow(x, b);
}

void PowerFit::displayEquation() const {
    std::cout << "Fitted equation: y = " << a << " * x^" << b << std::endl;
}