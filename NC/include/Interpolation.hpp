#ifndef INTERPOLATION_HPP
#define INTERPOLATION_HPP

#include "Matrix.hpp"

// Interpolation inherits Matrix.
// Stores data points as an n×2 matrix (col-0 = x, col-1 = f(x)).
class Interpolation : public Matrix {
public:
    Interpolation();
    Interpolation(int n);
    virtual ~Interpolation() = default;

    // Load data points from file (format: first line = n, then n lines of "x f")
    void readDataFromFile(const string& filename);

    // Accessors
    double xAt(int i) const;
    double fAt(int i) const;
    int getN() const;

    // Pure virtual: subclasses implement their interpolation strategy
    virtual double interpolate(double xval) = 0;

    // Display data points
    void displayData(ostream& os = cout) const;
};

#endif // INTERPOLATION_HPP
