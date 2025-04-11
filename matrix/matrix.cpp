// matrix.cpp

#include "matrix.hpp"
#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

Matrix::Matrix() {
    rows = 0;
    cols = 0;
}

Matrix::Matrix(int r, int c) {
    rows = r;
    cols = c;
    data = vector<vector<double>>(r, vector<double>(c, 0));
}

Matrix::Matrix(const Matrix& other) {
    rows = other.rows;
    cols = other.cols;
    data = other.data;
}

int Matrix::getRows() const {
    return rows;
}

int Matrix::getCols() const {
    return cols;
}

double Matrix::get(int i, int j) const {
    if (i >= 0 && i < rows && j >= 0 && j < cols) {
        return data[i][j];
    }
    cout << "Index out of bounds!" << endl;
    return 0;
}

void Matrix::set(int i, int j, double val) {
    if (i >= 0 && i < rows && j >= 0 && j < cols) {
        data[i][j] = val;
    } else {
        cout << "Index out of bounds!" << endl;
    }
}

void Matrix::print() {
    cout << *this;
}

void Matrix::writeToFile(string filename) {
    ofstream outFile(filename + ".txt");
    if (!outFile) {
        cout << "Error opening file!" << endl;
        return;
    }
    outFile << rows << " " << cols << endl;
    outFile << *this;
}

Matrix Matrix::readFromFile(string filename) {
    ifstream inFile(filename + ".txt");
    if (!inFile) {
        cout << "Error opening file!" << endl;
        return Matrix();
    }
    int r, c;
    inFile >> r >> c;
    Matrix mat(r, c);
    for(int i = 0; i < r; i++) {
        for(int j = 0; j < c; j++) {
            inFile >> mat.data[i][j];
        }
    }
    return mat;
}

Matrix Matrix::operator+(Matrix& other) {
    if (rows != other.rows || cols != other.cols) {
        cout << "Error: Matrices must be of the same dimensions!" << endl;
        return Matrix();
    }
    Matrix result(rows, cols);
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            result.data[i][j] = data[i][j] + other.data[i][j];
        }
    }
    return result;
}

Matrix Matrix::operator-(Matrix& other) {
    if (rows != other.rows || cols != other.cols) {
        cout << "Error: Matrices must be of the same dimensions!" << endl;
        return Matrix();
    }
    Matrix result(rows, cols);
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            result.data[i][j] = data[i][j] - other.data[i][j];
        }
    }
    return result;
}

Matrix Matrix::operator*(Matrix& other) {
    if (cols != other.rows) {
        cout << "Matrix dimensions do not match for multiplication!" << endl;
        return Matrix();
    }
    Matrix result(rows, other.cols);
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < other.cols; j++) {
            for(int k = 0; k < cols; k++) {
                result.data[i][j] += data[i][k] * other.data[k][j];
            }
        }
    }
    return result;
}

Matrix Matrix::transpose() {
    Matrix result(cols, rows);
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            result.data[j][i] = data[i][j];
        }
    }
    return result;
}

Matrix Matrix::identityMatrix(int size) {
    Matrix identity(size, size);
    for(int i = 0; i < size; i++) {
        identity.data[i][i] = 1;
    }
    return identity;
}

bool Matrix::isSymmetric() {
    if (rows != cols) {
        return false;
    }
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < i; j++) {
            if (fabs(data[i][j] - data[j][i]) > 1e-10) {
                return false;
            }
        }
    }
    return true;
}

ostream& operator<<(ostream& os, const Matrix& mat) {
    for(int i = 0; i < mat.rows; i++) {
        for(int j = 0; j < mat.cols; j++) {
            os << mat.data[i][j] << " ";
        }
        os << endl;
    }
    return os;
}

istream& operator>>(istream& is, Matrix& mat) {
    for(int i = 0; i < mat.rows; i++) {
        for(int j = 0; j < mat.cols; j++) {
            is >> mat.data[i][j];
        }
    }
    return is;
}

vector<double> Matrix::gaussianElimination(vector<double>& b) {
    if (rows != cols) {
        cout << "Matrix must be square for Gaussian elimination!" << endl;
        return vector<double>();
    }
    
    if (rows != b.size()) {
        cout << "Vector b must have the same size as matrix rows!" << endl;
        return vector<double>();
    }
    
    int n = rows;
    
    // Create augmented matrix
    vector<vector<double>> augmented = data;
    for(int i = 0; i < n; i++) {
        augmented[i].push_back(b[i]);
    }

    // Forward elimination with partial pivoting
    for(int i = 0; i < n; i++) {
        // Find pivot (largest absolute value in current column)
        int maxRow = i;
        for(int k = i + 1; k < n; k++) {
            if (fabs(augmented[k][i]) > fabs(augmented[maxRow][i])) {
                maxRow = k;
            }
        }

        // Swap rows
        if (maxRow != i) {
            swap(augmented[i], augmented[maxRow]);
        }

        // Check for singular matrix
        if (fabs(augmented[i][i]) < 1e-10) {
            cout << "Matrix is singular or nearly singular!" << endl;
            return vector<double>();
        }

        // Eliminate below
        for(int k = i + 1; k < n; k++) {
            double factor = augmented[k][i] / augmented[i][i];
            for(int j = i; j <= n; j++) {
                augmented[k][j] -= factor * augmented[i][j];
            }
        }
    }

    // Back substitution
    vector<double> x(n);
    for(int i = n - 1; i >= 0; i--) {
        x[i] = augmented[i][n];
        for(int j = i + 1; j < n; j++) {
            x[i] -= augmented[i][j] * x[j];
        }
        x[i] /= augmented[i][i];
    }

    return x;
}

// DooLittle LU Decomposition
pair<Matrix, Matrix> Matrix::luDecompositionDoolittle() 
{
    if (rows != cols) {
        cout << "Matrix must be square for LU decomposition!" << endl;
        return {Matrix(), Matrix()};
    }
    
    int n = rows;
    Matrix L(n, n);
    Matrix U(n, n);
    
    for(int i = 0; i < n; i++) {
        L.data[i][i] = 1.0;
    }
    for(int j = 0; j < n; j++) {
        // Upper triangular matrix U
        for(int i = 0; i <= j; i++) 
        {
            double sum = 0.0;
            for(int k = 0; k < i; k++) 
            {
                sum += L.data[i][k] * U.data[k][j];
            }
            U.data[i][j] = data[i][j] - sum;
        }
        
        // Lower triangular matrix L
        for(int i = j + 1; i < n; i++)
         {
            double sum = 0.0;
            for(int k = 0; k < j; k++) 
            {
                sum += L.data[i][k] * U.data[k][j];
            }
            
            L.data[i][j] = (data[i][j] - sum) / U.data[j][j];
        }
    }
    
    return {L, U};
}

// Crout LU Decomposition
pair<Matrix, Matrix> Matrix::luDecompositionCrout() {
    if (rows != cols) {
        cout << "Matrix must be square for LU decomposition!" << endl;
        return {Matrix(), Matrix()};
    }
    
    int n = rows;
    Matrix L(n, n);
    Matrix U(n, n);
    
    for(int i = 0; i < n; i++) {
        U.data[i][i] = 1.0;
    }
    
    for(int j = 0; j < n; j++) {
        // Lower triangular matrix L
        for(int i = j; i < n; i++) {
            double sum = 0.0;
            for(int k = 0; k < j; k++) {
                sum += L.data[i][k] * U.data[k][j];
            }
            L.data[i][j] = data[i][j] - sum;
        }
        
        // Upper triangular matrix U
        for(int i = j + 1; i < n; i++) {
            double sum = 0.0;
            for(int k = 0; k < j; k++) {
                sum += L.data[j][k] * U.data[k][i];
            }
            
            U.data[j][i] = (data[j][i] - sum) / L.data[j][j];
        }
    }
    
    return {L, U};
}

// Cholesky Decomposition
Matrix Matrix::choleskyDecomposition() {
    if (rows != cols) {
        cout << "Matrix must be square for Cholesky decomposition!" << endl;
        return Matrix();
    }
    
    if (!isSymmetric()) {
        cout << "Matrix must be symmetric for Cholesky decomposition!" << endl;
        return Matrix();
    }
    
    int n = rows;
    Matrix L(n, n);
    
    for(int i = 0; i < n; i++) {
        for(int j = 0; j <= i; j++) {
            double sum = 0;
            
            if (j == i) { 
                for(int k = 0; k < j; k++) {
                    sum += L.data[j][k] * L.data[j][k];
                }
                
                double value = data[j][j] - sum;
                if (value <= 0) {
                    cout << "Matrix is not positive definite!" << endl;
                    return Matrix();
                }
                
                L.data[j][j] = sqrt(value);
            } else { 
                for(int k = 0; k < j; k++) {
                    sum += L.data[i][k] * L.data[j][k];
                }
                
                L.data[i][j] = (data[i][j] - sum) / L.data[j][j];
            }
        }
    }
    
    return L;
}

// Solve a system Ax = b using precomputed LU decomposition
vector<double> Matrix::solveLU(vector<double>& b, Matrix& L, Matrix& U) {
    int n = rows;
    if (n != b.size() || L.getRows() != n || U.getRows() != n) {
        cout << "Dimensions mismatch in LU solver!" << endl;
        return vector<double>();
    }
    
    vector<double> y(n, 0);
    for(int i = 0; i < n; i++) {
        y[i] = b[i];
        for(int j = 0; j < i; j++) {
            y[i] -= L.data[i][j] * y[j];
        }
        y[i] /= L.data[i][i];
    }
    
    vector<double> x(n, 0);
    for(int i = n - 1; i >= 0; i--) {
        x[i] = y[i];
        for(int j = i + 1; j < n; j++) {
            x[i] -= U.data[i][j] * x[j];
        }
        x[i] /= U.data[i][i];
    }
    
    return x;
}

// Calculate determinant using the LU decomposition
double Matrix::determinant() {
    if (rows != cols) {
        cout << "Matrix must be square to calculate determinant!" << endl;
        return 0;
    }
    
    auto [L, U] = luDecompositionDoolittle();
    
    if (L.getRows() == 0 || U.getRows() == 0) {
        return 0;
    }
    
    double det = 1.0;
    for(int i = 0; i < rows; i++) {
        det *= U.data[i][i];
    }
    
    return det;
}

bool Matrix::isDiagonallyDominant() {
    if (rows != cols) {
        return false;
    }
    
    for(int i = 0; i < rows; i++) {
        double diagonalValue = fabs(data[i][i]);
        double rowSum = 0.0;
        
        for(int j = 0; j < cols; j++) 
        {
            if(i!=j) 
            {
                rowSum += fabs(data[i][j]);
            }
        }
        
        if (diagonalValue <= rowSum) 
        {
            return false;
        }
    }
    
    return true;
}

bool Matrix::makeDiagonallyDominant(){
    if(rows != cols){
        cout << "Matrix must be square to attempt making it diagonally dominant!" << endl;
        return false;
    }
 
    for(int i = 0; i < rows; i++){
        int bestRow = i;
        double bestValue = fabs(data[i][i]);
        double rowSum = 0.0;

        for(int j = 0; j < cols; j++){
            if(i != j){
                rowSum += fabs(data[i][j]);
            }
        }

        if(bestValue <= rowSum) {
            for(int k = i + 1; k < rows; k++) {
                double potentialDiag = fabs(data[k][i]);
                double potentialSum = 0.0;

                for(int j = 0; j < cols; j++) {
                    if (j != i) {
                        potentialSum += fabs(data[k][j]);
                    }
                }

                if(potentialDiag > potentialSum) {
                    bestRow = k;
                    break;
                }
            }

            if (bestRow != i) {
                swap(data[i], data[bestRow]);
            } else {
                cout << "Cannot make matrix diagonally dominant at row " << i << endl;
                return false;
            }
        }
    }
    return true;
}


// Gauss-Jacobi
vector<double> Matrix::gaussJacobi(vector<double>& b, int maxIterations, double tolerance) 
{
    if (rows != cols) 
    {
        cout << "Matrix must be square for Gauss-Jacobi method!" << endl;
        return vector<double>();
    }
    
    if (rows != (int)b.size()) 
    {
        cout << "Vector b must have the same size as matrix rows!" << endl;
        return vector<double>();
    }
    
    int n = rows;
    vector<double> x(n, 0.0); 
    vector<double> x_new(n, 0.0);
    
    if (!isDiagonallyDominant()) 
    {
    	if(!makeDiagonallyDominant()){
        	cout << "Warning: Matrix is not diagonally dominant, Gauss-Jacobi may not converge!" << endl;
    	}
    }
    
    for (int i = 0; i < n; i++) 
    {
        if(fabs(data[i][i]) < 1e-10) 
        {
            cout << "Error: Zero diagonal element detected. Cannot use Gauss-Jacobi method!" << endl;
            return vector<double>();
        }
    }
    
    // Approximation
    for(int iter = 0; iter < maxIterations; iter++) 
    {
        for(int i = 0; i < n; i++) 
        {
            double sum = 0.0;
            for(int j = 0; j < n; j++) 
            {
                if(j!=i) 
                {
                    sum += data[i][j] * x[j];
                }
            }
            x_new[i] = (b[i] - sum) / data[i][i];
        }
        
        // Check convergence
        double error = 0.0;
        for (int i = 0; i < n; i++) 
        {
            error += fabs(x_new[i] - x[i]);
        }
        
        x = x_new;
        
        if (error < tolerance) 
        {
            cout << "Gauss-Jacobi converged after " << iter + 1 << " iterations." << endl;
            return x;
        }
    }
    
    cout << "Gauss-Jacobi did not converge within " << maxIterations << " iterations." << endl;
    return x;
}

// Gauss-Seidel 
vector<double> Matrix::gaussSeidel(vector<double>& b, int maxIterations, double tolerance) {
    if(rows != cols) 
    {
        cout << "Matrix must be square for Gauss-Seidel method!" << endl;
        return vector<double>();
    }
    
    if(rows != (int)b.size()) 
    {
        cout << "Vector b must have the same size as matrix rows!" << endl;
        return vector<double>();
    }
    
    int n = rows;
    vector<double> x(n, 0.0);    
    vector<double> x_prev(n, 0.0); 
    
    if (!isDiagonallyDominant()) 
    {
        if(!makeDiagonallyDominant()){
        	cout << "Warning: Matrix is not diagonally dominant, Gauss-Jacobi may not converge!" << endl;
    	}
    }
    
    for(int i = 0; i < n; i++) 
    {
        if (fabs(data[i][i]) < 1e-10) 
        {
            cout << "Error: Zero diagonal element detected. Cannot use Gauss-Seidel method!" << endl;
            return vector<double>();
        }
    }
    
    //Approximation
    for(int iter = 0; iter < maxIterations; iter++) 
    {
        x_prev = x;
        
        for(int i = 0; i < n; i++){
            double sum1 = 0.0;
            double sum2 = 0.0;
            
            for(int j = 0; j < i; j++) 
            {
                sum1 += data[i][j] * x[j]; 
            }
            
            for(int j = i + 1; j < n; j++) 
            {
                sum2 += data[i][j] * x_prev[j];
            }
            
            x[i] = (b[i] - sum1 - sum2) / data[i][i];
        }
        
        // Check Convergence
        double error = 0.0;
        for(int i = 0; i < n; i++) 
        {
            error += fabs(x[i] - x_prev[i]);
        }
        
        if(error < tolerance) 
        {
            cout << "Gauss-Seidel converged after " << iter + 1 << " iterations." << endl;
            return x;
        }
    }
    
    cout << "Gauss-Seidel did not converge within " << maxIterations << " iterations." << endl;
    return x;
}
