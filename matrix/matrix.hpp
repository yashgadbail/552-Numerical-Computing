#ifndef MATRIX_HPP
#define MATRIX_HPP

#include<iostream>
#include<fstream>
#include<vector>

using namespace std;

class Matrix {
    private:
        int rows, cols;
        vector<vector<double>> data;

    public:
        Matrix();
        Matrix(int r,int c);
        Matrix(const Matrix& other);
        ~Matrix(){}

        int getRows();
        int getCols();

        void print();
        void writeToFile(string filename);
        static Matrix readFromFile(string filename);

        Matrix operator+(Matrix& other);
        Matrix operator-(Matrix& other);

        static Matrix identityMatrix(int size);
        bool isSymmetric();

        friend ostream& operator<<(ostream& os,Matrix& mat);
        friend istream& operator>>(istream& is,Matrix& mat);
};

#endif
