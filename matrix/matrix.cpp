#include "matrix.hpp"

// Default Constructor
Matrix::Matrix(){rows=0;cols=0;}

// Parameterized Constructor
Matrix::Matrix(int r,int c){
    rows=r;cols=c;
    data=vector<vector<double>>(r,vector<double>(c,0));
}

// Copy Constructor
Matrix::Matrix(const Matrix& other){
    rows=other.rows;
    cols=other.cols;data=other.data;
}

// Getters
int Matrix::getRows(){return rows;}
int Matrix::getCols(){return cols;}

// Print matrix
void Matrix::print(){cout<<*this;}

// Write matrix to a file
void Matrix::writeToFile(string filename){
    ofstream outFile(filename+".txt");
    if(!outFile){cout<<"Error opening file!"<<endl;return;}
    outFile<<*this;
}

// Addition
Matrix Matrix::operator+(Matrix& other){
    if(rows!=other.rows||cols!=other.cols){
        cout<<"Error: Matrices must be of the same dimensions!"<<endl;
        return Matrix();
    }
    Matrix result(rows,cols);
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            result.data[i][j]=data[i][j]+other.data[i][j];
        }
    }
    return result;
}

// Subtraction
Matrix Matrix::operator-(Matrix& other){
    if(rows!=other.rows||cols!=other.cols){
        cout<<"Error: Matrices must be of the same dimensions!"<<endl;
        return Matrix();
    }
    Matrix result(rows,cols);
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            result.data[i][j]=data[i][j]-other.data[i][j];
        }
    }
    return result;
}

// Identity Matrix Generator
Matrix Matrix::identityMatrix(int size){
    Matrix identity(size,size);
    for(int i=0;i<size;i++){identity.data[i][i]=1;}
    return identity;
}

// Multiplication
Matrix Matrix::operator*(Matrix& other){
    if(cols!=other.rows){
        cout<<"Matrix dimensions do not match for multiplication!\n";
        return Matrix();
    }
    Matrix result(rows,other.cols);
    for(int i=0;i<rows;i++)
        for(int j=0;j<other.cols;j++)
            for(int k=0;k<cols;k++)
                result.data[i][j]+=data[i][k]*other.data[k][j];
    return result;
}

// Check if matrix is symmetric
bool Matrix::isSymmetric(){
    if(rows!=cols){return false;}
    for(int i=0;i<rows;i++){
        for(int j=0;j<i;j++){
            if(data[i][j]!=data[j][i]){return false;}
        }
    }
    return true;
}

// Read Matrix from a file
Matrix Matrix::readFromFile(string filename){
    ifstream inFile(filename+".txt");
    if(!inFile){cout<<"Error opening file!"<<endl;return Matrix();}
    int r,c;
    inFile>>r>>c;
    Matrix mat(r,c);
    for(int i=0;i<r;i++){
        for(int j=0;j<c;j++){
            inFile>>mat.data[i][j];
        }
    }
    return mat;
}

// Output operator
ostream& operator<<(ostream& os, Matrix& mat) {
    for (int i = 0; i < mat.rows; i++) {
        for (int j = 0; j < mat.cols; j++)
            os << mat.data[i][j] << " ";
        os << endl;
    }
    return os;
}

// Input operator
istream& operator>>(istream& is, Matrix& mat) {
    for (int i = 0; i < mat.rows; i++)
        for (int j = 0; j < mat.cols; j++)
            is >> mat.data[i][j];
    return is;
}
