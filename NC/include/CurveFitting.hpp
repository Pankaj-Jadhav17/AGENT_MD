#ifndef CURVEFITTING_HPP
#define CURVEFITTING_HPP

#include "Matrix.hpp"
#include <vector>

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

    double evaluate(double xval, const vector<double>& coeffs) const;

    double rmsError(const vector<double>& coeffs) const;

    // Display
    void displayData(ostream& os = cout) const;
    void displayFit(int degree, ostream& os = cout) const;
};

#endif
