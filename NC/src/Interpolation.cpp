#include "Interpolation.hpp"

Interpolation::Interpolation() {}

Interpolation::Interpolation(int n) : Matrix(n, 2) {}

void Interpolation::readDataFromFile(const string& filename) {
    ifstream fin(filename);
    if (!fin.is_open())
        throw runtime_error("Cannot open file: " + filename);

    int n;
    fin >> n;

    // Resize inherited Matrix to n×2
    rows = n;
    cols = 2;
    data.assign(n, vector<double>(2, 0.0));

    for (int i = 0; i < n; i++)
        fin >> data[i][0] >> data[i][1];
    fin.close();
}

double Interpolation::xAt(int i) const { return (*this)(i, 0); }
double Interpolation::fAt(int i) const { return (*this)(i, 1); }
int Interpolation::getN() const { return getRows(); }

void Interpolation::displayData(ostream& os) const {
    int n = getRows();
    os << "Data Points (" << n << " points):\n";
    os << fixed << setprecision(6);
    os << "  " << setw(12) << "x" << setw(12) << "f(x)" << "\n";
    for (int i = 0; i < n; i++)
        os << "  " << setw(12) << xAt(i) << setw(12) << fAt(i) << "\n";
}
