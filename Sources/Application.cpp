#include <iostream>

#include "Math/Matrix.h"

#include "Tests/Test.h"

#include "Utils/UtilityFunctions.h"

int main() {
    int resultTests = 0;
#ifndef NDEBUG
    resultTests = RunTests();
#endif
    LOG("Task", 1);
    Matrix firstMatrix = Matrix(GetUserMatrix<double>());
    uint32_t row, col;
    std::cout << "Enter the row and column of the item you want to receive: ";
    std::cin >> row >> col;
    PrintMatrix(firstMatrix.GetStd(), "Input Matrix");
    LOG("Trace Input Matrix", firstMatrix.GetTrace());
    LOG("Requested element", firstMatrix.GetElement(row, col));
    std::cout << "\n";

    LOG("Task", 2);
    Matrix secondMatrix = Matrix(GetUserMatrix<double>());
    double scalar;
    std::cout << "Enter the value of the scalar by which you want to multiply the matrix: ";
    std::cin >> scalar;
    PrintMatrix((secondMatrix * scalar).GetStd(), "Matrix * scalar");
    std::cout << "\n";
    Matrix thirdMatrix = Matrix(GetUserMatrix<double>());
    Matrix fourthMatrix = Matrix(GetUserMatrix<double>());
    PrintMatrix((thirdMatrix + fourthMatrix).GetStd(), "Mat1 + Mat2");
    std::cout << "\n";
    Matrix fifthMatrix = Matrix(GetUserMatrix<double>());
    Matrix sixthMatrix = Matrix(GetUserMatrix<double>());
    PrintMatrix((fifthMatrix * sixthMatrix).GetStd(), "Mat1 * Mat2");
    std::cout << "\n";

    LOG("Task", 3);
    Matrix seventhMatrix = Matrix(GetUserMatrix<double>());
    PrintMatrix(seventhMatrix.GetStd(), "Get Determinant");
    double det = seventhMatrix.GetDeterminant();
    constexpr double EPSILON = 1e-9;
    LOG("Determinant", det);
    const char* result = std::fabs(det) < EPSILON ? "no" : "yes";
    std::cout << "Is there a matrix that is the inverse of this one? " << result << "\n";

    return resultTests;
}