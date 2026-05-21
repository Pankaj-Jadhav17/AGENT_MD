#ifndef LEASTSQUARES_HPP
#define LEASTSQUARES_HPP

#include "CurveFitting.hpp"
#include <utility>

// LeastSquares inherits CurveFitting (which inherits Matrix).
// Specialises for linear fit: y = a*x + b (degree-1 polynomial).
class LeastSquares : public CurveFitting {
public:
    LeastSquares();
    LeastSquares(int n);

    // Linear fit: returns {a, b} for y = a*x + b
    pair<double, double> linearFit() const;

    // Evaluate linear fit at x
    double evaluate(double x, double a, double b) const;

    // RMS error for linear fit
    double rmsError(double a, double b) const;

    // Display linear fit results
    void displayLinearFit(ostream& os = cout) const;
};

#endif // LEASTSQUARES_HPP
