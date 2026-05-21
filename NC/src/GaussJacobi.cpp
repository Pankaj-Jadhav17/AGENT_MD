#include "GaussJacobi.hpp"

GaussJacobi::GaussJacobi() : maxIter(1000), tolerance(1e-10) {}
GaussJacobi::GaussJacobi(int n, int maxIter, double tol)
    : SLE(n), maxIter(maxIter), tolerance(tol) {}

// Jacobi iterative method.
// 'this' (inherited Matrix) is the coefficient matrix A.
vector<double> GaussJacobi::solve() {
    int n = getRows();
    vector<double> x(n, 0.0);
    vector<double> xNew(n, 0.0);

    for (int iter = 0; iter < maxIter; iter++) {
        for (int i = 0; i < n; i++) {
            double sum = 0.0;
            for (int j = 0; j < n; j++)
                if (j != i)
                    sum += (*this)(i, j) * x[j];
            xNew[i] = (b[i] - sum) / (*this)(i, i);
        }

        double maxDiff = 0.0;
        for (int i = 0; i < n; i++)
            maxDiff = max(maxDiff, fabs(xNew[i] - x[i]));

        x = xNew;
        if (maxDiff < tolerance) return x;
    }
    return x;
}
