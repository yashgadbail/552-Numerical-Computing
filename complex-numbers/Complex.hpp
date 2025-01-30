#ifndef COMPLEX_HPP
#define COMPLEX_HPP

#include <iostream>

class Complex {

    private:
        double real;
        double imag;

    public:
        Complex();
        Complex(double r, double i);

        double getReal() const;
        double getImag() const;
        void setReal(double r);
        void setImag(double i);

        Complex operator+(const Complex& other) const;
        Complex operator-(const Complex& other) const;

        void display() const;
};

#endif
