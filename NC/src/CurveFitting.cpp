#include "CurveFitting.hpp"
#include <fstream>
#include <iomanip>
#include <stdexcept>
#include <cmath>

CurveFitting::CurveFitting() {}

CurveFitting::CurveFitting(int n) : Matrix(n, 2) {}

void CurveFitting::readDataFromFile(const string& filename) {
    ifstream fin(filename);
    if (!fin.is_open())
        throw runtime_error("Cannot open file: " + filename);

    int n;
    fin >> n;

    rows = n;
    cols = 2;
    data.assign(n, vector<double>(2, 0.0));

    for (int i = 0; i < n; i++)
        fin >> data[i][0] >> data[i][1];
    fin.close();
}

double CurveFitting::xAt(int i) const { return (*this)(i, 0); }
double CurveFitting::fAt(int i) const { return (*this)(i, 1); }
int CurveFitting::getN() const { return getRows(); }

vector<double> CurveFitting::fit(int degree) const {
    int n = getN();
    int m = degree + 1; // number of coefficients

    if (m > n)
        throw runtime_error("CurveFitting: degree too high for number of data points");

    // Build normal equation matrix (X^T * X) of size m x m
    Matrix XtX(m, m);
    for (int j = 0; j < m; j++)
        for (int k = 0; k < m; k++)
            for (int i = 0; i < n; i++)
                XtX(j, k) += pow(xAt(i), j + k);

    // Build right-hand side (X^T * f) of size m
    vector<double> Xtf(m, 0.0);
    for (int j = 0; j < m; j++)
        for (int i = 0; i < n; i++)
            Xtf[j] += pow(xAt(i), j) * fAt(i);

    // Solve using Gauss elimination on augmented matrix [XtX | Xtf]
    Matrix aug(m, m + 1);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++)
            aug(i, j) = XtX(i, j);
        aug(i, m) = Xtf[i];
    }

    // Forward elimination with partial pivoting
    for (int i = 0; i < m; i++) {
        int maxRow = i;
        for (int k = i + 1; k < m; k++)
            if (fabs(aug(k, i)) > fabs(aug(maxRow, i)))
                maxRow = k;
        aug.swapRows(i, maxRow);

        if (fabs(aug(i, i)) < 1e-12)
            throw runtime_error("CurveFitting: singular normal equations");

        for (int k = i + 1; k < m; k++) {
            double factor = aug(k, i) / aug(i, i);
            for (int j = i; j <= m; j++)
                aug(k, j) -= factor * aug(i, j);
        }
    }

    // Back substitution
    vector<double> coeffs(m, 0.0);
    for (int i = m - 1; i >= 0; i--) {
        coeffs[i] = aug(i, m);
        for (int j = i + 1; j < m; j++)
            coeffs[i] -= aug(i, j) * coeffs[j];
        coeffs[i] /= aug(i, i);
    }
    return coeffs;
}

// Evaluate polynomial: c0 + c1*x + c2*x^2 + ...
double CurveFitting::evaluate(double xval, const vector<double>& coeffs) const {
    double result = 0.0;
    double xPow = 1.0;
    for (size_t i = 0; i < coeffs.size(); i++) {
        result += coeffs[i] * xPow;
        xPow *= xval;
    }
    return result;
}

// Root mean square error
double CurveFitting::rmsError(const vector<double>& coeffs) const {
    int n = getN();
    double sumSq = 0.0;
    for (int i = 0; i < n; i++) {
        double residual = fAt(i) - evaluate(xAt(i), coeffs);
        sumSq += residual * residual;
    }
    return sqrt(sumSq / n);
}

void CurveFitting::displayData(ostream& os) const {
    int n = getN();
    os << "Curve Fitting Data (" << n << " points):\n";
    os << fixed << setprecision(6);
    os << "  " << setw(12) << "x" << setw(12) << "f(x)" << "\n";
    for (int i = 0; i < n; i++)
        os << "  " << setw(12) << xAt(i) << setw(12) << fAt(i) << "\n";
}

void CurveFitting::displayFit(int degree, ostream& os) const {
    auto coeffs = fit(degree);
    os << fixed << setprecision(6);
    os << "Polynomial Fit (degree " << degree << "):\n";
    os << "  f(x) = ";
    for (int i = 0; i < (int)coeffs.size(); i++) {
        if (i > 0) {
            if (coeffs[i] >= 0) os << " + ";
            else os << " - ";
            os << fabs(coeffs[i]);
            if (i == 1) os << "*x";
            else os << "*x^" << i;
        } else {
            os << coeffs[i];
        }
    }
    os << "\n";
    os << "  RMS Error: " << rmsError(coeffs) << "\n";
}
