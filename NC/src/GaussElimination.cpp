#include "GaussElimination.hpp"

GaussElimination::GaussElimination() {}
GaussElimination::GaussElimination(int n) : SLE(n) {}

// Gauss Elimination with partial pivoting and back substitution.
// 'this' (inherited Matrix) is the coefficient matrix A.
vector<double> GaussElimination::solve() {
    int n = getRows();
    // Build augmented matrix [A | b]
    Matrix aug(n, n + 1);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            aug(i, j) = (*this)(i, j);
        aug(i, n) = b[i];
    }

    // Forward elimination with partial pivoting
    for (int i = 0; i < n; i++) {
        int maxRow = i;
        for (int k = i + 1; k < n; k++)
            if (fabs(aug(k, i)) > fabs(aug(maxRow, i)))
                maxRow = k;
        aug.swapRows(i, maxRow);

        if (fabs(aug(i, i)) < 1e-12)
            throw runtime_error("GaussElimination: singular or nearly singular matrix");

        for (int k = i + 1; k < n; k++) {
            double factor = aug(k, i) / aug(i, i);
            for (int j = i; j <= n; j++)
                aug(k, j) -= factor * aug(i, j);
        }
    }

    // Back substitution
    vector<double> x(n, 0.0);
    for (int i = n - 1; i >= 0; i--) {
        x[i] = aug(i, n);
        for (int j = i + 1; j < n; j++)
            x[i] -= aug(i, j) * x[j];
        x[i] /= aug(i, i);
    }
    return x;
}
