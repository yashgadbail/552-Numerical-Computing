#include <iostream>
#include "matrix.hpp"
#include <vector>
#include <iomanip>
#include <fstream>
using namespace std;

Matrix getMatrixFromFile(string matrixName) {
    string filename;
    cout << "Enter filename (without extension) for " << matrixName << ": ";
    cin >> filename;
    return Matrix::readFromFile(filename);
}

vector<double> getVectorFromFile(string vectorName) {
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
    for(int i = 0; i < size; i++) {
        inFile >> v[i];
    }
    
    return v;
}

void writeVectorToFile(const vector<double>& v, const string& filename) {
    ofstream outFile(filename + ".txt");
    if (!outFile) {
        cout << "Error opening file for writing!" << endl;
        return;
    }
    
    outFile << v.size() << endl;
    for(size_t i = 0; i < v.size(); i++) {
        outFile << setprecision(8) << v[i] << endl;
    }
    
    cout << "Vector written to " << filename << ".txt" << endl;
}

void printVector(const vector<double>& v, const string& name) {
    cout << name << ": ";
    for (double val : v) {
        cout << setprecision(4) << val << " ";
    }
    cout << endl;
}

int main() {
    string choice;
    cout << "Read from file (f) or input manually (m)? ";
    cin >> choice;

    Matrix A, B;

    if (choice == "f") {
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

    if(A.getRows() == 0 || B.getRows() == 0) {
        cout << "Error reading matrices.\n";
        return 1;
    }

    cout << "\nMatrix A:\n" << A;
    cout << "\nMatrix B:\n" << B;

    cout << "\nChoose operation type:\n";
    cout << "1. Basic Matrix Operations\n";
    cout << "2. Gaussian Elimination\n";
    cout << "3. LU Decomposition\n";
    cout << "Enter choice (1-3): ";
    int mainChoice;
    cin >> mainChoice;

    switch (mainChoice) {
        case 1: {
            // Basic matrix operations
            if (A.getRows() == B.getRows() && A.getCols() == B.getCols()) {
                Matrix C = A + B;
                cout << "\nA + B:\n" << C;
                C.writeToFile("output_addition");
            } else {
                cout << "\nAddition not possible (different dimensions).\n";
            }

            if (A.getRows() == B.getRows() && A.getCols() == B.getCols()) {
                Matrix D = A - B;
                cout << "\nA - B:\n" << D;
                D.writeToFile("output_subtraction");
            } else {
                cout << "\nSubtraction not possible (different dimensions).\n";
            }

            if (A.getCols() == B.getRows()) {
                Matrix E = A * B;
                cout << "\nA * B:\n" << E;
                E.writeToFile("output_multiplication");
            } else {
                cout << "\nMultiplication not possible (A columns != B rows).\n";
            }

            cout << "\nMatrix A is " << (A.isSymmetric() ? "symmetric." : "not symmetric.") << "\n";
            cout << "Matrix B is " << (B.isSymmetric() ? "symmetric." : "not symmetric.") << "\n";
            
            Matrix AT = A.transpose();
            cout << "\nTranspose of A:\n" << AT;
            AT.writeToFile("output_transpose");
            
            break;
        }
        

		case 2: {
			// Linear system solvers
			if (A.getRows() != A.getCols()) {
				cout << "Matrix A must be square to solve linear systems!\n";
				break;
			}
			
			int n = A.getRows();
			vector<double> b;
			
			cout << "Read right-hand side vector b from file (f) or input manually (m)? ";
			string bChoice;
			cin >> bChoice;
			
			if (bChoice == "f") {
				b = getVectorFromFile("Vector b");
				if (b.empty()) {
				    cout << "Error reading vector b from file.\n";
				    break;
				}
				
				if ((int)b.size() != n) {
				    cout << "Vector b size (" << b.size() << ") doesn't match matrix A rows (" << n << ")!\n";
				    break;
				}
			} else {
				b.resize(n);
				cout << "Enter the right-hand side vector b (size " << n << "):\n";
				for (int i = 0; i < n; i++) {
				    cin >> b[i];
				}
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
				        
				        cout << "\nVerification (A*x should equal b):\n";
				        for (int i = 0; i < n; i++) {
				            double sum = 0.0;
				            for (int j = 0; j < n; j++) {
				                sum += A.get(i, j) * x[j];
				            }
				            cout << "Row " << i+1 << ": " << sum << "\t(actual " << b[i] << ")\n";
				        }
				    }
				    break;
				}
				case 2: {
				    cout << "Matrix is " << (A.isDiagonallyDominant() ? "diagonally dominant." : "not diagonally dominant.") << endl;
				    cout << "Enter maximum iterations: ";
				    int maxIter;
				    cin >> maxIter;
				    cout << "Enter tolerance: ";
				    double tol;
				    cin >> tol;
				    
				    vector<double> x = A.gaussJacobi(b, maxIter, tol);
				    if (!x.empty()) 
				    {
				        printVector(x, "Solution by Gauss-Jacobi Method");
				        
				        cout << "\nVerification (A*x should equal b):\n";
				        for (int i = 0; i < n; i++) {
				            double sum = 0.0;
				            for (int j = 0; j < n; j++) {
				                sum += A.get(i, j) * x[j];
				            }
				            cout << "Row " << i+1 << ": " << sum << "\t(actual " << b[i] << ")\n";
				        }
				    }
				    break;
				}
				case 3: {
				    cout << "Matrix is " << (A.isDiagonallyDominant() ? "diagonally dominant." : "not diagonally dominant.") << endl;
				    cout << "Enter maximum iterations: ";
				    int maxIter;
				    cin >> maxIter;
				    cout << "Enter tolerance: ";
				    double tol;
				    cin >> tol;
				    
				    vector<double> x = A.gaussSeidel(b, maxIter, tol);
				    if (!x.empty()) 
				    {
				        printVector(x, "Solution by Gauss-Seidel Method");
				        
				        cout << "\nVerification (A*x should equal b):\n";
				        for (int i = 0; i < n; i++) {
				            double sum = 0.0;
				            for (int j = 0; j < n; j++) {
				                sum += A.get(i, j) * x[j];
				            }
				            cout << "Row " << i+1 << ": " << sum << "\t(actual " << b[i] << ")\n";
				        }
				    }
				    break;
				}
				default:
				    cout << "Invalid choice.\n";
			}
			break;
		}
        
        case 3: {
            // LU Decomposition
            if (A.getRows() != A.getCols()) {
                cout << "Matrix A must be square for LU decomposition!\n";
                break;
            }
            
            cout << "\nChoose LU decomposition method:\n";
            cout << "1. Do'Little Method (L has 1's on diagonal)\n";
            cout << "2. Crout Method (U has 1's on diagonal)\n";
            cout << "3. Cholesky Method (for symmetric matrices)\n";
            cout << "Enter method (1-3): ";
            int luChoice;
            cin >> luChoice;
            
            switch (luChoice) {
                case 1: {
                    auto [L,U] = A.luDecompositionDoolittle();
                    if (L.getRows() > 0 && U.getRows() > 0) {
                        cout << "\nDoolittle Decomposition of A:\n";
                        cout << "L matrix:\n" << L;
                        cout << "U matrix:\n" << U;
                        
                        // Verify decomposition
                        Matrix product = L * U;
                        cout << "\nL * U:\n" << product;
                        
                        L.writeToFile("output_doolittle_L");
                        U.writeToFile("output_doolittle_U");
                    }
                    break;
                }
                case 2: {
                    auto [L,U]  = A.luDecompositionCrout();
                    if (L.getRows() > 0 && U.getRows() > 0) {
                        cout << "\nCrout Decomposition of A:\n";
                        cout << "L matrix:\n" << L;
                        cout << "U matrix:\n" << U;
                        
                        // Verify decomposition
                        Matrix product = L * U;
                        cout << "\nL * U:\n" << product;
                        
                        L.writeToFile("output_crout_L");
                        U.writeToFile("output_crout_U");
                    }
                    break;
                }
                case 3: {
                    if (!A.isSymmetric()) {
                        cout << "Matrix A is not symmetric! Cannot use Cholesky decomposition.\n";
                        break;
                    }
                    
                    Matrix L = A.choleskyDecomposition();
                    if (L.getRows() > 0) {
                        cout << "\nCholesky Decomposition of A:\n";
                        cout << "L matrix:\n" << L;
                        
                        Matrix LT = L.transpose();
                        cout << "L^T matrix:\n" << LT;
                        
                        Matrix product = L * LT;
                        cout << "\nL * L^T:\n" << product;
                        
                        L.writeToFile("output_cholesky_L");
                    }
                    break;
                }
                default:
                    cout << "Invalid choice.\n";
            }
            break;
        }
        default:
            cout << "Invalid choice.\n";
    }

    return 0;
}
