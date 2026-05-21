#include "LUCholesky.hpp"

LUCholesky::LUCholesky() {}
LUCholesky::LUCholesky(int n) : SLE(n) {}

// Cholesky decomposition: A = L * L^T (requires symmetric positive-definite A).
// 'this' (inherited Matrix) is the coefficient matrix A.
vector<double> LUCholesky::solve() {
    int n = getRows();

    if (!isSymmetric())
        throw runtime_error("LUCholesky: matrix must be symmetric");

    Matrix L(n, n);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= i; j++) {
            double sum = 0.0;
            for (int k = 0; k < j; k++)
                sum += L(i, k) * L(j, k);

            if (i == j) {
                double val = (*this)(i, i) - sum;
                if (val <= 0.0)
                    throw runtime_error("LUCholesky: matrix is not positive definite");
                L(i, j) = sqrt(val);
            } else {
                L(i, j) = ((*this)(i, j) - sum) / L(j, j);
            }
        }
    }

    // Forward substitution: L * y = b
    vector<double> y(n, 0.0);
    for (int i = 0; i < n; i++) {
        double sum = 0.0;
        for (int j = 0; j < i; j++)
            sum += L(i, j) * y[j];
        y[i] = (b[i] - sum) / L(i, i);
    }

    // Back substitution: L^T * x = y
    vector<double> x(n, 0.0);
    for (int i = n - 1; i >= 0; i--) {
        double sum = 0.0;
        for (int j = i + 1; j < n; j++)
            sum += L(j, i) * x[j];
        x[i] = (y[i] - sum) / L(i, i);
    }
    return x;
}
