#ifndef EIGENVALUE_HPP
#define EIGENVALUE_HPP

#include "Matrix.hpp"
#include <utility>

class EigenValue {
private:
    Matrix A;
    int n;
    int maxIter;
    double tolerance;

public:
    EigenValue();
    EigenValue(const Matrix& A, int maxIter = 1000, double tol = 1e-10);

    void setMatrix(const Matrix& A);

    // Eigenvalue methods
    double powerMethod() const;
    double inversePowerMethod() const;
    vector<double> qrAlgorithm() const;

    // Auxiliary
    vector<pair<double, double>> gershgorinBounds() const;
    double conditionNumber() const;

    // Display helpers
    void displayGershgorin(ostream& os = cout) const;
};

#endif // EIGENVALUE_HPP
