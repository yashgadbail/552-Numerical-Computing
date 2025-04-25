#ifndef CURVEFITTING_H
#define CURVEFITTING_H

#include <vector>
#include <string>
#include <fstream>

class CurveFitting {
protected:
    std::vector<double> x_points;
    std::vector<double> y_points;
    int n; // number of data points

public:
    CurveFitting();
    virtual ~CurveFitting();
    
    // Load data from console or file
    void loadDataFromConsole();
    void loadDataFromFile(const std::string& filename);
    
    // Save results to file
    void saveResultsToFile(const std::string& filename, const std::vector<double>& x_eval, 
                          const std::vector<double>& y_eval);
    
    // Display data points
    void displayData() const;
    
    // Fit the curve to the data
    virtual void fitCurve() = 0;
    
    // Evaluate at specific point or range
    virtual double evaluate(double x) const = 0;
    std::vector<double> evaluateRange(double start, double end, int points) const;
    
    // Generate script for gnuplot
    void generateGnuplotScript(const std::string& dataFile, const std::string& scriptFile,
                              const std::string& plotTitle) const;
};

class LeastSquareFit : public CurveFitting {
private:
    double a, b; // y = a + bx
    
public:
    LeastSquareFit();
    ~LeastSquareFit();
    
    void fitCurve() override;
    double evaluate(double x) const override;
    void displayEquation() const;
};

class SecondDegreeFit : public CurveFitting {
private:
    double a, b, c; // y = a + bx + cx^2
    
public:
    SecondDegreeFit();
    ~SecondDegreeFit();
    
    void fitCurve() override;
    double evaluate(double x) const override;
    void displayEquation() const;
};

class ExponentialFit : public CurveFitting {
private:
    double a, b; // y = a * e^(bx)
    
public:
    ExponentialFit();
    ~ExponentialFit();
    
    void fitCurve() override;
    double evaluate(double x) const override;
    void displayEquation() const;
};

class PowerFit : public CurveFitting {
private:
    double a, b; // y = a * x^b
    
public:
    PowerFit();
    ~PowerFit();
    
    void fitCurve() override;
    double evaluate(double x) const override;
    void displayEquation() const;
};

#endif // CURVEFITTING_H