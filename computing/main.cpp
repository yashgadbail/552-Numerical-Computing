#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include "interpolation.h"
#include "curvefitting.h"
#include "tchebyshev.h"
#include "comparison.h"

using namespace std;

// Function prototypes
void lagrangeInterpolationMenu();
void splineInterpolationMenu();
void curveRegressionMenu();
void tchebyshevPolynomialMenu();
void comparisonMenu();

// Utility functions
void clearInputBuffer();
double getDouble(const string& prompt);
int getInteger(const string& prompt);
string getString(const string& prompt);
bool getFileOrConsoleChoice();

int main() {
    int choice = 0;
    
    do {
        cout << "\n======== Mathematical Computing Menu ========" << endl;
        cout << "1. Lagrange Polynomial Interpolation" << endl;
        cout << "2. Spline Interpolation" << endl;
        cout << "3. Curve Fitting Methods" << endl;
        cout << "4. Tchebyshev Polynomial" << endl;
        cout << "5. Compare Interpolation Methods" << endl;
        cout << "0. Exit" << endl;
        cout << "===========================================" << endl;
        
        choice = getInteger("Enter your choice: ");
        
        switch (choice) {
            case 1:
                lagrangeInterpolationMenu();
                break;
            case 2:
                splineInterpolationMenu();
                break;
            case 3:
                curveRegressionMenu();
                break;
            case 4:
                tchebyshevPolynomialMenu();
                break;
            case 5:
                comparisonMenu();
                break;
            case 0:
                cout << "Exiting program. Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 0);
    
    return 0;
}

void lagrangeInterpolationMenu() {
    LagrangeInterpolation lagrange;
    
    cout << "\n------ Lagrange Polynomial Interpolation ------" << endl;
    
    // Get data input
    bool useFile = getFileOrConsoleChoice();
    if (useFile) {
        string filename = getString("Enter filename: ");
        lagrange.loadDataFromFile(filename);
    } else {
        lagrange.loadDataFromConsole();
    }
    
    // Display data
    lagrange.displayData();
    
    // Evaluate at points
    char evalChoice;
    cout << "Evaluate at (s)ingle point or (r)ange? [s/r]: ";
    cin >> evalChoice;
    clearInputBuffer();
    
    if (tolower(evalChoice) == 's') {
        double x = getDouble("Enter x value to evaluate: ");
        double y = lagrange.evaluate(x);
        cout << "f(" << x << ") = " << y << endl;
    } else {
        double start = getDouble("Enter start of range: ");
        double end = getDouble("Enter end of range: ");
        int points = getInteger("Enter number of points: ");
        
        vector<double> x_values;
        double step = (end - start) / (points - 1);
        for (int i = 0; i < points; i++) {
            x_values.push_back(start + i * step);
        }
        
        vector<double> y_values = lagrange.evaluateRange(start, end, points);
        
        // Save results
        string filename = getString("Enter filename to save results: ");
        lagrange.saveResultsToFile(filename, x_values, y_values);
        
        // Create gnuplot script
        string scriptFile = filename + ".gp";
        lagrange.generateGnuplotScript(filename, scriptFile, "Lagrange Polynomial Interpolation");
    }
}

void splineInterpolationMenu() {
    SplineInterpolation spline;
    
    cout << "\n------ Cubic Spline Interpolation ------" << endl;
    
    // Get data input
    bool useFile = getFileOrConsoleChoice();
    if (useFile) {
        string filename = getString("Enter filename: ");
        spline.loadDataFromFile(filename);
    } else {
        spline.loadDataFromConsole();
    }
    
    // Display data
    spline.displayData();
    
    // Prepare spline
    spline.prepare();
    
    // Evaluate at points
    char evalChoice;
    cout << "Evaluate at (s)ingle point or (r)ange? [s/r]: ";
    cin >> evalChoice;
    clearInputBuffer();
    
    if (tolower(evalChoice) == 's') {
        double x = getDouble("Enter x value to evaluate: ");
        double y = spline.evaluate(x);
        cout << "f(" << x << ") = " << y << endl;
    } else {
        double start = getDouble("Enter start of range: ");
        double end = getDouble("Enter end of range: ");
        int points = getInteger("Enter number of points: ");
        
        vector<double> x_values;
        double step = (end - start) / (points - 1);
        for (int i = 0; i < points; i++) {
            x_values.push_back(start + i * step);
        }
        
        vector<double> y_values = spline.evaluateRange(start, end, points);
        
        // Save results
        string filename = getString("Enter filename to save results: ");
        spline.saveResultsToFile(filename, x_values, y_values);
        
        // Create gnuplot script
        string scriptFile = filename + ".gp";
        spline.generateGnuplotScript(filename, scriptFile, "Cubic Spline Interpolation");
    }
}

void curveRegressionMenu() {
    int choice = 0;
    
    do {
        cout << "\n------ Curve Fitting Methods ------" << endl;
        cout << "1. Linear Least Squares Fit" << endl;
        cout << "2. Second Degree Polynomial Fit" << endl;
        cout << "3. Exponential Fit" << endl;
        cout << "4. Power Fit" << endl;
        cout << "0. Back to Main Menu" << endl;
        cout << "--------------------------------" << endl;
        
        choice = getInteger("Enter your choice: ");
        
        if (choice >= 1 && choice <= 4) {
            CurveFitting* fit = nullptr;
            string title;
            
            switch (choice) {
                case 1:
                    fit = new LeastSquareFit();
                    title = "Linear Least Squares Fit";
                    break;
                case 2:
                    fit = new SecondDegreeFit();
                    title = "Second Degree Polynomial Fit";
                    break;
                case 3:
                    fit = new ExponentialFit();
                    title = "Exponential Fit";
                    break;
                case 4:
                    fit = new PowerFit();
                    title = "Power Fit";
                    break;
            }
            
            // Get data input
            bool useFile = getFileOrConsoleChoice();
            if (useFile) {
                string filename = getString("Enter filename: ");
                fit->loadDataFromFile(filename);
            } else {
                fit->loadDataFromConsole();
            }
            
            // Display data
            fit->displayData();
            
            // Fit curve
            fit->fitCurve();
            
            // Evaluate range
            double start = getDouble("Enter start of range for evaluation: ");
            double end = getDouble("Enter end of range for evaluation: ");
            int points = getInteger("Enter number of points: ");
            
            vector<double> x_values;
            double step = (end - start) / (points - 1);
            for (int i = 0; i < points; i++) {
                x_values.push_back(start + i * step);
            }
            
            vector<double> y_values = fit->evaluateRange(start, end, points);
            
            // Save results
            string filename = getString("Enter filename to save results: ");
            fit->saveResultsToFile(filename, x_values, y_values);
            
            // Create gnuplot script
            string scriptFile = filename + ".gp";
            fit->generateGnuplotScript(filename, scriptFile, title);
            
            delete fit;
        } else if (choice != 0) {
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 0);
}

void tchebyshevPolynomialMenu() {
    TchebyshevPolynomial tchebyshev;
    
    cout << "\n------ Tchebyshev Polynomial ------" << endl;
    
    // Get data input
    bool useFile = getFileOrConsoleChoice();
    if (useFile) {
        string filename = getString("Enter filename: ");
        tchebyshev.loadDataFromFile(filename);
    } else {
        tchebyshev.loadDataFromConsole();
    }
    
    // Display data
    tchebyshev.displayData();
    
    // Set degree
    int degree = getInteger("Enter degree of Tchebyshev polynomial: ");
    tchebyshev.setDegree(degree);
    
    // Prepare polynomial
    tchebyshev.prepare();
    
    // Evaluate at points
    char evalChoice;
    cout << "Evaluate at (s)ingle point or (r)ange? [s/r]: ";
    cin >> evalChoice;
    clearInputBuffer();
    
    if (tolower(evalChoice) == 's') {
        double x = getDouble("Enter x value to evaluate: ");
        double y = tchebyshev.evaluate(x);
        cout << "f(" << x << ") = " << y << endl;
    } else {
        double start = getDouble("Enter start of range: ");
        double end = getDouble("Enter end of range: ");
        int points = getInteger("Enter number of points: ");
        
        vector<double> x_values;
        double step = (end - start) / (points - 1);
        for (int i = 0; i < points; i++) {
            x_values.push_back(start + i * step);
        }
        
        vector<double> y_values = tchebyshev.evaluateRange(start, end, points);
        
        // Save results
        string filename = getString("Enter filename to save results: ");
        tchebyshev.saveResultsToFile(filename, x_values, y_values);
        
        // Create gnuplot script
        string scriptFile = filename + ".gp";
        tchebyshev.generateGnuplotScript(filename, scriptFile, "Tchebyshev Polynomial");
    }
}

void comparisonMenu() {
    Comparison comparison;
    
    cout << "\n------ Compare Interpolation Methods ------" << endl;
    
    // Get data input
    bool useFile = getFileOrConsoleChoice();
    if (useFile) {
        string filename = getString("Enter filename: ");
        comparison.loadDataFromFile(filename);
    } else {
        comparison.loadDataFromConsole();
    }
    
    // Prepare all methods
    comparison.prepareAll();
    
    // Evaluate at range
    double start = getDouble("Enter start of range for comparison: ");
    double end = getDouble("Enter end of range for comparison: ");
    int points = getInteger("Enter number of points: ");
    
    // Generate comparison data
    comparison.compareAndGenerate(start, end, points);
    
    // Generate comparison plot
    comparison.generateComparisonPlot();
}

// Utility functions
void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

double getDouble(const string& prompt) {
    double value;
    cout << prompt;
    while (!(cin >> value)) {
        cout << "Invalid input. Please enter a number: ";
        clearInputBuffer();
    }
    clearInputBuffer();
    return value;
}

int getInteger(const string& prompt) {
    int value;
    cout << prompt;
    while (!(cin >> value)) {
        cout << "Invalid input. Please enter an integer: ";
        clearInputBuffer();
    }
    clearInputBuffer();
    return value;
}

string getString(const string& prompt) {
    string value;
    cout << prompt;
    getline(cin, value);
    return value;
}

bool getFileOrConsoleChoice() {
    char choice;
    cout << "Load data from (f)ile or (c)onsole? [f/c]: ";
    cin >> choice;
    clearInputBuffer();
    return (tolower(choice) == 'f');
}