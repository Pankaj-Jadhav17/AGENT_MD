#include "LeastSquares.hpp"

LeastSquares::LeastSquares() {}
LeastSquares::LeastSquares(int n) : CurveFitting(n) {}

// Linear fit: y = a*x + b (degree-1 polynomial using closed-form formulas)
pair<double, double> LeastSquares::linearFit() const {
    int n = getN();
    double sumX = 0, sumF = 0, sumXF = 0, sumX2 = 0;

    for (int i = 0; i < n; i++) {
        double xi = xAt(i), fi = fAt(i);
        sumX  += xi;
        sumF  += fi;
        sumXF += xi * fi;
        sumX2 += xi * xi;
    }

    double a = (n * sumXF - sumX * sumF) / (n * sumX2 - sumX * sumX);
    double b = (sumF - a * sumX) / n;
    return {a, b};
}

double LeastSquares::evaluate(double x, double a, double b) const {
    return a * x + b;
}

double LeastSquares::rmsError(double a, double b) const {
    int n = getN();
    double sumSq = 0.0;
    for (int i = 0; i < n; i++) {
        double residual = fAt(i) - evaluate(xAt(i), a, b);
        sumSq += residual * residual;
    }
    return sqrt(sumSq / n);
}

void LeastSquares::displayLinearFit(ostream& os) const {
    auto [a, b] = linearFit();
    int n = getN();

    os << fixed << setprecision(6);
    os << "\nLinear Fit: y = " << a << " * x + " << b << "\n";
    os << "RMS Error:  " << rmsError(a, b) << "\n";

    os << "\nFitted values:\n";
    os << "  " << setw(12) << "x" << setw(12) << "f(x)"
       << setw(16) << "y_fit" << setw(14) << "residual" << "\n";
    for (int i = 0; i < n; i++) {
        double xi = xAt(i);
        double fi = fAt(i);
        double yi = evaluate(xi, a, b);
        os << "  " << setw(12) << xi << setw(12) << fi
           << setw(16) << yi << setw(14) << (fi - yi) << "\n";
    }
}
