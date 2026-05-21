#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>
#include <stdexcept>
#include <iomanip>
#include <algorithm>

using namespace std;

class Matrix {
protected:
    int rows, cols;
    vector<vector<double>> data;

public:
    Matrix();
    Matrix(int r, int c);
    Matrix(int r, int c, double val);

    // Access
    int getRows() const;
    int getCols() const;
    double& operator()(int i, int j);
    double operator()(int i, int j) const;
    vector<double> getRow(int i) const;
    vector<double> getCol(int j) const;
    void setRow(int i, const vector<double>& row);

    // Arithmetic operators
    Matrix operator+(const Matrix& other) const;
    Matrix operator-(const Matrix& other) const;
    Matrix operator*(const Matrix& other) const;
    Matrix operator*(double scalar) const;
    bool operator==(const Matrix& other) const;

    // Matrix operations
    double determinant() const;
    Matrix inverse() const;
    Matrix transpose() const;

    // Properties
    bool isSquare() const;
    bool isSymmetric() const;
    bool isDiagonal() const;
    bool isDiagonallyDominant() const;
    bool isNull() const;

    // I/O
    void readFromFile(const string& filename);
    void displayToFile(const string& filename) const;
    void display(ostream& os = cout) const;

    // Utility
    static Matrix identity(int n);
    double norm() const; // infinity norm
    void swapRows(int i, int j);
};

#endif // MATRIX_HPP
