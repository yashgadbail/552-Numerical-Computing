#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

const int N = 4; 

void printMatrix(double A[N][N]) 
{
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << A[i][j] << " ";
        }
        cout << endl;
    }
}

void printVector(double b[N]) {
    for (int i = 0; i < N; i++) {
        cout << b[i] << " ";
    }
    cout << endl;
}

void gaussianElimination(double A[N][N], double b[N], double x[N]) {
    for (int i = 0; i < N; i++) {
        int maxRow = i;
        for (int k = i + 1; k < N; k++) {
            if (fabs(A[k][i]) > fabs(A[maxRow][i])) {
                maxRow = k;
            }
        }

        // Swap rows
        for (int k = 0; k < N; k++) {
            swap(A[i][k], A[maxRow][k]);
        }
        swap(b[i], b[maxRow]);

        // Make diagonal 1 and reduce below
        for (int k = i + 1; k < N; k++) {
            double factor = A[k][i] / A[i][i];
            for (int j = i; j < N; j++) {
                A[k][j] -= factor * A[i][j];
            }
            b[k] -= factor * b[i];
        }
    }

    // Back substitution
    for (int i = N - 1; i >= 0; i--) {
        x[i] = b[i];
        for (int j = i + 1; j < N; j++) {
            x[i] -= A[i][j] * x[j];
        }
        x[i] /= A[i][i];
    }
}

// Function to calculate determinant using Laplace expansion
double determinant(double A[N][N]) {
    double det = 1.0;
    double temp[N][N];
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            temp[i][j] = A[i][j];
        }
    }

    for (int i = 0; i < N; i++) {
        int maxRow = i;
        for (int k = i + 1; k < N; k++) {
            if (fabs(temp[k][i]) > fabs(temp[maxRow][i])) {
                maxRow = k;
            }
        }
        if (maxRow != i) {
            for (int k = 0; k < N; k++) {
                swap(temp[i][k], temp[maxRow][k]);
            }
            det *= -1;
        }
        det *= temp[i][i];

        if (temp[i][i] == 0) {
            return 0;
        }

        for (int k = i + 1; k < N; k++) {
            double factor = temp[k][i] / temp[i][i];
            for (int j = i; j < N; j++) {
                temp[k][j] -= factor * temp[i][j];
            }
        }
    }
    return det;
}

// Function to read matrix from file
void readFromFile(double A[N][N], double b[N]) {
    ifstream file("matrix_input.txt");
    if (!file) {
        cout << "Error opening file!" << endl;
        return;
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            file >> A[i][j];
        }
    }
    for (int i = 0; i < N; i++) {
        file >> b[i];
    }
    file.close();
}

// Main function
int main() {
    double A[N][N] = {
        {10, 7, 8, 7},
        {7, 5, 6, 5},
        {8, 6, 10, 9},
        {7, 5, 9, 10}
    };

    double b[N] = {32, 23, 33, 31};

    double A_perturbed[N][N] = {
        {10, 7, 8.1, 7.2},
        {7.08, 5.04, 6, 5},
        {8, 5.98, 9.89, 9},
        {6.99, 4.99, 9, 9.92}
    };

    double b_perturbed[N] = {32.1, 22.9, 33.1, 30.9};

    double x[N];

    int choice;
    while (true) {
        cout << "\nChoose an operation:\n";
        cout << "1. Solve Ax = b\n";
        cout << "2. Solve A~x = b\n";
        cout << "3. Solve Ax = b~\n";
        cout << "4. Compute |A|\n";
        cout << "5. Read from file\n";
        cout << "6. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                gaussianElimination(A, b, x);
                cout << "Solution x: "; printVector(x);
                break;
            case 2:
                gaussianElimination(A_perturbed, b, x);
                cout << "Solution x: "; printVector(x);
                break;
            case 3:
                gaussianElimination(A, b_perturbed, x);
                cout << "Solution x: "; printVector(x);
                break;
            case 4:
                cout << "Determinant of A: " << determinant(A) << endl;
                break;
            case 5:
                readFromFile(A, b);
                cout << "Matrix and vector read from file.\n";
                break;
            case 6:
                return 0;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    }
}
