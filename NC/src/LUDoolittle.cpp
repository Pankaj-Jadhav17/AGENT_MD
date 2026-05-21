#include "LUDoolittle.hpp"

LUDoolittle::LUDoolittle() {}
LUDoolittle::LUDoolittle(int n) : SLE(n) {}

// Doolittle decomposition: L has unit diagonal, U has computed diagonal.
// 'this' (inherited Matrix) is the coefficient matrix A.
vector<double> LUDoolittle::solve() {
    int n = getRows();
    Matrix L(n, n), U(n, n);

    for (int i = 0; i < n; i++)
        L(i, i) = 1.0;

    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            double sum = 0.0;
            for (int k = 0; k < i; k++)
                sum += L(i, k) * U(k, j);
            U(i, j) = (*this)(i, j) - sum;
        }
        for (int j = i + 1; j < n; j++) {
            double sum = 0.0;
            for (int k = 0; k < i; k++)
                sum += L(j, k) * U(k, i);
            if (fabs(U(i, i)) < 1e-12)
                throw runtime_error("LUDoolittle: zero pivot encountered");
            L(j, i) = ((*this)(j, i) - sum) / U(i, i);
        }
    }

    // Forward substitution: L * y = b
    vector<double> y(n, 0.0);
    for (int i = 0; i < n; i++) {
        double sum = 0.0;
        for (int j = 0; j < i; j++)
            sum += L(i, j) * y[j];
        y[i] = b[i] - sum;
    }

    // Back substitution: U * x = y
    vector<double> x(n, 0.0);
    for (int i = n - 1; i >= 0; i--) {
        double sum = 0.0;
        for (int j = i + 1; j < n; j++)
            sum += U(i, j) * x[j];
        x[i] = (y[i] - sum) / U(i, i);
    }
    return x;
}
