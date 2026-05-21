#ifndef CURVEFITTING_HPP
#define CURVEFITTING_HPP

#include "Matrix.hpp"
#include <vector>

// CurveFitting inherits Matrix.
// Stores data points as an n×2 matrix (col-0 = x, col-1 = f(x)).
// Provides polynomial curve fitting via normal equations.
class CurveFitting : public Matrix {
public:
    CurveFitting();
    CurveFitting(int n);
    virtual ~CurveFitting() = default;

    // Load data from file (format: first line = n, then n lines of "x f")
    void readDataFromFile(const string& filename);

    // Accessors
    double xAt(int i) const;
    double fAt(int i) const;
    int getN() const;

    // Polynomial fit of given degree: f(x) = c0 + c1*x + c2*x^2 + ...
    vector<double> fit(int degree) const;

    // Evaluate polynomial at xval given coefficients
    double evaluate(double xval, const vector<double>& coeffs) const;

    // Root mean square error
    double rmsError(const vector<double>& coeffs) const;

    // Display
    void displayData(ostream& os = cout) const;
    void displayFit(int degree, ostream& os = cout) const;
};

#endif // CURVEFITTING_HPP
