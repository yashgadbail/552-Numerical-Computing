#include "Complex.hpp"
#include <iostream>

int main() {
    double real1, imag1, real2, imag2;
    std::cout << "Enter first complex number:\n";
    std::cout << "Real part: ";
    std::cin >> real1;
    std::cout << "Imaginary part: ";
    std::cin >> imag1;

    std::cout << "\nEnter second complex number:\n";
    std::cout << "Real part: ";
    std::cin >> real2;
    std::cout << "Imaginary part: ";
    std::cin >> imag2;

    Complex c1(real1, imag1);
    Complex c2(real2, imag2);

    std::cout << "\nComplex Number 1: ";
    c1.display();

    std::cout << "Complex Number 2: ";
    c2.display();

    Complex sum = c1 + c2;
    std::cout << "Sum: ";
    sum.display();

    Complex diff = c1 - c2;
    std::cout << "Difference: ";
    diff.display();

    return 0;
}
