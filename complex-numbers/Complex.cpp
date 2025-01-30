#include "Complex.hpp"
#include <iostream>

Complex::Complex() : real(0), imag(0) {}

Complex::Complex(double r, double i) : real(r), imag(i) {}

double Complex::getReal() const { return real; }
double Complex::getImag() const { return imag; }

void Complex::setReal(double r) { real = r; }
void Complex::setImag(double i) { imag = i; }

Complex Complex::operator+(const Complex& other) const {
    return Complex(real + other.real, imag + other.imag);
}

Complex Complex::operator-(const Complex& other) const {
    return Complex(real - other.real, imag - other.imag);
}

void Complex::display() const {
    if(imag == 1 || imag == -1){
        std::cout << real << " + " << imag << "i" << std::endl;
    } else {
        if (imag < 0) {
            std::cout << real << " - " << -imag << "i" << std::endl;
        } else {
            std::cout << real << " + " << imag << "i" << std::endl;
        }
    }
}
