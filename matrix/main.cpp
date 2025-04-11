#include <iostream>
#include "matrix.hpp"
#include <vector>
#include <iomanip>
#include <fstream>
using namespace std;

// Read matrix from file
Matrix getMatrixFromFile(const string& matrixName) {
    string filename;
    cout << "Enter filename (without extension) for " << matrixName << ": ";
    cin >> filename;
    return Matrix::readFromFile(filename);
}

// Read vector from file
vector<double> getVectorFromFile(const string& vectorName) {
    string filename;
    cout << "Enter filename (without extension) for " << vectorName << ": ";
    cin >> filename;
    
    ifstream inFile(filename + ".txt");
    if (!inFile) {
        cout << "Error opening file for " << vectorName << "!" << endl;
        return vector<double>();
    }

    int size;
    inFile >> size;

    vector<double> v(size);
    for (int i = 0; i < size; ++i)
        inFile >> v[i];

    return v;
}

// Output vector to file
void writeVectorToFile(const vector<double>& v, const string& filename) {
    ofstream outFile(filename + ".txt");
    if (!outFile) {
        cout << "Error opening file for writing!" << endl;
        return;
    }

    outFile << v.size() << endl;
    for (double val : v)
        outFile << setprecision(8) << val << endl;

    cout << "Vector written to " << filename << ".txt" << endl;
}

// Print vector
void printVector(const vector<double>& v, const string& name) {
    if (!name.empty()) cout << name << ": ";
    for (double val : v)
        cout << setprecision(4) << val << " ";
    cout << endl;
}

int main() {
    string choice;
    cout << "Read matrices and vectors from file (f) or input manually (m)? ";
    cin >> choice;
    bool fromFile = (choice == "f");

    Matrix A, B;
    vector<double> b;

    cout << "\nChoose operation type:\n";
    cout << "1. Basic Matrix Operations (A and B)\n";
    cout << "2. Solve Linear System (A and b)\n";
    cout << "3. LU Decomposition (A only)\n";
    cout << "Enter choice (1-3): ";
    int mainChoice;
    cin >> mainChoice;

    if (mainChoice == 1) {
        // Matrix A and B
        if (fromFile) {
            A = getMatrixFromFile("Matrix A");
            B = getMatrixFromFile("Matrix B");
        } else {
            int r, c;
            cout << "Enter dimensions for Matrix A: ";
            cin >> r >> c;
            A = Matrix(r, c);
            cout << "Enter elements for Matrix A:\n";
            cin >> A;

            cout << "Enter dimensions for Matrix B: ";
            cin >> r >> c;
            B = Matrix(r, c);
            cout << "Enter elements for Matrix B:\n";
            cin >> B;
        }

        if (A.getRows() == 0 || B.getRows() == 0) {
            cout << "Error reading matrices.\n";
            return 1;
        }

        cout << "\nMatrix A:\n" << A;
        cout << "\nMatrix B:\n" << B;

        // Perform Basic Matrix Operations
        if (A.getRows() == B.getRows() && A.getCols() == B.getCols()) {
            Matrix C = A + B;
            cout << "\nA + B:\n" << C;
            C.writeToFile("output_addition");

            Matrix D = A - B;
            cout << "\nA - B:\n" << D;
            D.writeToFile("output_subtraction");
        } else {
            cout << "\nAddition and subtraction not possible (dimension mismatch).\n";
        }

        if (A.getCols() == B.getRows()) {
            Matrix E = A * B;
            cout << "\nA * B:\n" << E;
            E.writeToFile("output_multiplication");
        } else {
            cout << "\nMultiplication not possible (A.cols != B.rows).\n";
        }

        cout << "\nMatrix A is " << (A.isSymmetric() ? "" : "not ") << "symmetric.\n";
        cout << "Matrix B is " << (B.isSymmetric() ? "" : "not ") << "symmetric.\n";

        Matrix AT = A.transpose();
        cout << "\nTranspose of A:\n" << AT;
        AT.writeToFile("output_transpose");
    }

    else if (mainChoice == 2) {
        // Linear System Solver: Matrix A and vector b
        if (fromFile) {
            A = getMatrixFromFile("Matrix A");
            b = getVectorFromFile("Vector b");
        } else {
            int r, c;
            cout << "Enter dimensions for Matrix A: ";
            cin >> r >> c;
            A = Matrix(r, c);
            cout << "Enter elements for Matrix A:\n";
            cin >> A;

            b.resize(r);
            cout << "Enter elements for vector b (size " << r << "):\n";
            for (int i = 0; i < r; ++i)
                cin >> b[i];
        }

        int n = A.getRows();
        if (A.getCols() != n) {
            cout << "Matrix A must be square!\n";
            return 1;
        }

        if ((int)b.size() != n) {
            cout << "Vector b size does not match A row count!\n";
            return 1;
        }

        cout << "\nChosen Vector b: ";
        printVector(b, "");

        cout << "\nChoose solver method:\n";
        cout << "1. Gaussian Elimination\n";
        cout << "2. Gauss-Jacobi Iteration\n";
        cout << "3. Gauss-Seidel Iteration\n";
        cout << "Enter method (1-3): ";
        int solverChoice;
        cin >> solverChoice;

        switch (solverChoice) {
            case 1: {
                vector<double> x = A.gaussianElimination(b);
                if (!x.empty()) {
                    printVector(x, "Solution by Gaussian Elimination");
                }
                break;
            }
            case 2:
            case 3: {
                cout << "Matrix is " << (A.isDiagonallyDominant() ? "" : "not ") << "diagonally dominant.\n";
                int maxIter;
                double tol;
                cout << "Enter max iterations: ";
                cin >> maxIter;
                cout << "Enter tolerance: ";
                cin >> tol;

                vector<double> x = (solverChoice == 2) ?
                    A.gaussJacobi(b, maxIter, tol) :
                    A.gaussSeidel(b, maxIter, tol);

                if (!x.empty()) {
                    string method = (solverChoice == 2) ? "Gauss-Jacobi" : "Gauss-Seidel";
                    printVector(x, "Solution by " + method);

                    cout << "\nVerification (A*x):\n";
                    for (int i = 0; i < n; i++) {
                        double sum = 0.0;
                        for (int j = 0; j < n; j++)
                            sum += A.get(i, j) * x[j];
                        cout << "Row " << i+1 << ": " << sum << " \t(expected " << b[i] << ")\n";
                    }
                }
                break;
            }
            default:
                cout << "Invalid solver choice.\n";
        }
    }

    else if (mainChoice == 3) {
        // LU Decomposition: Matrix A only
        if (fromFile) {
            A = getMatrixFromFile("Matrix A");
        } else {
            int r, c;
            cout << "Enter dimensions for Matrix A: ";
            cin >> r >> c;
            A = Matrix(r, c);
            cout << "Enter elements for Matrix A:\n";
            cin >> A;
        }

        if (A.getRows() != A.getCols()) {
            cout << "Matrix A must be square for LU decomposition!\n";
            return 1;
        }

        cout << "\nChoose LU method:\n";
        cout << "1. Doolittle\n";
        cout << "2. Crout\n";
        cout << "3. Cholesky (for symmetric matrices)\n";
        cout << "Enter method (1-3): ";
        int luChoice;
        cin >> luChoice;

        switch (luChoice) {
            case 1: {
                auto [L, U] = A.luDecompositionDoolittle();
                cout << "\nDoolittle Decomposition:\nL:\n" << L << "U:\n" << U;
                L.writeToFile("output_doolittle_L");
                U.writeToFile("output_doolittle_U");
                break;
            }
            case 2: {
                auto [L, U] = A.luDecompositionCrout();
                cout << "\nCrout Decomposition:\nL:\n" << L << "U:\n" << U;
                L.writeToFile("output_crout_L");
                U.writeToFile("output_crout_U");
                break;
            }
            case 3: {
                if (!A.isSymmetric()) {
                    cout << "Matrix is not symmetric. Cholesky not applicable.\n";
                    break;
                }
                Matrix L = A.choleskyDecomposition();
                Matrix LT = L.transpose();
                cout << "\nCholesky Decomposition:\nL:\n" << L << "L^T:\n" << LT;
                L.writeToFile("output_cholesky_L");
                break;
            }
            default:
                cout << "Invalid choice.\n";
        }
    }

    else {
        cout << "Invalid operation choice.\n";
    }

    return 0;
}
