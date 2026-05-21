#include "GaussSeidel.hpp"

GaussSeidel::GaussSeidel() : maxIter(1000), tolerance(1e-10) {}
GaussSeidel::GaussSeidel(int n, int maxIter, double tol)
    : SLE(n), maxIter(maxIter), tolerance(tol) {}

// Gauss-Seidel: uses updated values immediately (unlike Jacobi).
// 'this' (inherited Matrix) is the coefficient matrix A.
vector<double> GaussSeidel::solve() {
    int n = getRows();
    vector<double> x(n, 0.0);

    for (int iter = 0; iter < maxIter; iter++) {
        double maxDiff = 0.0;

        for (int i = 0; i < n; i++) {
            double sum = 0.0;
            for (int j = 0; j < n; j++)
                if (j != i)
                    sum += (*this)(i, j) * x[j];

            double xNew = (b[i] - sum) / (*this)(i, i);
            maxDiff = max(maxDiff, fabs(xNew - x[i]));
            x[i] = xNew;
        }

        if (maxDiff < tolerance) return x;
    }
    return x;
}
