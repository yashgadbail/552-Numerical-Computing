#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

class Matrix {
private:
    int rows, cols;
    vector<vector<double>> data;

public:
    // Constructors & Destructor
    Matrix();
    Matrix(int r, int c);
    Matrix(const Matrix& other);
    ~Matrix();

    // Getters
    int getRows();
    int getCols();

    // Basic Operations
    void print();
    void writeToFile(string filename);

    // Operators
    Matrix operator+(Matrix& other);
    Matrix operator-(Matrix& other);
    friend ostream& operator<<(ostream& os, Matrix& mat);
    friend istream& operator>>(istream& is, Matrix& mat);

    // Identity Matrix Generator
    static Matrix identityMatrix(int size);

    // Check if matrix is symmetric
    bool isSymmetric();

    // File handling
    static Matrix readFromFile(string filename);
};

#endif // MATRIX_HPP
