#ifndef TCHEBYSHEV_H
#define TCHEBYSHEV_H

#include <vector>
#include <string>
#include <fstream>

class TchebyshevPolynomial {
private:
    std::vector<double> x_points;
    std::vector<double> y_points;
    int n; // number of data points
    int degree; // degree of polynomial
    std::vector<double> coefficients;
    
    // Calculate Tchebyshev polynomial values
    double tchebyshevValue(int n, double x) const;
    
public:
    TchebyshevPolynomial();
    ~TchebyshevPolynomial();
    
    // Load data from console or file
    void loadDataFromConsole();
    void loadDataFromFile(const std::string& filename);
    
    // Set the degree of polynomial
    void setDegree(int deg);
    
    // Prepare the polynomial
    void prepare();
    
    // Evaluate at specific point or range
    double evaluate(double x) const;
    std::vector<double> evaluateRange(double start, double end, int points) const;
    
    // Display data points
    void displayData() const;
    
    // Save results to file
    void saveResultsToFile(const std::string& filename, const std::vector<double>& x_eval, 
                          const std::vector<double>& y_eval);
    
    // Generate script for gnuplot
    void generateGnuplotScript(const std::string& dataFile, const std::string& scriptFile,
                              const std::string& plotTitle) const;
};

#endif // TCHEBYSHEV_H