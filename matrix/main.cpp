#include "matrix.hpp"

Matrix getMatrixFromFile(string matrixName){
    string filename;
    cout<<"Enter filename (without extension) for "<<matrixName<<": ";
    cin>>filename;
    return Matrix::readFromFile(filename);
}

int main(){
    string choice;
    cout<<"Read from file (f) or input manually (m)? ";
    cin>>choice;

    Matrix A,B;

    if(choice=="f"){
        A=getMatrixFromFile("Matrix A");
        B=getMatrixFromFile("Matrix B");
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

    if(A.getRows()==0||B.getRows()==0){
        cout<<"Error reading matrices.\n";
        return 1;
    }

    cout<<"\nMatrix A:\n"<<A;
    cout<<"\nMatrix B:\n"<<B;

    // Addition
    if(A.getRows()==B.getRows()&&A.getCols()==B.getCols()){
        Matrix C=A+B;
        cout<<"\nA + B:\n"<<C;
        C.writeToFile("output_addition");
    }
    else{
        cout<<"\nAddition not possible (different dimensions).\n";
    }

    // Subtraction
    if(A.getRows()==B.getRows()&&A.getCols()==B.getCols()){
        Matrix D=A-B;
        cout<<"\nA - B:\n"<<D;
        D.writeToFile("output_subtraction");
    }
    else{
        cout<<"\nSubtraction not possible (different dimensions).\n";
    }

    // Multiplication
    if(A.getCols()==B.getRows()){
        Matrix E=A*B;
        cout<<"\nA * B:\n"<<E;
        E.writeToFile("output_multiplication");
    }
    else{
        cout<<"\nMultiplication not possible (A columns != B rows).\n";
    }

    // Identity Matrix
    if(A.getRows()==A.getCols()){
        Matrix I=Matrix::identityMatrix(A.getRows());
        cout<<"\nIdentity Matrix of size "<<A.getRows()<<"x"<<A.getRows()<<":\n"<<I;
        I.writeToFile("output_identity");
    }

    // Symmetry Check
    cout<<"\nMatrix A is "<<(A.isSymmetric()?"symmetric.":"not symmetric.")<<"\n";
    cout<<"Matrix B is "<<(B.isSymmetric()?"symmetric.":"not symmetric.")<<"\n";

    return 0;
}
