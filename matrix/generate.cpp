#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>  // For rand() and srand()
#include <ctime>    // For time()
#include <cmath>    // For abs()
using namespace std;

void generateDiagonallyDominantMatrix(int rows, int cols, const string& filename, bool symmetric) {
    ofstream outFile(filename + ".txt");
    if (!outFile) {
        cout << "Error: Unable to open file for writing.\n";
        return;
    }
    
    // Write matrix dimensions
    outFile << rows << " " << cols << endl;
    
    // Seed random number generator
    srand(time(0));
    
    // Dynamically allocate memory for the matrix
    double** matrix = new double*[rows];
    for (int i = 0; i < rows; i++) {
        matrix[i] = new double[cols];
    }
    
    // Generate matrix values
    for (int i = 0; i < rows; i++) {
        double rowSum = 0.0;
        
        // Fill the non-diagonal elements first
        for (int j = 0; j < cols; j++) {
            if (i != j || i >= cols) { // Non-diagonal element or outside the square part
                double randomValue = (rand() % 2000 - 1000) / 10.0; // Range: -100.0 to 100.0
                
                if (symmetric && i < cols && j < rows && i > j) {
                    // For symmetric matrices, copy the value from the upper triangle
                    // (only applies to the square part of the matrix)
                    matrix[i][j] = matrix[j][i];
                } else {
                    matrix[i][j] = randomValue;
                    if (symmetric && i < cols && j < rows && i < j) {
                        matrix[j][i] = randomValue; // Mirror value for symmetry
                    }
                }
                
                rowSum += fabs(matrix[i][j]);
            }
        }
        
        // Make the diagonal element larger than the sum of other elements in the row
        // (only applicable for the square part of the matrix)
        if (i < cols) {
            // Adding a small random amount to ensure strict diagonal dominance
            double extraValue = (rand() % 1000) / 10.0 + 1.0; // Range: 1.0 to 101.0
            double diagValue = rowSum + extraValue;
            
            // Randomly make some diagonal elements negative (preserves diagonal dominance)
            if (rand() % 2 == 0) {
                diagValue = -diagValue;
            }
            
            matrix[i][i] = diagValue;
        }
    }
    
    // Write the matrix to file
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            outFile << setw(12) << fixed << setprecision(2) << matrix[i][j] << " ";
        }
        outFile << endl;
    }
    
    // Free dynamically allocated memory
    for (int i = 0; i < rows; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
    
    outFile.close();
    cout << "Diagonally dominant matrix saved to " << filename << ".txt" << endl;
}

int main() {
    int rows, cols;
    string filename;
    char squareChoice, symmetricChoice;
    bool isSquare, isSymmetric = false;
    
    cout << "Do you want to generate a square matrix (NxN)? (y/n): ";
    cin >> squareChoice;
    
    isSquare = (squareChoice == 'y' || squareChoice == 'Y');
    
    if (isSquare) {
        cout << "Enter matrix size (N for NxN): ";
        cin >> rows;
        cols = rows;
        
        cout << "Generate a symmetric matrix? (y/n): ";
        cin >> symmetricChoice;
        isSymmetric = (symmetricChoice == 'y' || symmetricChoice == 'Y');
    } else {
        cout << "Enter number of rows (N for NxM): ";
        cin >> rows;
        cout << "Enter number of columns (M for NxM): ";
        cin >> cols;
    }
    
    if (rows <= 0 || cols <= 0) {
        cout << "Error: Matrix dimensions must be positive.\n";
        return 1;
    }
    
    cout << "Enter filename (without extension): ";
    cin >> filename;
    
    generateDiagonallyDominantMatrix(rows, cols, filename, isSymmetric);
    
    return 0;
}
