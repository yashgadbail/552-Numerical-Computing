#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include <vector>
#include <string>
#include <fstream>

class Interpolation {
protected:
    std::vector<double> x_points;
    std::vector<double> y_points;
    int n; // number of data points

public:
    Interpolation();
    virtual ~Interpolation();

    const std::vector<double>& getXPoints() const { return x_points; }
    const std::vector<double>& getYPoints() const { return y_points; }
    
    // Load data from console or file
    void loadDataFromConsole();
    void loadDataFromFile(const std::string& filename);
    
    // Save results to file
    void saveResultsToFile(const std::string& filename, const std::vector<double>& x_eval, 
                          const std::vector<double>& y_eval);
    
    // Display data points
    void displayData() const;
    
    // Evaluate at specific point or range
    virtual double evaluate(double x) const = 0;
    std::vector<double> evaluateRange(double start, double end, int points) const;
    
    // Generate script for gnuplot
    void generateGnuplotScript(const std::string& dataFile, const std::string& scriptFile,
                              const std::string& plotTitle) const;
};

class LagrangeInterpolation : public Interpolation {
public:
    LagrangeInterpolation();
    ~LagrangeInterpolation();
    
    double evaluate(double x) const override;
};

class SplineInterpolation : public Interpolation {
private:
    std::vector<double> a, b, c, d; // Spline coefficients
    void calculateCoefficients();
    
public:
    SplineInterpolation();
    ~SplineInterpolation();
    
    void prepare(); // Prepare the spline coefficients
    double evaluate(double x) const override;
};

#endif // INTERPOLATION_H