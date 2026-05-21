#include "LUCrout.hpp"

LUCrout::LUCrout() {}
LUCrout::LUCrout(int n) : SLE(n) {}

// Crout decomposition: L has computed diagonal, U has unit diagonal.
// 'this' (inherited Matrix) is the coefficient matrix A.
vector<double> LUCrout::solve() {
    int n = getRows();
    Matrix L(n, n), U(n, n);

    for (int i = 0; i < n; i++)
        U(i, i) = 1.0;

    for (int j = 0; j < n; j++) {
        for (int i = j; i < n; i++) {
            double sum = 0.0;
            for (int k = 0; k < j; k++)
                sum += L(i, k) * U(k, j);
            L(i, j) = (*this)(i, j) - sum;
        }
        for (int i = j + 1; i < n; i++) {
            double sum = 0.0;
            for (int k = 0; k < j; k++)
                sum += L(j, k) * U(k, i);
            if (fabs(L(j, j)) < 1e-12)
                throw runtime_error("LUCrout: zero pivot encountered");
            U(j, i) = ((*this)(j, i) - sum) / L(j, j);
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

    // Back substitution: U * x = y
    vector<double> x(n, 0.0);
    for (int i = n - 1; i >= 0; i--) {
        double sum = 0.0;
        for (int j = i + 1; j < n; j++)
            sum += U(i, j) * x[j];
        x[i] = y[i] - sum;
    }
    return x;
}
