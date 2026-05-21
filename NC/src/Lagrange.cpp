#include "Lagrange.hpp"

Lagrange::Lagrange() {}
Lagrange::Lagrange(int n) : Interpolation(n) {}

// Compute the i-th Lagrange basis polynomial L_i(xval)
// L_i(x) = product of (x - x_j) / (x_i - x_j) for all j != i
double Lagrange::basisPoly(int i, double xval) const {
    int n = getN();
    double result = 1.0;
    for (int j = 0; j < n; j++) {
        if (j != i)
            result *= (xval - xAt(j)) / (xAt(i) - xAt(j));
    }
    return result;
}

// Interpolate: P(x) = sum of f_i * L_i(x)
double Lagrange::interpolate(double xval) {
    int n = getN();
    double result = 0.0;
    for (int i = 0; i < n; i++)
        result += fAt(i) * basisPoly(i, xval);
    return result;
}
