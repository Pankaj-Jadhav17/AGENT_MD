#include "EigenValue.hpp"

EigenValue::EigenValue() : n(0), maxIter(1000), tolerance(1e-10) {}

EigenValue::EigenValue(const Matrix& A, int maxIter, double tol)
    : A(A), n(A.getRows()), maxIter(maxIter), tolerance(tol) {
    if (!A.isSquare())
        throw runtime_error("EigenValue: matrix must be square");
}

void EigenValue::setMatrix(const Matrix& A) {
    this->A = A;
    n = A.getRows();
}

// Power method: finds dominant (largest magnitude) eigenvalue
double EigenValue::powerMethod() const {
    vector<double> x(n, 1.0);
    double eigenvalue = 0.0;

    for (int iter = 0; iter < maxIter; iter++) {
        // y = A * x
        vector<double> y(n, 0.0);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                y[i] += A(i, j) * x[j];

        // Find max magnitude component
        double maxVal = 0.0;
        for (int i = 0; i < n; i++)
            if (fabs(y[i]) > fabs(maxVal))
                maxVal = y[i];

        double newEigenvalue = maxVal;

        // Normalize
        for (int i = 0; i < n; i++)
            x[i] = y[i] / maxVal;

        if (fabs(newEigenvalue - eigenvalue) < tolerance)
            return newEigenvalue;

        eigenvalue = newEigenvalue;
    }
    return eigenvalue;
}

// Inverse power method: finds smallest magnitude eigenvalue
double EigenValue::inversePowerMethod() const {
    Matrix Ainv = A.inverse();
    vector<double> x(n, 1.0);
    double eigenvalue = 0.0;

    for (int iter = 0; iter < maxIter; iter++) {
        vector<double> y(n, 0.0);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                y[i] += Ainv(i, j) * x[j];

        double maxVal = 0.0;
        for (int i = 0; i < n; i++)
            if (fabs(y[i]) > fabs(maxVal))
                maxVal = y[i];

        double newEigenvalue = 1.0 / maxVal;

        for (int i = 0; i < n; i++)
            x[i] = y[i] / maxVal;

        if (fabs(newEigenvalue - eigenvalue) < tolerance)
            return newEigenvalue;

        eigenvalue = newEigenvalue;
    }
    return eigenvalue;
}

// QR Algorithm: finds all eigenvalues
vector<double> EigenValue::qrAlgorithm() const {
    Matrix Ak = A;

    for (int iter = 0; iter < maxIter; iter++) {
        // QR decomposition using Gram-Schmidt
        int m = Ak.getRows();
        Matrix Q(m, m), R(m, m);

        for (int j = 0; j < m; j++) {
            // Get column j of Ak
            vector<double> v = Ak.getCol(j);

            // Subtract projections
            for (int i = 0; i < j; i++) {
                double dot = 0.0;
                vector<double> qi = Q.getCol(i);
                for (int k = 0; k < m; k++)
                    dot += qi[k] * v[k];
                R(i, j) = dot;
                for (int k = 0; k < m; k++)
                    v[k] -= dot * qi[k];
            }

            // Normalize
            double norm = 0.0;
            for (int k = 0; k < m; k++)
                norm += v[k] * v[k];
            norm = sqrt(norm);
            R(j, j) = norm;

            if (norm > 1e-12)
                for (int k = 0; k < m; k++)
                    Q(k, j) = v[k] / norm;
        }

        // A_{k+1} = R * Q
        Ak = R * Q;

        // Check convergence: are sub-diagonal elements near zero?
        double offDiag = 0.0;
        for (int i = 1; i < m; i++)
            offDiag += fabs(Ak(i, i - 1));
        if (offDiag < tolerance) break;
    }

    // Eigenvalues are on the diagonal
    vector<double> eigenvalues(n);
    for (int i = 0; i < n; i++)
        eigenvalues[i] = Ak(i, i);
    return eigenvalues;
}

// Gershgorin circle bounds: each disc centered at A(i,i) with radius = sum|A(i,j)| for j!=i
vector<pair<double, double>> EigenValue::gershgorinBounds() const {
    vector<pair<double, double>> bounds(n);
    for (int i = 0; i < n; i++) {
        double center = A(i, i);
        double radius = 0.0;
        for (int j = 0; j < n; j++)
            if (j != i) radius += fabs(A(i, j));
        bounds[i] = {center - radius, center + radius};
    }
    return bounds;
}

// Condition number: ||A|| * ||A^{-1}|| using infinity norm
double EigenValue::conditionNumber() const {
    double normA = A.norm();
    Matrix Ainv = A.inverse();
    double normAinv = Ainv.norm();
    return normA * normAinv;
}

void EigenValue::displayGershgorin(ostream& os) const {
    auto bounds = gershgorinBounds();
    os << "Gershgorin Disc Bounds:\n";
    for (int i = 0; i < n; i++)
        os << "  Disc " << i << ": [" << bounds[i].first
           << ", " << bounds[i].second << "]\n";
}
