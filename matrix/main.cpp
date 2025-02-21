#include "matrix.hpp"

Matrix getMatrixFromFile(string matrixName) {
    string filename;
    cout << "Enter filename (without extension) for " << matrixName << ": ";
    cin >> filename;
    return Matrix::readFromFile(filename);
}

int main() {
    // Get matrices from two separate files
    Matrix A = getMatrixFromFile("Matrix A");
    Matrix B = getMatrixFromFile("Matrix B");

    if (A.getRows() == 0 || B.getRows() == 0) {
        cout << "Error reading matrices." << endl;
        return 1;
    }

    cout << "Matrix A:\n" << A;
    cout << "Matrix B:\n" << B;

    // Addition
    Matrix C = A + B;
    cout << "A + B:\n" << C;
    C.writeToFile("output_addition");

    // Subtraction
    Matrix D = A - B;
    cout << "A - B:\n" << D;
    D.writeToFile("output_subtraction");

    // Identity Matrix
    if (A.getRows() == A.getCols()) {
        Matrix I = Matrix::identityMatrix(A.getRows());
        cout << "Identity Matrix of size " << A.getRows() << "x" << A.getRows() << ":\n" << I;
        I.writeToFile("output_identity");
    }

    // Check if A is symmetric
    if (A.isSymmetric())
        cout << "Matrix A is symmetric." << endl;
    else
        cout << "Matrix A is not symmetric." << endl;

    if (B.isSymmetric())
        cout << "Matrix B is symmetric." << endl;
    else
        cout << "Matrix B is not symmetric." << endl;

    return 0;
}
