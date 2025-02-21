#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>  // For rand() and srand()
#include <ctime>    // For time()

using namespace std;

void generateMatrix(int n, const string& filename, bool symmetric) {
    ofstream outFile(filename + ".txt");
    if (!outFile) {
        cout << "Error: Unable to open file for writing.\n";
        return;
    }

    // Write matrix dimensions
    outFile << n << " " << n << endl;

    // Seed random number generator
    srand(time(0));

    // Dynamically allocate memory for the matrix
    double** matrix = new double*[n];
    for (int i = 0; i < n; i++) {
        matrix[i] = new double[n];
    }

    // Generate matrix values
    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {  // Fill only upper triangle
            double randomValue = (rand() % 10000 - 5000) / 10.0; // Range: -500.0 to 500.0
            matrix[i][j] = randomValue;
            if (symmetric) {
                matrix[j][i] = randomValue; // Mirror value for symmetry
            }
        }
    }

    // Write the matrix to file
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            outFile << setw(10) << fixed << setprecision(2) << matrix[i][j] << " ";
        }
        outFile << endl;
    }

    // Free dynamically allocated memory
    for (int i = 0; i < n; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;

    outFile.close();
    cout << "Matrix saved to " << filename << ".txt" << endl;
}

int main() {
    int n;
    string filename;
    char choice;

    cout << "Enter matrix size (N for NxN): ";
    cin >> n;
    
    if (n <= 0) {
        cout << "Error: Matrix size must be positive.\n";
        return 1;
    }

    cout << "Enter filename (without extension): ";
    cin >> filename;

    cout << "Generate a symmetric matrix? (y/n): ";
    cin >> choice;
    
    bool symmetric = (choice == 'y' || choice == 'Y');

    generateMatrix(n, filename, symmetric);

    return 0;
}
