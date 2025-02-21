#include "matrix.hpp"
#include <fstream>  // Include for file writing

int main(){
    string choice;
    cout<<"Read from file (f) or input manually (m)? ";
    cin>>choice;

    Matrix A,B;

    if(choice=="f"){
        string filenameA,filenameB;
        cout<<"Enter filename for Matrix A (without extension): ";
        cin>>filenameA;
        A=Matrix::readFromFile(filenameA);

        cout<<"Enter filename for Matrix B (without extension): ";
        cin>>filenameB;
        B=Matrix::readFromFile(filenameB);
    }
    else{
        int r,c;
        cout<<"Enter dimensions for Matrix A: ";
        cin>>r>>c;
        A=Matrix(r,c);
        cout<<"Enter elements for Matrix A:\n";
        cin>>A;

        cout<<"Enter dimensions for Matrix B: ";
        cin>>r>>c;
        B=Matrix(r,c);
        cout<<"Enter elements for Matrix B:\n";
        cin>>B;
    }

    // Open output file
    ofstream outFile("output.txt");

    cout<<"\nMatrix A:\n"<<A;
    outFile<<"\nMatrix A:\n"<<A;

    cout<<"\nMatrix B:\n"<<B;
    outFile<<"\nMatrix B:\n"<<B;

    Matrix C=A+B;
    cout<<"\nMatrix A + B:\n"<<C;
    outFile<<"\nMatrix A + B:\n"<<C;

    Matrix D=A-B;
    cout<<"\nMatrix A - B:\n"<<D;
    outFile<<"\nMatrix A - B:\n"<<D;

    if(A.isSymmetric()){ 
        cout<<"\nMatrix A is symmetric.\n"; 
        outFile<<"\nMatrix A is symmetric.\n"; 
    }
    else{ 
        cout<<"\nMatrix A is NOT symmetric.\n"; 
        outFile<<"\nMatrix A is NOT symmetric.\n"; 
    }

    if(B.isSymmetric()){ 
        cout<<"Matrix B is symmetric.\n"; 
        outFile<<"Matrix B is symmetric.\n"; 
    }
    else{ 
        cout<<"Matrix B is NOT symmetric.\n"; 
        outFile<<"Matrix B is NOT symmetric.\n"; 
    }

    int size;
    cout<<"\nEnter size for Identity Matrix: ";
    cin>>size;

    Matrix I=Matrix::identityMatrix(size);
    cout<<"\nIdentity Matrix:\n"<<I;
    outFile<<"\nIdentity Matrix:\n"<<I;

    outFile.close();  // Close file after writing

    cout<<"\nResults saved to output.txt\n";

    return 0;
}
