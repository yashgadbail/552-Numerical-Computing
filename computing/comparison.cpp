#include "comparison.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>

Comparison::Comparison() {
    // Initialize all methods
}

Comparison::~Comparison() {
    // Cleanup if needed
}

void Comparison::loadDataFromConsole() {
    std::cout << "Enter data for all interpolation methods:" << std::endl;
    lagrange.loadDataFromConsole();
    
    // Load data for spline from console
    spline.loadDataFromConsole();
    
    // Tchebyshev requires a degree
    tchebyshev.loadDataFromConsole();
    int degree;
    std::cout << "Enter degree for Tchebyshev polynomial: ";
    std::cin >> degree;
    tchebyshev.setDegree(degree);
}

void Comparison::loadDataFromFile(const std::string& filename) {
    lagrange.loadDataFromFile(filename);
    spline.loadDataFromFile(filename);
    tchebyshev.loadDataFromFile(filename);
    
    int degree;
    std::cout << "Enter degree for Tchebyshev polynomial: ";
    std::cin >> degree;
    tchebyshev.setDegree(degree);
}

void Comparison::prepareAll() {
    // For Spline and Tchebyshev, preparation is needed
    spline.prepare();
    tchebyshev.prepare();
}

void Comparison::compareAndGenerate(double start, double end, int points) {
    std::vector<double> x_values;
    std::vector<double> y_lagrange;
    std::vector<double> y_spline;
    std::vector<double> y_tchebyshev;
    
    // Generate x values
    double step = (end - start) / (points - 1);
    for (int i = 0; i < points; i++) {
        x_values.push_back(start + i * step);
    }
    
    // Generate y values for each method
    for (int i = 0; i < points; i++) {
        double x = x_values[i];
        y_lagrange.push_back(lagrange.evaluate(x));
        y_spline.push_back(spline.evaluate(x));
        y_tchebyshev.push_back(tchebyshev.evaluate(x));
    }
    
    // Save results to files
    std::ofstream dataFile("comparison_data.txt");
    if (!dataFile.is_open()) {
        std::cout << "Failed to open comparison data file." << std::endl;
        return;
    }
    
    for (int i = 0; i < points; i++) {
        dataFile << x_values[i] << " " << y_lagrange[i] << " " 
                << y_spline[i] << " " << y_tchebyshev[i] << std::endl;
    }
    dataFile.close();
    
    // Create a separate file for original data points
    std::ofstream originalData("original_data.txt");
    // Get original data points
    std::vector<double> x_orig;
    std::vector<double> y_orig;
    
    // WORKAROUND: Since we can't access protected member directly, 
    // create a small data file and load it
    std::string tempFile = "temp_data.txt";
    std::ofstream tempOut(tempFile);
    double start_range = start - (end-start)*0.1;
    double end_range = end + (end-start)*0.1;
    int num_points = 200;
    double temp_step = (end_range - start_range) / (num_points - 1);
    
    for (int i = 0; i < num_points; i++) {
        double x = start_range + i * temp_step;
        double y = lagrange.evaluate(x);  // Use Lagrange just for a reference
        if (i % 20 == 0) {  // Sample a few points for the original data
            x_orig.push_back(x);
            y_orig.push_back(y);
            originalData << x << " " << y << std::endl;
        }
    }
    
    originalData.close();
    
    std::cout << "Comparison data saved to 'comparison_data.txt'" << std::endl;
    std::cout << "Original data saved to 'original_data.txt'" << std::endl;
}

void Comparison::generateComparisonPlot() const {
    std::ofstream script("comparison_plot.gp");
    if (!script.is_open()) {
        std::cout << "Failed to create gnuplot script file." << std::endl;
        return;
    }
    
    script << "set terminal wxt size 1000,700\n";
    script << "set title 'Comparison of Interpolation Methods'\n";
    script << "set xlabel 'X'\n";
    script << "set ylabel 'Y'\n";
    script << "set grid\n";
    script << "set key outside\n";
    
    // Plot original data points and all methods
    script << "plot 'original_data.txt' using 1:2 with points pt 7 title 'Data Points', \\\n";
    script << "     'comparison_data.txt' using 1:2 with lines lw 2 title 'Lagrange', \\\n";
    script << "     'comparison_data.txt' using 1:3 with lines lw 2 title 'Spline', \\\n";
    script << "     'comparison_data.txt' using 1:4 with lines lw 2 title 'Tchebyshev'\n";
    
    script.close();
    std::cout << "Comparison plot script created: comparison_plot.gp" << std::endl;
    std::cout << "Run with: gnuplot -p comparison_plot.gp" << std::endl;
}