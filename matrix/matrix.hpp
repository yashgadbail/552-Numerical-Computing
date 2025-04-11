#ifndef MATRIX_HPP
#define MATRIX_HPP
#include <iostream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

class Matrix {
private:
    int rows, cols;
    vector<vector<double>> data;

public:
    // Constructors
    Matrix();
    Matrix(int r, int c);
    Matrix(const Matrix& other);
    ~Matrix() {}

    // Getters
    int getRows() const;
    int getCols() const;
    double get(int i, int j) const;
    void set(int i, int j, double val);

    // Operations
    Matrix operator+(Matrix& other);
    Matrix operator-(Matrix& other);
    Matrix operator*(Matrix& other);
    Matrix transpose();
    Matrix identityMatrix(int size);
    bool isSymmetric();
    double determinant(); 
    
    void print();
    void writeToFile(string filename);
    static Matrix readFromFile(string filename);
    
    // Linear system solvers
    vector<double> gaussianElimination(vector<double>& b);
    
    // LU Decomposition
    pair<Matrix, Matrix> luDecompositionDoolittle();
    pair<Matrix, Matrix> luDecompositionCrout();
    Matrix choleskyDecomposition();
    
    // Solve using LU
    vector<double> solveLU(vector<double>& b, Matrix& L, Matrix& U);
    
    // Gauss ELimination
	vector<double> gaussJacobi(vector<double>& b, int maxIterations = 100, double tolerance = 1e-6);
	vector<double> gaussSeidel(vector<double>& b, int maxIterations = 100, double tolerance = 1e-6);
	bool isDiagonallyDominant();
	bool makeDiagonallyDominant();


    // Friend operators for I/O - changed to const references
    friend ostream& operator<<(ostream& os, const Matrix& mat);
    friend istream& operator>>(istream& is, Matrix& mat);
};
#endif
